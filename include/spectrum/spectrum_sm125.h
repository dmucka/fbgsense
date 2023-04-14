/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include "interrogator/constants/sm125.h"
#include "spectrum/spectrum.h"

// disable padding, so we can overlay this structure over raw tcp data.
#pragma pack(1)
struct sm125_spectrum_subheader_t
{
	uint32_t m_subHeaderSize;
	uint32_t m_minWavelength;
	uint32_t m_increment;
	uint32_t m_sampleCount;
	uint32_t m_index;
	int16_t  m_intensities[sm125_samples];
};

struct sm125_spectrum_header_t
{
	uint32_t m_headerSize;
	uint32_t m_protocolVersion;
	uint32_t m_channelCount;
	uint32_t m_reserved;
	uint32_t m_counter;
	sm125_spectrum_subheader_t m_channels[spectrum_max_channels];
};
#pragma pack()

// we are exploiting the fact that fft works best on factorizable sample counts.
// unfortunately, sm125 gives out a prime number of samples. thus, the easiest way to
// make the computation faster is to subtract one sample.
// this change makes the computation ~4x faster.

/**
 * @brief Spectrum of SM125 interrogator.
*/
class spectrum_sm125 : public spectrum
{
	bool m_initialized = false;
public:
	spectrum_sm125();

	void update(uint8_t* data);
};
