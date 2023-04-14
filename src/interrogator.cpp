/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "interrogator.h"

interrogator::interrogator(std::string address, uint16_t port) : m_address(address), m_port(port)
{
	sockpp::inet_address sockAddress(address, port);
	sockpp::tcp_connector* conn = new sockpp::tcp_connector(sockAddress);

	if (!*conn)
		throw std::exception(std::format("Cannot connect to {}.\n\t{}", sockAddress.to_string(), conn->last_error_str()).c_str());

	conn->read_timeout(std::chrono::seconds(5));

	this->m_conn = conn;

	if (!this->ping())
		throw std::exception("Device does not respond.");

	this->recv_serial_number();
	this->recv_system_image_id();
	this->recv_num_daq_duts();
	this->recv_scan_speed();
	this->recv_min_max_wvl();
}

interrogator::interrogator(interrogator&& other) noexcept
{
	m_conn = other.m_conn;
	m_address = other.m_address;
	m_port = other.m_port;
	m_serial_number = other.m_serial_number;
	m_idn = other.m_idn;
	m_sockinit = other.m_sockinit;

	other.m_conn = nullptr;
}

interrogator::~interrogator()
{
	delete this->m_conn;
}

bool interrogator::ping()
{
	try
	{
		auto data = send_command(SM125_IDN);

		std::string idn(reinterpret_cast<const char*>(data.get()));
		this->m_idn = parse_str_reply(idn);

		return true;
	}
	catch (std::exception&)
	{
		return false;
	}
}

void interrogator::set_channels(uint8_t count)
{
	for (uint8_t i = 1; i <= spectrum_max_channels; i++)
	{
		std::string legacy = convert_legacy_channel(i);

		if (i > count)
		{
			send_command(std::format(SM125_SET_DUT_STATE, legacy, 0));
		}
		else
		{
			send_command(std::format(SM125_SET_DUT_STATE, legacy, 1));
		}
	}
}

#pragma region Getters

spectrum_sm125* interrogator::spectrums()
{
	return &this->m_spectrums;
}

std::string interrogator::num_daq_duts()
{
	return this->m_num_daq_duts;
}

std::string interrogator::serial_number()
{
	return this->m_serial_number;
}

std::string interrogator::idn()
{
	return this->m_idn;
}

std::string interrogator::system_image_id()
{
	return this->m_system_image_id;
}

std::string interrogator::scan_speed()
{
	return this->m_scan_speed;
}

std::string interrogator::min_wvl()
{
	return m_min_wvl;
}

std::string interrogator::max_wvl()
{
	return m_max_wvl;
}

#pragma endregion

#pragma region Internal Recv Functions

void interrogator::recv_min_max_wvl()
{
	auto data_min = send_command(SM125_GET_MIN_WVL_USER_DATA);
	auto data_max = send_command(SM125_GET_MAX_WVL_USER_DATA);

	std::string wvl_min(reinterpret_cast<const char*>(data_min.get()));
	std::string wvl_max(reinterpret_cast<const char*>(data_max.get()));

	this->m_min_wvl = parse_str_reply(wvl_min);
	this->m_max_wvl = parse_str_reply(wvl_max);
}

void interrogator::recv_scan_speed()
{
	auto data = send_command(SM125_GET_SCAN_SPEED);

	std::string scan_speed(reinterpret_cast<const char*>(data.get()));
	this->m_scan_speed = parse_str_reply(scan_speed);
}

void interrogator::recv_num_daq_duts()
{
	auto data = send_command(SM125_GET_NUM_DAQ_DUTS);

	std::string num_daq_duts(reinterpret_cast<const char*>(data.get()));
	this->m_num_daq_duts = parse_str_reply(num_daq_duts);
}

void interrogator::recv_system_image_id()
{
	auto data = send_command(SM125_GET_SYSTEM_IMAGE_ID);

	std::string system_image_id(reinterpret_cast<const char*>(data.get()));
	this->m_system_image_id = parse_str_reply(system_image_id);
}

void interrogator::recv_serial_number()
{
	auto data = send_command(SM125_GET_PRODUCT_SN);

	std::string serialNumber(reinterpret_cast<const char*>(data.get()));
	this->m_serial_number = parse_str_reply(serialNumber);
}

void interrogator::recv_spectrums()
{
	auto data = send_command(SM125_GET_DATA);

	this->m_spectrums.update(data.get());
}

std::unique_ptr<uint8_t[]> interrogator::recv_spectrums_data(size_t& dataLength)
{
	auto data = send_command(SM125_GET_DATA, dataLength);
	return std::move(data);
}

#pragma endregion

std::unique_ptr<uint8_t[]> interrogator::send_command(std::string cmd)
{
	size_t replyLength;
	auto data = send_command(cmd, replyLength);

	return std::move(data);
}

std::unique_ptr<uint8_t[]> interrogator::send_command(std::string cmd, size_t& replyLength)
{
	std::lock_guard<std::mutex> guard(m_socket_mutex);

	if (!this->m_conn->is_connected())
		throw std::exception("Connection has been lost.");

	// command must end with a line feed character (chapter 5.1.2 in Enlight API manual)
	cmd.append("\n");

	size_t bytesSize = this->m_conn->write_n(cmd.c_str(), cmd.length());

	if (bytesSize < 0)
		throw std::exception(std::format("Exception occured while sending a command.\n\t{}", m_conn->last_error_str()).c_str());
	else if (bytesSize != cmd.length())
		throw std::exception("Exception occured while sending a command. Invalid number of bytes were written.");

	// read what will be the size of the message
	constexpr size_t msgLengthSize = 10;
	uint8_t msgLength[msgLengthSize + 1];
	msgLength[msgLengthSize] = '\0'; // the device sends us unterminated ASCII string

	bytesSize = this->m_conn->read_n(&msgLength, msgLengthSize);

	if (bytesSize < 0)
		throw std::exception(std::format("Exception occured while receiving data.\n\t{}", m_conn->last_error_str()).c_str());
	else if (bytesSize == 0)
		throw std::exception("Exception occured while receiving data. No bytes were received.");

	size_t msgSize = atoi(reinterpret_cast<const char*>(msgLength));

	replyLength = msgSize;

	// read the real message
	std::unique_ptr<uint8_t[]> msg = std::make_unique<uint8_t[]>(msgSize);

	bytesSize = this->m_conn->read_n(msg.get(), msgSize);
	if (bytesSize < 0)
		throw std::exception(std::format("Exception occured while receiving data.\n\t{}", m_conn->last_error_str()).c_str());
	else if (bytesSize == 0)
		throw std::exception("Exception occured while receiving data. No bytes were received.");
	else if (bytesSize != msgSize)
		throw std::exception("Exception occured while receiving data. Invalid number of bytes were written.");

	return std::move(msg);
}

std::string interrogator::parse_str_reply(std::string reply)
{
	// device returns the command name plus one whitespace, and after that real data follows
	return reply.substr(reply.find(' ') + 1);
}
