/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include "../processor.h"
#include "../spectrum_types.h"
#include "../spectrum/spectrum_state.h"

namespace fbgsense::filtering
{
	class filter : public processor<filter>
	{
	public:
		virtual void perform(spectrum_state spect, std::initializer_list<parameter_t> params) = 0;

		virtual ~filter() = default;
	};
}
