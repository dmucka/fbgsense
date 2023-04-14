/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <iomanip>

/// <summary>
/// Forces decimal comma for numbers instead of a dot.
/// </summary>
struct decimal_comma : std::numpunct<char> {
	char_type do_decimal_point() const override { return ','; }
};
