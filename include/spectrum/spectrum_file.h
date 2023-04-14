/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <exception>

#include "spectrum/spectrum.h"

namespace fs = std::filesystem;

/**
 * @brief Spectrum of csv file.
*/
class spectrum_file : public spectrum
{
public:
	spectrum_file();
	spectrum_file(size_t storage, size_t samples);
	spectrum_file(size_t samples);

	void load(fs::path filePath);
};
