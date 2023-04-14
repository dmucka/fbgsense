/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <numbers>
#include <fftw3.h>

#include "../spectrum_types.h"
#include "filter.h"

namespace fbgsense::filtering
{
	class filter_fft : public filter
	{
	public:
		constexpr int get_type() override
		{
			return get_hash("FFT");
		}

		std::unique_ptr<filter> create() override
		{
			return std::make_unique<filter_fft>();
		}

		/// <summary>
		/// Filter the signal using Fourier transform.
		/// </summary>
		void perform(spectrum_state spect, std::initializer_list<parameter_t> params) override;
	};
}
