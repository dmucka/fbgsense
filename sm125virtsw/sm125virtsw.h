/*
 * This file is a part of the project "fbgsense", which is released under MIT license.
 * Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
 * See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
 */

#pragma once

#include <interrogator/constants/sm125.h>
#include <interrogator.h>

#include <thread>
#include <sstream>
#include <chrono>
#include <format>

#include <cxxopts.hpp>
#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>

