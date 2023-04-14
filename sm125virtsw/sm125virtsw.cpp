/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "sm125virtsw.h"

#define RETCODE_OK 0
#define RETCODE_FAIL 1

// https://github.com/fpagliughi/sockpp/blob/master/examples/tcp/tcpechosvr.cpp

interrogator* ig = nullptr;
std::unique_ptr<uint8_t[]> lastSpectrumData = nullptr;
size_t lastSpectrumDataLength = 0;
unsigned int delaySeconds = 0;
bool workOnServer = false;
bool handleSetActiveChannels = false;
int forceRefresh = 0;

void PrintError(std::string msg)
{
	std::cerr << "[Error] " << msg << std::endl;
}

std::string getReplyLength(size_t size)
{
	const int lenSize = 10;
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(lenSize) << size;

	return oss.str();
}

std::string prependReply(std::string cmd, std::string reply)
{
	return cmd + " " + reply;
}

void sourceThreadHandler()
{
	ig->set_channels(16);

	while (true)
	{
		try
		{
			while (workOnServer)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			for (int i = 0; i < 8; ++i)
			{
				lastSpectrumData = ig->recv_spectrums_data(lastSpectrumDataLength);
			}

			std::string timestamp = std::format("[{:%d-%m-%Y %H:%M:%OS}] ", std::chrono::system_clock::now());
			std::cout << timestamp << "Refreshed source spectrum." << std::endl;
		}
		catch (std::exception& ex)
		{
			PrintError(ex.what());
		}

		if (delaySeconds > 0)
		{
			std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));
		}
	}
}

void serverThreadHandler(sockpp::tcp_socket sock)
{
	ssize_t n;
	char buf[2048];
	std::string threadId = "[" + std::to_string(sock.peer_address().port()) + "] ";

	while ((n = sock.read(buf, sizeof(buf))) > 0)
	{
		std::string timestamp = std::format("[{:%d-%m-%Y %H:%M:%OS}] ", std::chrono::system_clock::now());
		std::string cmd(buf, buf + n);

		if (cmd.back() != '\n')
			cmd.push_back('\n');

		// trim newlines
		cmd = cmd.substr(0, cmd.find_last_of('\n') + 1);
		std::string cmdPrint = cmd.substr(0, cmd.size() - 1);

		std::string originalCmd = cmdPrint;

		// check if the command has some parameters
		if (cmd.find(' ') != std::string::npos)
		{
			cmd = cmd.substr(0, cmd.find(' '));
		}

		std::string reply;

		// handle commands
		if (cmdPrint == SM125_GET_PRODUCT_SN)
		{
			// #PRODUCT_SN SIA9L6
			reply = prependReply("#PRODUCT_SN", ig->serial_number());
		}
		else if (cmdPrint == SM125_IDN)
		{
			// #IDN : sm125 v2, Rev 2.109, Date Created: Mon Nov 14 15:26:50 EST 2011
			reply = prependReply("#IDN", ig->idn());
		}
		else if (cmdPrint == SM125_GET_SYSTEM_IMAGE_ID)
		{
			// #SYSTEM_IMAGE_ID : sm125 v2, Rev 2.109, Date Created: Mon Nov 14 15:26:50 EST 2011
			reply = prependReply("#SYSTEM_IMAGE_ID", ig->system_image_id());
		}
		else if (cmdPrint == SM125_GET_NUM_DAQ_DUTS)
		{
			// #NUM_DAQ_DUTS 4
			reply = prependReply("#NUM_DAQ_DUTS", ig->num_daq_duts());
		}
		else if (cmdPrint == SM125_GET_SCAN_SPEED)
		{
			// #SCAN_SPEED 2
			reply = prependReply("#SCAN_SPEED", ig->scan_speed());
		}
		else if (cmdPrint == SM125_GET_MIN_WVL_USER_DATA)
		{
			// #MIN_WVL_USER_DATA 1510.0000
			reply = prependReply("#MIN_WVL_USER_DATA", ig->min_wvl());
		}
		else if (cmdPrint == SM125_GET_MAX_WVL_USER_DATA)
		{
			// #MAX_WVL_USER_DATA 1590.0000
			reply = prependReply("#MAX_WVL_USER_DATA", ig->max_wvl());
		}
		else if (cmdPrint == SM125_GET_DATA && forceRefresh <= 0)
		{
			if (lastSpectrumData == nullptr)
				continue;
		
			std::string lenMsg = getReplyLength(lastSpectrumDataLength);
		
			//std::cout << threadId << "Sending: " << lenMsg << std::endl;
			sock.write_n(lenMsg.c_str(), lenMsg.size());
		
			// artificial pause between replies, to simulate network latency
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		
			//std::cout << threadId << "Sending last spectrum data." << std::endl;
			sock.write_n(lastSpectrumData.get(), lastSpectrumDataLength);
		
			continue;
		}
		//else if (cmd.rfind("#SET_DUT", 0) == 0) // ignore requests for setting channels
		//{
		//	// #DUT1_STATE 1
		//	//reply = "#" + originalCmd.substr(originalCmd.find('_') + 1);
		//	reply = "#" + std::string(originalCmd.begin() + originalCmd.find('_') + 1, originalCmd.begin() + originalCmd.find(' ')) + " 1";
		//}
		//else if (cmd.rfind(SM125_SET_NORMALIZE_DUTS, 0) == 0) // undocumented
		//{
		//	// #NORMALIZE_DUTS 1
		//	reply = "#NORMALIZE_DUTS 1";
		//}
		else if (!handleSetActiveChannels && cmd.rfind("#SET_DUT", 0) == 0) // skip requests to set active channels by default
		{
			reply = "#" + std::string(originalCmd.begin() + originalCmd.find('_') + 1, originalCmd.begin() + originalCmd.find(' ')) + " 1";
		}
		else
		{
			workOnServer = true;

			std::cout << timestamp << threadId << "Warning: unknown command " << cmdPrint << std::endl;
			std::cout << timestamp << threadId << "Passing through to device..." << std::endl;

			size_t replyLength;
			auto data = ig->send_command(originalCmd, replyLength);
			auto dataAsString = reinterpret_cast<const char*>(data.get());

			std::string lenMsg = getReplyLength(replyLength);

			std::cout << timestamp << threadId << "Sending: " << lenMsg << std::endl;
			sock.write_n(lenMsg.c_str(), lenMsg.size());

			std::cout << timestamp << threadId << "Sending: " << dataAsString << std::endl;
			sock.write_n(data.get(), replyLength);

			if (cmd.rfind("#SET_DUT16_STATE", 0) == 0) // after setting DUTS, force refresh
			{
				forceRefresh = 15;
				//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				//for (int i = 0; i < 8; ++i)
				//{
				//	lastSpectrumData = ig->recv_spectrums_data(lastSpectrumDataLength);
				//}
			}

			if (forceRefresh > 0)
				--forceRefresh;

			workOnServer = false;

			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			continue;
		}

		std::cout << timestamp << threadId << "Received: " << cmdPrint << std::endl;

		std::vector<char> replyBuffer(reply.begin(), reply.end());
		replyBuffer.push_back('\0');

		std::string lenMsg = getReplyLength(replyBuffer.size());

		std::cout << timestamp << threadId << "Sending: " << lenMsg << std::endl;
		sock.write_n(lenMsg.c_str(), lenMsg.size());

		std::cout << timestamp << threadId << "Sending: " << reply << std::endl;
		sock.write_n(replyBuffer.data(), replyBuffer.size());
	}

	std::string timestamp = std::format("[{:%d-%m-%Y %H:%M:%OS}] ", std::chrono::system_clock::now());
	std::cout << timestamp << "Connection closed from " << sock.peer_address() << std::endl;
}

int main(int argc, char* argv[])
{
	try
	{
		// setup cmdline options using cxxoptions library
		cxxopts::Options options("sm125virtsw", "SM125 Virtual Switch");

		options.add_options()
			("a,address", "Device interface IP address.", cxxopts::value<std::string>())
			("p,port", "Port for connections to this switch.", cxxopts::value<unsigned int>())
			("d,delay", "Hold each sample for [delay] seconds.", cxxopts::value<unsigned int>()->default_value("0"))
			("c,channels", "Handle requests to set active channels.", cxxopts::value<bool>())
			("h,help", "Print usage.")
			;

		auto result = options.parse(argc, argv);

		// show help
		if (result.count("help") || !result.count("address") || !result.count("port"))
		{
			std::cout << options.help() << std::endl;
			return RETCODE_FAIL;
		}

		std::string address = result["address"].as<std::string>();
		unsigned int port = result["port"].as<unsigned int>();
		delaySeconds = result["delay"].as<unsigned int>();
		handleSetActiveChannels = result["channels"].as<bool>();

		sockpp::socket_initializer sockInit; // needed for proper function
		sockpp::tcp_acceptor acc(port);

		if (!acc)
			PrintError(acc.last_error_str());

		std::cout << "Connecting to " << address << ":" << sm125_port << std::endl;

		ig = new interrogator(address, sm125_port);
		if (!ig->ping())
			return RETCODE_FAIL;

		std::thread sourceThread(sourceThreadHandler);
		sourceThread.detach();

		while (lastSpectrumData == nullptr)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		std::cout << "Succesfully connected, waiting for connections." << std::endl;

		while (true)
		{
			sockpp::inet_address peer;

			// Accept a new client connection
			sockpp::tcp_socket sock = acc.accept(&peer);

			std::string timestamp = std::format("[{:%d-%m-%Y %H:%M:%OS}] ", std::chrono::system_clock::now());
			std::cout << timestamp << "Received a connection request from " << peer << std::endl;

			if (!sock)
			{
				std::cerr << timestamp << "Error accepting incoming connection: "
					<< acc.last_error_str() << std::endl;
			}
			else
			{
				// Create a thread and transfer the new stream to it.
				std::thread serverThread(serverThreadHandler, std::move(sock));
				serverThread.detach();
			}
		}
	}
	catch (std::exception& ex)
	{
		PrintError(ex.what());
#if !NDEBUG
		system("pause");
#endif
		return RETCODE_FAIL;
	}

	return RETCODE_OK;
}
