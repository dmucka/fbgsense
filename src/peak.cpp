/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "peak.h"

peak::peak(size_t position, size_t index, spectrum_unit_t wavelength, spectrum_unit_t intensity)
	: m_position(position), m_index(index), m_wavelength(wavelength), m_intensity(intensity), m_channel(1)
{
}

peak::peak(size_t position, size_t index, uint8_t channel, spectrum_unit_t wavelength, spectrum_unit_t intensity)
	: m_position(position), m_index(index), m_wavelength(wavelength), m_intensity(intensity), m_channel(channel)
{
}

peak::peak(size_t position, size_t index, uint8_t channel, spectrum_unit_t wavelength, spectrum_unit_t intensity, spectrum_unit_t asymmetry)
	: m_position(position), m_index(index), m_wavelength(wavelength), m_intensity(intensity), m_channel(channel), m_asymmetry(asymmetry)
{
}

size_t peak::index()
{
	return m_index;
}

void peak::index(uint8_t index)
{
	m_index = index;
}

spectrum_unit_t peak::wavelength()
{
	return m_wavelength;
}

spectrum_unit_t peak::intensity()
{
	return m_intensity;
}

uint8_t peak::channel()
{
	return m_channel;
}

size_t peak::position()
{
	return m_position;
}

spectrum_unit_t peak::asymmetry()
{
	return m_asymmetry;
}

bounds_t peak::bounds()
{
	return m_bounds;
}

void peak::bounds(bounds_t bounds)
{
	m_bounds = bounds;
}
