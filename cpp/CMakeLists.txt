# Copyright (c) 2020 - present, Lars Johannsmeier
# All rights reserved.
# contact: lars.johannsmeier@tum.de

cmake_minimum_required(VERSION 3.16.0)

include(cmake/utils.cmake)
get_msrm_utils_version()

project(msrm_cpp_utils VERSION ${MSRM_UTILS_VERSION}
                 DESCRIPTION "multi-purpose function library"
                 LANGUAGES CXX)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
get_filename_component(extern "../extern"
                       REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/extern)
message(${FETCHCONTENT_BASE_DIR})

include(cmake/extern.txt)

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

option(MSRM_UTILS_INSTALL "Generate the install target" ${MSRM_UTILS_MAIN_PROJECT})

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

    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_VERBOSE_MAKEFILE ON)

    add_compile_options(-std=c++17)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
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

target_include_directories(msrm_cpp_utils PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(msrm_cpp_utils PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}

    # NOTE on why the build interface: https://gitlab.kitware.com/cmake/cmake/-/issues/15415#note_333595
    # we only need the dependencies during the build, not in API
    "$<BUILD_INTERFACE:simple-websocket-server>"
    # NOTE we must use spdlog_header_only to integrate it into the static lib
    "$<BUILD_INTERFACE:nlohmann_json::nlohmann_json>"
    "ssl"
    "z"
)

set_target_properties(msrm_cpp_utils PROPERTIES PUBLIC_HEADER "${HEADER}")
set_target_properties(msrm_cpp_utils PROPERTIES UNITY_BUILD ON)

# Testing only available if this is the main app AND BUILD_TESTING flag set
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR ROBOTP2P_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(USE_PCH)
  target_precompile_headers(msrm_cpp_utils PRIVATE ${CMAKE_SOURCE_DIR}/include/commons/pch.hpp)
endif()

#install(TARGETS msrm_cpp_utils CONFIGURATIONS Release DESTINATION ${CMAKE_INSTALL_PREFIX}/lib PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/msrm_utils)
#install(TARGETS msrm_cpp_utils CONFIGURATIONS Debug DESTINATION ${CMAKE_INSTALL_PREFIX}/lib PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/msrm_utils)

if(MSRM_UTILS_INSTALL)
    message(STATUS "Generating install")
    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/msrm_cpp_utilsConfig.cmake.in")
    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/msrm_cpp_utilsConfig.cmake")
    set(config_targets_file "msrm_cpp_utilsConfigTargets.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/msrm_cpp_utilsConfigVersion.cmake")
    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/msrm_cpp_utils")
    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")
    message(STATUS "pkg_config=${pkg_config}")

    # ---------------------------------------------------------------------------------------
    # Install targets
    # ---------------------------------------------------------------------------------------
    install(TARGETS msrm_cpp_utils
            EXPORT msrm_cpp_utils-targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/msrm_cpp_utils
    )

    # ---------------------------------------------------------------------------------------
    # Install pkg-config file
    # ---------------------------------------------------------------------------------------
    configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
    install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

    # ---------------------------------------------------------------------------------------
    # Install CMake config files
    # ---------------------------------------------------------------------------------------
    install(EXPORT msrm_cpp_utils-targets
            FILE ${config_targets_file}
            DESTINATION ${export_dest_dir}
            NAMESPACE msrm_cpp_utils::)

    include(CMakePackageConfigHelpers)
    configure_file("${project_config_in}" "${project_config_out}" @ONLY)

    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
    install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

    # ---------------------------------------------------------------------------------------
    # Support creation of installable packages
    # ---------------------------------------------------------------------------------------
    include(cmake/msrm_cpp_utilsCPack.cmake)
endif()
