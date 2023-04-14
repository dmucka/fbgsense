/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include "detection_method.h"

namespace fbgsense::detection
{
	class detection_method_localmax : public detection_method
	{
		constexpr int get_type() override
		{
			return get_hash("LOCAL_MAXIMA");
		}

		std::unique_ptr<detection_method> create() override
		{
			return std::make_unique<detection_method_localmax>();
		}

		void perform(spectrum_state spect, std::initializer_list<parameter_t> params) override;
	};
}
