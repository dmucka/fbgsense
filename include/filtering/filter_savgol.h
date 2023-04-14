/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <span>

#include "../savgol.h"
#include "../spectrum_types.h"
#include "filter.h"

namespace fbgsense::filtering
{
	class filter_savgol : public filter
	{
	public:

		constexpr int get_type() override
		{
			return get_hash("SAVGOL");
		}

		/// <inheritdoc/>
		std::unique_ptr<filter> create() override
		{
			return std::make_unique<filter_savgol>();
		}

		/// <summary>
		/// Filter using the Savitzky-Golay filter with Gram polynome approximation technique.
		/// </summary>
		void perform(spectrum_state spect, std::initializer_list<parameter_t> params) override;
	};
}
