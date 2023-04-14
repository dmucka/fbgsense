/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múčka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <iostream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>

#include <cxxopts.hpp>

#include <spectrum/fitting.h>
#include <interrogator/constants/sm125.h>
#include <interrogator.h>

#include <spectrum/spectrum_file.h>

#include <cpp-terminal/base.hpp>
