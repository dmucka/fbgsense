# 
# This file is a part of the project "fbgsense", which is released under MIT license.
# Copyright (c) 2022 Daniel Múèka, Masaryk University, Faculty of Informatics
# See file LICENSE or visit https://opensource.org/licenses/MIT for full license details.
# 

file(GLOB_RECURSE HEADERS_LIST CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE SOURCES_LIST CONFIGURE_DEPENDS "*.cpp")

set(SRC ${SOURCES_LIST} ${HEADERS_LIST} fbgsense-gui.rc)

if(WIN32)
	add_executable(fbgsense-gui WIN32 ${SRC})
else()
	add_executable(fbgsense-gui ${SRC})
endif()

# Include wxWidgets
# disable default manifest linking (https://github.com/KDAB/GammaRay/issues/458)
set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
# include wxWidgets manifest with DPI awareness enabled
add_compile_definitions( WX_CPU_AMD64=1 )
add_compile_definitions( wxUSE_RC_MANIFEST=1 )
add_compile_definitions( wxUSE_DPI_AWARE_MANIFEST=2 )
find_package(unofficial-wxwidgets CONFIG REQUIRED)
target_include_directories(fbgsense-gui PRIVATE wx::wxcore wx::wxgl)
target_link_libraries(fbgsense-gui PRIVATE wx::wxcore wx::wxgl)

# Include openGLPlotLive
target_include_directories(fbgsense-gui PRIVATE openGLPlotLive)
target_link_libraries(fbgsense-gui PRIVATE openGLPlotLive)

# Copy Shader files to build directory
add_custom_command(TARGET fbgsense-gui PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/openGLPlotLive/Shaders $<TARGET_FILE_DIR:fbgsense-gui>/Shaders)

# Copy Texture files to build directory
add_custom_command(TARGET fbgsense-gui PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/openGLPlotLive/textures $<TARGET_FILE_DIR:fbgsense-gui>/textures)

# Include fbgsense library
target_include_directories(fbgsense-gui PRIVATE fbgsense)
target_link_libraries(fbgsense-gui PRIVATE fbgsense)
