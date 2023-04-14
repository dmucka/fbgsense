/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múčka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "fbgsense-cli.h"

#define RETCODE_OK 0
#define RETCODE_FAIL 1

using namespace std;
namespace fs = std::filesystem;

void PrintError(string msg)
{
	cerr << "[Error] " << msg << endl;
}

int main(int argc, char* argv[])
{
	try
	{
		// setup cmdline options using cxxoptions library
		cxxopts::Options options("fbgsense", "Generic FBG spectrum analysis tool");

		options.add_options()
			("h,help", "Print usage.")
			;

		options.add_options("DETECTION")
			("method", "Filtering method. Choices are: ft, sg", cxxopts::value<std::string>())
			("segment", "Segmentation method. Choices are: lm, in", cxxopts::value<std::string>())
			("fit", "Fitting method. Choices are: g, big, l", cxxopts::value<std::string>())
			("force", "Force fit all peaks.", cxxopts::value<bool>())
			("equalize", "Perform undersampling process.", cxxopts::value<bool>())
			("r", "Thresholding parameter.", cxxopts::value<int>()->default_value("5"))
			("t", "Filtering parameter.", cxxopts::value<int>()->default_value("10"))
			;

		options.add_options("LIVE MODE")
			("a,address", "Device interface IP address.", cxxopts::value<std::string>())
			("l,live", "Poll data from device continuously.", cxxopts::value<bool>())
			("c,channels", "Set active channels. [1-16]", cxxopts::value<int>())
			;

		options.add_options("FILE MODE")
			("f,file", "Path to the spectrum file.", cxxopts::value<std::string>())
			;

		auto result = options.parse(argc, argv);

		// show help
		if (result.count("help") || !result.count("file") && !result.count("address"))
		{
			std::cout << options.help() << std::endl;
			return RETCODE_FAIL;
		}

		std::string filteringType = result["method"].as<std::string>();
		filter_type selectedFilter;

		if (filteringType == "ft")
			selectedFilter = filter_type::FFT;
		else if (filteringType == "sg")
			selectedFilter = filter_type::SAVGOL;
		else
		{
			PrintError("Invalid filtering method.");
			return RETCODE_FAIL;
		}

		std::string segmentationType = result["segment"].as<std::string>();
		segment_type selectedSegmentation;

		if (segmentationType == "lm")
			selectedSegmentation = segment_type::LOCAL_MINIMA;
		else if (segmentationType == "in")
			selectedSegmentation = segment_type::INFLECTION_POINTS;
		else
		{
			PrintError("Invalid segmentation method.");
			return RETCODE_FAIL;
		}

		parameter_t relativeThreshold = result["r"].as<int>();
		parameter_t filteringParameter = result["t"].as<int>();

		fit_settings_t fitSettings;
		fitSettings.enable = result.count("fit");

		if (fitSettings.enable)
		{
			std::string fitType = result["fit"].as<std::string>();

			if (fitType == "big")
				fitSettings.fitType = fit_type::BIGAUSSIAN;
			else if (fitType == "g")
				fitSettings.fitType = fit_type::GAUSSIAN;
			else if (fitType == "l")
				fitSettings.fitType = fit_type::LINEAR;
			else
			{
				PrintError("Invalid fitting method.");
				return RETCODE_FAIL;
			}

			fitSettings.force = result["force"].as<bool>();
			fitSettings.equalize = result["equalize"].as<bool>();
		}
		
		std::string delimiter = Term::is_stdout_a_tty() ? "     " : "\t";

		// file mode, parse spectrum from file
		if (result.count("file"))
		{
			fs::path spectrumFilePath(result["file"].as<std::string>());
			if (!fs::exists(spectrumFilePath) || fs::is_directory(spectrumFilePath))
			{
				PrintError("Specified file does not exist.");
				return RETCODE_FAIL;
			}

			spectrum_file spectrum(sm125_samples, sm125_samples - 1);
			spectrum.set_filter(selectedFilter);
			spectrum.set_segmentation_method(selectedSegmentation);

			ostringstream s;

			auto begin_total = std::chrono::high_resolution_clock::now();
			spectrum.load(spectrumFilePath);
			auto end_load = std::chrono::high_resolution_clock::now();
			spectrum.perform(1, { filteringParameter }, { relativeThreshold }, { relativeThreshold }, fitSettings);
			auto end_perform = std::chrono::high_resolution_clock::now();
			spectrum.print(s, delimiter);
			spectrum.clean();
			auto end_total = std::chrono::high_resolution_clock::now();

			auto elapsedTotal = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - begin_total);
			auto elapsedLoad = std::chrono::duration_cast<std::chrono::milliseconds>(end_load - begin_total);
			auto elapsedPerform = std::chrono::duration_cast<std::chrono::milliseconds>(end_perform - end_load);

			s << "Load   :" << delimiter << std::right << std::setw(6) << elapsedLoad << endl;
			s << "Perform:" << delimiter << std::right << std::setw(6) << elapsedPerform << endl;
			s << "Total  :" << delimiter << std::right << std::setw(6) << elapsedTotal << endl;

			cout << s.str();
		}
		else if (result.count("address")) // live mode, parse spectrums from device
		{
			interrogator interrogator(result["address"].as<std::string>(), sm125_port);

			if (result.count("channels"))
			{
				interrogator.set_channels(result["channels"].as<int>());
			}

			bool live = result["live"].as<bool>();

			Term::Terminal term(live && Term::is_stdout_a_tty(), false, false);
			Term::save_screen();

			do
			{
				ostringstream s;

				if (live && Term::is_stdout_a_tty())
					s << Term::cursor_off() << Term::move_cursor(1, 1);

				s << "Serial Number:" << delimiter << interrogator.serial_number() << endl;

				auto begin_total = std::chrono::high_resolution_clock::now();
				interrogator.recv_spectrums();
				spectrum_sm125* spectrum = interrogator.spectrums();
				spectrum->set_filter(selectedFilter);
				spectrum->set_segmentation_method(selectedSegmentation);
				auto end_network = std::chrono::high_resolution_clock::now();
				spectrum->perform({ filteringParameter }, { relativeThreshold }, { relativeThreshold }, fitSettings);
				auto end_perform = std::chrono::high_resolution_clock::now();
				spectrum->print(s, delimiter);
				spectrum->clean();
				auto end_total = std::chrono::high_resolution_clock::now();

				auto elapsedTotal   = std::chrono::duration_cast<std::chrono::milliseconds>(end_total - begin_total);
				auto elapsedNetwork = std::chrono::duration_cast<std::chrono::milliseconds>(end_network - begin_total);
				auto elapsedPerform = std::chrono::duration_cast<std::chrono::milliseconds>(end_perform - end_network);

				if (live && Term::is_stdout_a_tty())
				{
					int count = 0;
					int longest = 0;

					// clear the rest of the terminal in case there is different number of peaks
					istringstream iss(s.str());
					std::string line;
					while (getline(iss, line))
					{
						++count;
						if (line.size() > longest)
							longest = line.size();
					}

					int rows, cols;
					Term::get_term_size(rows, cols);
					rows = rows - count - 4;
					
					std::string filler(longest, ' ');
					
					for (int i = 0; i < rows; i++)
					{
						s << filler << endl;
					}
				}

				s << "Network:" << delimiter << std::right << std::setw(6) << elapsedNetwork << endl;
				s << "Perform:" << delimiter << std::right << std::setw(6) << elapsedPerform << endl;
				s << "Total  :" << delimiter << std::right << std::setw(6) << elapsedTotal   << endl;

				if (live && Term::is_stdout_a_tty())
				{
					s << Term::cursor_on();
				}

				Term::write(s.str());
			} while (live);
		}
	}
	catch (cxxopts::OptionException& ex)
	{
		PrintError(ex.what());
		return RETCODE_FAIL;
	}
	catch (exception& ex)
	{
		PrintError(std::format("Exception occured. {0}", ex.what()));
#ifndef NDEBUG
		system("pause");
#endif
		return RETCODE_FAIL;
	}


#ifndef NDEBUG
	system("pause");
#endif

	return RETCODE_OK;
}
