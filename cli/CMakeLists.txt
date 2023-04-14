# 
# This file is a part of the project "fbgsense", which is released under MIT license.
# Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
# See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
# 

add_executable(fbgsense-cli fbgsense-cli.cpp fbgsense-cli.h)

# Include cxxopts
find_package(cxxopts CONFIG REQUIRED)
target_link_libraries(fbgsense-cli PRIVATE cxxopts::cxxopts)

# Include cpp-terminal
target_include_directories(fbgsense-cli PRIVATE cpp-terminal)
target_link_libraries(fbgsense-cli PRIVATE cpp-terminal)

# Include fbgsense library
target_include_directories(fbgsense-cli PRIVATE fbgsense)
target_link_libraries(fbgsense-cli PRIVATE fbgsense)