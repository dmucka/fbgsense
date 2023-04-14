# 
# This file is a part of the project "fbgsense", which is released under MIT license.
# Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
# See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
# 

file(GLOB_RECURSE HEADERS_LIST CONFIGURE_DEPENDS "../include/*.h")
file(GLOB_RECURSE SOURCES_LIST CONFIGURE_DEPENDS "*.cpp")

# https://decovar.dev/blog/2021/03/08/cmake-cpp-library/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(fbgsense SHARED ${SOURCES_LIST} ${HEADERS_LIST})

set_target_properties(fbgsense PROPERTIES VERSION ${PROJECT_VERSION})

# Add headers
target_include_directories(fbgsense PUBLIC ../include)

# Include fftw3
find_package(FFTW3f CONFIG REQUIRED)
target_link_libraries(fbgsense PRIVATE FFTW3::fftw3f)

# Include sockpp
if(WIN32)
  target_link_libraries(fbgsense PRIVATE wsock32 ws2_32)
endif()

find_package(sockpp CONFIG REQUIRED)
target_include_directories(fbgsense PRIVATE ${SOCKPP_INCLUDE_DIRS})
target_link_libraries(fbgsense PRIVATE ${SOCKPP_LIBRARIES})

# gsl-curve-fit
target_include_directories(fbgsense PRIVATE gsl-curve-fit)
target_link_libraries(fbgsense PRIVATE gsl-curve-fit)
