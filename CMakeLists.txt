# Copyright (c) 2020 - present, Lars Johannsmeier
# All rights reserved.
# contact: lars.johannsmeier@tum.de

cmake_minimum_required(VERSION 3.16...3.20)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 3)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(msrm_cpp_utils VERSION ${PROJECT_VERSION}
    DESCRIPTION "multi-purpose function library"
    LANGUAGES CXX
    )


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE
        Release
        CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui, ccmake
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
        "Debug"
        "Release"
        "MinSizeRel"
        "RelWithDebInfo")
endif()

include(cmake/ApplyModuleOptions.cmake)
include(cmake/Conan.cmake)
include(cmake/Doxygen.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/StaticAnalyzers.cmake)

configure_file (
    "${PROJECT_SOURCE_DIR}/include/commons/config.hpp.in"
    "${PROJECT_BINARY_DIR}/include/commons/config.hpp"
    )
include_directories("${PROJECT_BINARY_DIR}/src/include")

option(ONLY_COVERAGE "Build only tests necessary for coverage" ON)
option(LIBCPP "Build with libc++" ON)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" ON)
option(ENABLE_ASAN "Enable address sanitizer" ON)
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable the building of the test" ON)
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)
option(SIMPLE_BUILD "Build the project as minimally as possible" ON)
option(BUILD_DOC "Build the project's documentation" OFF)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)
option(DEBUG_LOGGING "Enabling debug logging" ON)
option(ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" ON)
option(ENABLE_PCH "Enable Precompiled Headers" ON)
option(ENABLE_UNITY_BUILD "Enable unity build" ON)

add_library(project_warnings INTERFACE)
add_library(project_options INTERFACE)

target_compile_features(project_options
    INTERFACE
    cxx_std_17)

if(ENABLE_PCH)
    target_precompile_headers(
        project_options
        INTERFACE
        ${CMAKE_SOURCE_DIR}/include/commons/pch.hpp
        )
endif()

target_compile_options(project_warnings
    INTERFACE
    #    -Wall
    #    -Wextra
    #    -Wshadow
    #    -Wnon-virtual-dtor
    #    -Wold-style-cast
    #    -Wcast-align
    #    -Wunused
    #    -Woverloaded-virtual
    #    -Wpedantic
    #    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    #    -Wdouble-promotion
    #    -Wformat=2
    -Wreturn-type
    -Wuninitialized
    )

if(ONLY_COVERAGE OR ENABLE_COVERAGE)
    target_compile_options(project_options INTERFACE --coverage -O0 -g)
    target_link_libraries(project_options INTERFACE --coverage)
endif()

if(ENABLE_ASAN)
    target_compile_options(project_options INTERFACE -fsanitize=address)
    target_link_libraries(project_options INTERFACE -fsanitize=address)
endif()

# some GCC specific warnings. These flags are added only if the used compiler is GCC.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(project_warnings
        INTERFACE
        #        -Wmisleading-indentation
        #        -Wduplicated-cond
        #        -Wlogical-op
        #        -Wuseless-cast
        )
    target_link_libraries(project_options INTERFACE stdc++fs)
endif()

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (project_options INTERFACE -fcolor-diagnostics)
    endif ()
endif ()

find_program(CCACHE ccache)
if(CCACHE)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CUDA_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()



find_program(FPING fping)
if(NOT FPING)
    message(FATAL_ERROR "fping not found!")
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/_deps)
include(cmake/extern.cmake)

add_subdirectory(src)

# ---------------------------------------------------------------------------------------
# Set MSRM_UTILS_MAIN_PROJECT to ON if we are building robotp2p
# ---------------------------------------------------------------------------------------
# Check if msrm_utils is being used directly or via add_subdirectory, but allow overriding
#if(NOT DEFINED MSRM_CPP_UTILS_MAIN_PROJECT)
#    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
#        set(MSRM_CPP_UTILS_MAIN_PROJECT ON)
#    else()
#        set(MSRM_CPP_UTILS_MAIN_PROJECT OFF)
#    endif()
#endif()

#option(MSRM_CPP_UTILS_INSTALL "Generate the install target" ${MSRM_CPP_UTILS_MAIN_PROJECT})
## set the verbosity level for logging/printing
#add_definitions(-Dverbose_level=1)

# Set global flags only if this is the main project
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#    set(CMAKE_CXX_FLAGS_DEBUG "-g")
#    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#    set(CMAKE_CXX_STANDARD 17)
#    set(CMAKE_CXX_STANDARD_REQUIRED ON)
#    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#    set(CMAKE_DEBUG_POSTFIX _dbg)
#    set(CMAKE_EXPORT_COMPILE_COMMANDS On)
#    set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/../extern/lib/cmake)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type -Werror=uninitialized -Wno-error=deprecated-declarations -Wno-error=unused-result")

#    set(CMAKE_CXX_EXTENSIONS OFF)
#    set(CMAKE_VERBOSE_MAKEFILE ON)

#    add_compile_options(-std=c++17)
#    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#        add_compile_options(-Wthread-safety)
#    endif()


#endif()





#if(MSRM_CPP_UTILS_INSTALL2)
#    message(STATUS "Generating install")
#    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/msrm_cpp_utilsConfig.cmake.in")
#    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/msrm_cpp_utilsConfig.cmake")
#    set(config_targets_file "msrm_cpp_utilsConfigTargets.cmake")
#    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/msrm_cpp_utilsConfigVersion.cmake")
#    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/msrm_cpp_utils")
#    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
#    set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")
#    message(STATUS "pkg_config=${pkg_config}")

#    # ---------------------------------------------------------------------------------------
#    # Install targets
#    # ---------------------------------------------------------------------------------------

#    install(TARGETS ${CMAKE_PROJECT_NAME}
#        EXPORT ${CMAKE_PROJECT_NAME}-targets
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
#        )



#    # ---------------------------------------------------------------------------------------
#    # Install pkg-config file
#    # ---------------------------------------------------------------------------------------
#    configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
#    install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

#    # ---------------------------------------------------------------------------------------
#    # Install CMake config files
#    # ---------------------------------------------------------------------------------------
#    install(EXPORT ${CMAKE_PROJECT_NAME}-targets
#        FILE ${config_targets_file}
#        DESTINATION ${export_dest_dir}
#        NAMESPACE ${CMAKE_PROJECT_NAME}::)

#    include(CMakePackageConfigHelpers)
#    configure_file("${project_config_in}" "${project_config_out}" @ONLY)

#    write_basic_package_version_file("${version_config_file}" COMPATIBILITY SameMajorVersion)
#    install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

#    # ---------------------------------------------------------------------------------------
#    # Support creation of installable packages
#    # ---------------------------------------------------------------------------------------
#    include(cmake/msrm_cpp_utilsCPack.cmake)
#endif()


message("#########################################")
    message("\t \t Summary")
    message("#########################################")
        message("Build type:       \t ${CMAKE_BUILD_TYPE}")
        message("Install prefix:   \t ${CMAKE_INSTALL_PREFIX}")
        message("Testing enabled:  \t ${ENABLE_TESTING}")
        message("Clang-tidy:       \t ${ENABLE_CLANG_TIDY}")
        message("Cppcheck:         \t ${ENABLE_CPPCHECK}")
        message("Compiler:         \t ${CMAKE_CXX_COMPILER_ID}")
        message("Sanitizers:       \t ${ENABLE_ASAN}")
        message("Shared libs:      \t ${BUILD_SHARED_LIBS}")
        message("Build libcpp:     \t ${LIBCPP}")
        message("CCache executable:\t ${CCACHE}")
        message("PCH:              \t ${ENABLE_PCH}")
        message("IPO:              \t ${ENABLE_IPO}")
        message("Unity build:      \t ${ENABLE_UNITY_BUILD}")
        message("Building doc:     \t ${BUILD_DOC}")
        message("------------------------------------------")
        message("Version:          \t ${PROJECT_VERSION}")
        message("########################################")
