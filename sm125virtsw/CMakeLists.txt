# 
# This file is a part of the project "fbgsense", which is released under MIT license.
# Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
# See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
# 

add_executable(sm125virtsw sm125virtsw.cpp)

# Include sockpp
if(WIN32)
  target_link_libraries(sm125virtsw PRIVATE wsock32 ws2_32)
endif()

find_package(sockpp CONFIG REQUIRED)
target_include_directories(sm125virtsw PRIVATE ${SOCKPP_INCLUDE_DIRS})
target_link_libraries(sm125virtsw PRIVATE ${SOCKPP_LIBRARIES})

# Include cxxopts
find_package(cxxopts CONFIG REQUIRED)
target_link_libraries(sm125virtsw PRIVATE cxxopts::cxxopts)

# Include fbgsense library
target_include_directories(sm125virtsw PRIVATE fbgsense)
target_link_libraries(sm125virtsw PRIVATE fbgsense)
