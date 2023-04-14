/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <string>
#include <chrono>
#include <mutex>

#include <sockpp/tcp_connector.h>
#include <fftw3.h>

#include "spectrum/spectrum_sm125.h"

enum class interrogator_t
{
	sm125,
	hyperion
};

constexpr std::string convert_legacy_channel(uint8_t channel)
{
	switch (channel)
	{
	case 1: return "1.1";
	case 2: return "2.1";
	case 3: return "3.1";
	case 4: return "4.1";

	case 5: return "1.2";
	case 6: return "2.2";
	case 7: return "3.2";
	case 8: return "4.2";

	case 9: return "1.3";
	case 10: return "2.3";
	case 11: return "3.3";
	case 12: return "4.3";

	case 13: return "1.4";
	case 14: return "2.4";
	case 15: return "3.4";
	case 16: return "4.4";
	}

	throw std::exception("Invalid channel value.");
}

/**
 * @brief Data structure providing high-level access to the interrogator device.
*/
class interrogator
{
	sockpp::socket_initializer m_sockinit; // needed for proper init and cleanup of socket objects
	sockpp::tcp_connector* m_conn;
	std::mutex m_socket_mutex;
	std::string m_address;
	uint16_t m_port;

	std::string m_serial_number;
	std::string m_idn;
	std::string m_system_image_id;
	std::string m_num_daq_duts;
	std::string m_scan_speed;
	std::string m_min_wvl;
	std::string m_max_wvl;

	spectrum_sm125 m_spectrums;

	void recv_min_max_wvl();
	void recv_scan_speed();
	void recv_num_daq_duts();
	void recv_serial_number();
	void recv_system_image_id();

	std::string parse_str_reply(std::string reply);
public:
	interrogator(std::string address, uint16_t port);
	interrogator(interrogator&& other) noexcept;
	~interrogator();

	bool ping();
	void recv_spectrums();
	std::unique_ptr<uint8_t[]> recv_spectrums_data(size_t& dataLength);
	void set_channels(uint8_t count);

	std::string min_wvl();
	std::string max_wvl();
	std::string scan_speed();
	std::string num_daq_duts();
	std::string system_image_id();
	std::string idn();
	std::string serial_number();

	spectrum_sm125* spectrums();

	std::unique_ptr<uint8_t[]> send_command(std::string cmd);
	std::unique_ptr<uint8_t[]> send_command(std::string cmd, size_t& replyLength);
};
