/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#include "spectrum/spectrum_file.h"

spectrum_file::spectrum_file() : spectrum()
{
}

spectrum_file::spectrum_file(size_t storage, size_t samples) : spectrum(storage, samples)
{
}

spectrum_file::spectrum_file(size_t samples) : spectrum(samples)
{
}

void spectrum_file::load(fs::path filePath)
{
	auto file = std::ifstream(filePath);

	char delim;
	double wavelength, intensity;
	size_t i = 0;

	while (i < m_samples && file >> wavelength >> delim >> intensity && delim == ';')
	{
		m_wavelengths[i] = wavelength;
		m_intensities[0][i] = intensity;
		i++;
	}

	if (i < m_samples)
		throw std::exception("The file contains less valid samples than required.");
}
