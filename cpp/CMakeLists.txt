# Copyright (c) 2020 - present, Lars Johannsmeier
# All rights reserved.
# contact: lars.johannsmeier@tum.de

cmake_minimum_required(VERSION 3.16.0)

project(msrm_cpp_utils VERSION ${MSRM_UTILS_VERSION}
                 DESCRIPTION "multi-purpose function library"
                 LANGUAGES CXX)

include(${CMAKE_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

include(cmake/utils.cmake)
get_msrm_utils_version()



option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

# ---------------------------------------------------------------------------------------
# Set MSRM_UTILS_MAIN_PROJECT to ON if we are building robotp2p
# ---------------------------------------------------------------------------------------
# Check if msrm_utils is being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED MSRM_UTILS_MAIN_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(MSRM_UTILS_MAIN_PROJECT ON)
    else()
        set(MSRM_UTILS_MAIN_PROJECT OFF)
    endif()
endif()

#option(MSRM_UTILS_INSTALL "Generate the install target" ${MSRM_UTILS_MAIN_PROJECT})
option(MSRM_UTILS_INSTALL "Generate the install target" OFF)

# set the verbosity level for logging/printing
add_definitions(-Dverbose_level=1)

# Set global flags only if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_DEBUG_POSTFIX _dbg)
    set(CMAKE_EXPORT_COMPILE_COMMANDS On)
    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/../extern/lib/cmake)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type -Werror=uninitialized -Wno-error=deprecated-declarations -Wno-error=unused-result")

    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_VERBOSE_MAKEFILE ON)

    add_compile_options(-std=c++17)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wthread-safety)
    endif()


endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

find_program(FPING fping)
if(NOT FPING)
    message(FATAL_ERROR "fping not found!")
endif()

option(BUILD_TESTING "Build tests" OFF)
option(USE_PCH "Use precompiled headers" OFF)
option(STATIC_LIB "Compile a static library" ON)
enable_testing()

set(SOURCES src/conversion.cpp src/files.cpp src/json.cpp src/math.cpp src/network.cpp src/output.cpp src/system.cpp src/benchmarking.cpp src/geometry.cpp)
set(HEADER include/msrm_utils/conversion.hpp include/msrm_utils/files.hpp include/msrm_utils/json.hpp include/msrm_utils/math.hpp
    include/msrm_utils/network.hpp include/msrm_utils/output.hpp include/msrm_utils/system.hpp include/msrm_utils/benchmarking.hpp include/msrm_utils/geometry.hpp)

if(STATIC_LIB)
    add_library(msrm_cpp_utils STATIC ${SOURCES})
else()
    add_library(msrm_cpp_utils SHARED ${SOURCES})
endif()

target_include_directories(msrm_cpp_utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(msrm_cpp_utils PRIVATE ssl z)


set_target_properties(msrm_cpp_utils PROPERTIES UNITY_BUILD ON)

# Testing only available if this is the main app AND BUILD_TESTING flag set
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(USE_PCH)
  target_precompile_headers(msrm_cpp_utils PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/pch.hpp)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
    set_target_properties(msrm_cpp_utils PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
