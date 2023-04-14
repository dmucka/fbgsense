# 
# This file is a part of the project "fbgsense", which is released under MIT license.
# Copyright (c) 2022 Daniel Múčka, Masaryk University, Faculty of Informatics
# See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
# 

# CMakeList.txt : CMake project for fbgsense
# Structure inspired by https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html, 20.11.2021

cmake_minimum_required (VERSION 3.12)

set(VCPKG_TARGET_TRIPLET "x64-windows-static-md")

# Support for vcpkg
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

# Project name
project(
    fbgsense
    VERSION 0.1
    DESCRIPTION "Generic FBG spectrum analysis tool"
    LANGUAGES CXX C)

# Set C++20 https://gitlab.kitware.com/cmake/cmake/-/issues/22606
if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
    # target_compile_features(fbgsense PRIVATE cxx_std_23)
    # this change happened in CMake 3.20.4
    set(CMAKE_CXX_STANDARD 23) # /std:c++latest - unlocks the non stable cpp20 features. For new 16.11 versions
else ()
    # target_compile_features(fbgsense PRIVATE cxx_std_20)
    set(CMAKE_CXX_STANDARD 20) # /std:c++latest for msvc and -std=c++20 for everyone else.
endif ()

# cpp-terminal
option(CPPTERMINAL_BUILD_EXAMPLES "Set to ON to build examples" ON)
option(CPPTERMINAL_ENABLE_INSTALL "Set to ON to enable install" ON)
option(CPPTERMINAL_ENABLE_TESING "Set to ON to enable testing" ON)
set(CPPTERMINAL_BUILD_EXAMPLES OFF)
set(CPPTERMINAL_ENABLE_TESING OFF)
add_compile_definitions(NOMINMAX)
add_subdirectory(cpp-terminal)

# gsl-curve-fit
find_package(GSL REQUIRED)

add_library(gsl-curve-fit gsl-curve-fit/curve_fit.cpp gsl-curve-fit/curve_fit.hpp)
target_include_directories(gsl-curve-fit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gsl-curve-fit)
target_link_libraries(gsl-curve-fit PRIVATE GSL::gsl GSL::gslcblas)

# openGLPlotLive
add_subdirectory(openGLPlotLive)

# Copy dll files to exe location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project files
add_subdirectory(src)
add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(sm125virtsw)
