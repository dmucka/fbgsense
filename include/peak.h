/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <cstdint>
#include <vector>

#include "spectrum_types.h"

/**
 * @brief Data structure holding information about peak's properties.
*/
class peak
{
	size_t m_position{ 0 };
	size_t m_index{ 0 };
	uint8_t m_channel{ 0 };
	spectrum_unit_t m_wavelength{ 0 };
	spectrum_unit_t m_intensity{ 0 };
	spectrum_unit_t m_asymmetry{ 0 };
	bounds_t m_bounds;
public:
	peak(size_t position, size_t index, spectrum_unit_t wavelength, spectrum_unit_t intensity);
	peak(size_t position, size_t index, uint8_t channel, spectrum_unit_t wavelength, spectrum_unit_t intensity);
	peak(size_t position, size_t index, uint8_t channel, spectrum_unit_t wavelength, spectrum_unit_t intensity, spectrum_unit_t asymmetry);

	uint8_t channel();
	size_t position();
	size_t index();
	spectrum_unit_t wavelength();
	spectrum_unit_t intensity();
	spectrum_unit_t asymmetry();
	bounds_t bounds();

	void index(uint8_t index);
	void bounds(bounds_t bounds);
};

struct region_t
{
	int left{ -1 };
	int right{ -1 };
	std::vector<peak> peaks;
};

