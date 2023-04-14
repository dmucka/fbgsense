/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <vector>
#include <span>

#include "../spectrum_types.h"
#include "../peak.h"

/**
 * @brief Data structure reflecting spectrum's current state. Used in processors.
*/
struct spectrum_state
{
	channel_t channel;
	size_t samples;
	spectrum_unit_t* wavelengths;
	spectrum_unit_t* intensities;
	spectrum_unit_t* filtered_intensities;
	std::span<complex_t>& frequencies;
	std::span<derivation_t>& derivation;
	minmax_t& extremas;
	std::vector<peak>& peaks;
};
