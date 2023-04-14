/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include "segmentation_method.h"

namespace fbgsense::segmentation
{
	class segmentation_method_localmin : public segmentation_method
	{
	public:
		constexpr int get_type() override
		{
			return get_hash("LOCAL_MINIMA");
		}

		std::unique_ptr<segmentation_method> create() override
		{
			return std::make_unique<segmentation_method_localmin>();
		}

		std::vector<region_t> perform(spectrum_state spect, std::initializer_list<parameter_t> params) override;
	};
}
