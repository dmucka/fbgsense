/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <cstdint>

constexpr uint16_t sm125_samples = 16001;
constexpr uint16_t sm125_port = 50000;

constexpr const char* SM125_IDN = "#IDN?";
constexpr const char* SM125_GET_PRODUCT_SN = "#GET_PRODUCT_SN";
constexpr const char* SM125_GET_DATA = "#GET_DATA";
constexpr const char* SM125_GET_SCAN_SPEED = "#GET_SCAN_SPEED";
constexpr const char* SM125_GET_SYSTEM_IMAGE_ID = "#GET_SYSTEM_IMAGE_ID";
constexpr const char* SM125_GET_NUM_DAQ_DUTS = "#GET_NUM_DAQ_DUTS"; // undocumented
constexpr const char* SM125_GET_MIN_WVL_USER_DATA = "#GET_MIN_WVL_USER_DATA"; // undocumented
constexpr const char* SM125_GET_MAX_WVL_USER_DATA = "#GET_MAX_WVL_USER_DATA"; // undocumented

constexpr const char* SM125_SET_NORMALIZE_DUTS = "#SET_NORMALIZE_DUTS"; // undocumented
constexpr const char* SM125_SET_DUT_STATE = "#SET_DUT{}_STATE {}"; // channel, [1,0]

// sm125 uses legacy naming scheme in format 1.1, 1.2, 1.3, 1.4, 2.1, 2.2, 2.3, 2.4 etc.
constexpr size_t sm125_channel_map[16] = {
	1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16
};

constexpr uint8_t sm125_header_size = 20;
