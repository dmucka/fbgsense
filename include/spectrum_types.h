/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <cstdint>

#define spectrum_unit_t float

typedef uint8_t channel_t;
typedef uint16_t parameter_t;
typedef spectrum_unit_t complex_t[2];

struct spectrum_point_t
{
	spectrum_unit_t wavelength;
	spectrum_unit_t intensity;
};

struct minmax_t
{
	spectrum_unit_t min;
	spectrum_unit_t max;
};

enum class extrema_type
{
	NONE,     // no sign change
	MINIMUM,  // sign change from - to +
	MAXIMUM   // sign change from + to -
};

enum class fit_type
{
	BIGAUSSIAN,
	GAUSSIAN,
	LINEAR,
};

enum class filter_type
{
	FFT,
	SAVGOL
};

enum class detection_type
{
	LOCAL_MAXIMA
};

enum class segment_type
{
	LOCAL_MINIMA,
	INFLECTION_POINTS
};

struct derivation_t
{
	spectrum_unit_t value{ 0 };
	extrema_type extrema{ extrema_type::NONE };
};

struct fit_settings_t
{
	bool enable{ false };
	bool force{ false };
	fit_type fitType{ fit_type::GAUSSIAN };
	bool equalize{ false };
};

struct point_t
{
	spectrum_unit_t x;
	spectrum_unit_t y;
};

struct bounds_t
{
	bool valid{ false };
	point_t bottomleft;
	point_t bottomright;
	point_t topleft;
	point_t topright;
};
