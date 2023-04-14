/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "spectrum/spectrum_sm125.h"

spectrum_sm125::spectrum_sm125() : spectrum(sm125_samples, sm125_samples - 1)
{
}

void spectrum_sm125::update(uint8_t* data)
{
	sm125_spectrum_header_t* header = reinterpret_cast<sm125_spectrum_header_t*>(data);

	// sanity check
	if (header[0].m_headerSize != sm125_header_size)
		return;

	if (!m_initialized)
	{
		double startingWavelength = header->m_channels[0].m_minWavelength / 10000.0;
		double increment = header->m_channels[0].m_increment / 10000.0;

		for (size_t i = 0; i < sm125_samples; i++)
			m_wavelengths[i] = startingWavelength + (i * increment);

		m_initialized = true;
	}


	for (size_t c = 0; c < header->m_channelCount; c++)
	{
		// sanity check
		if (header->m_channels[c].m_subHeaderSize != sm125_header_size)
			continue;

		size_t index = to_index(sm125_channel_map[to_index(header->m_channels[c].m_index)]);

		for (size_t i = 0; i < sm125_samples; i++)
			m_intensities[index][i] = header->m_channels[c].m_intensities[i] / 100.0;
	}
}
