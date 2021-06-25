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
include(cmake/Doxygen.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/StaticAnalyzers.cmake)

if(NOT CONAN_INCLUDE_DIRS)
    include(cmake/Conan.cmake)
endif()

AssureOutOfSourceBuilds()

configure_file (
    "${PROJECT_SOURCE_DIR}/include/commons/config.hpp.in"
    "${PROJECT_BINARY_DIR}/include/commons/config.hpp"
    )

option(ONLY_COVERAGE "Build only tests necessary for coverage" OFF)
option(LIBCPP "Build with libc++" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" OFF)
option(ENABLE_ASAN "Enable address sanitizer" OFF)
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable the building of the test" ON)
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)
option(SIMPLE_BUILD "Build the project as minimally as possible" OFF)
option(BUILD_DOC "Build the project's documentation" OFF)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)
option(DEBUG_LOGGING "Enabling debug logging" ON)
option(ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" ON)
option(ENABLE_PCH "Enable Precompiled Headers" ON)
option(ENABLE_UNITY_BUILD "Enable unity build" ON)

add_library(${PROJECT_NAME}_project_warnings INTERFACE)
add_library(${PROJECT_NAME}_project_options INTERFACE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME}_project_options
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src/include>
    )

target_compile_features(${PROJECT_NAME}_project_options
    INTERFACE
    cxx_std_17)

if(ENABLE_PCH)
    target_precompile_headers(
        ${PROJECT_NAME}_project_options
        INTERFACE
        ${CMAKE_SOURCE_DIR}/include/commons/pch.hpp
        )
endif()

target_compile_options(${PROJECT_NAME}_project_warnings
    INTERFACE
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wreturn-type
    -Wuninitialized
    )

if(ONLY_COVERAGE OR ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME}_project_options INTERFACE --coverage -O0 -g)
    target_link_libraries(${PROJECT_NAME}_project_options INTERFACE --coverage)
endif()

if(ENABLE_ASAN)
    target_compile_options(${PROJECT_NAME}_project_options INTERFACE -fsanitize=address)
    target_link_libraries(${PROJECT_NAME}_project_options INTERFACE -fsanitize=address)
endif()

# some GCC specific warnings. These flags are added only if the used compiler is GCC.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}_project_warnings
        INTERFACE
        #        -Wmisleading-indentation
        #        -Wduplicated-cond
        #        -Wlogical-op
        #        -Wuseless-cast
        )
    target_link_libraries(${PROJECT_NAME}_project_options INTERFACE stdc++fs)
endif()

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options (${PROJECT_NAME}_project_options INTERFACE -fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options (${PROJECT_NAME}_project_options INTERFACE -fcolor-diagnostics)
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

include(cmake/FetchContent.cmake)

add_library(${PROJECT_NAME}::${PROJECT_NAME}_project_options ALIAS ${PROJECT_NAME}_project_options)
add_library(${PROJECT_NAME}::${PROJECT_NAME}_project_warnings ALIAS ${PROJECT_NAME}_project_warnings)

add_subdirectory(src)
if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

    install(TARGETS benchmarking conversion files json math network system ${PROJECT_NAME}_project_options ${PROJECT_NAME}_project_warnings EXPORT ${PROJECT_NAME}Targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        )

    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )

    message(STATUS "Generating install")
    set(project_config_in "${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in")
    set(project_config_out "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
    set(config_targets_file "${PROJECT_NAME}Targets.cmake")
    set(version_config_file "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
    set(export_dest_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
    set(pkgconfig_install_dir "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    #set(pkg_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc")
    #message(STATUS "pkg_config=${pkg_config}")

    # ---------------------------------------------------------------------------------------
    # Install pkg-config file
    # ---------------------------------------------------------------------------------------
    #configure_file("cmake/${PROJECT_NAME}.pc.in" "${pkg_config}" @ONLY)
    #install(FILES "${pkg_config}" DESTINATION "${pkgconfig_install_dir}")

    # ---------------------------------------------------------------------------------------
    # Install CMake config files
    # ---------------------------------------------------------------------------------------
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        ${project_config_in}
        ${project_config_out}
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}"
        )

    install(
        FILES ${project_config_out}
        DESTINATION ${export_dest_dir}
        )

    write_basic_package_version_file("${version_config_file}"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
        )
    install(FILES "${project_config_out}" "${version_config_file}" DESTINATION "${export_dest_dir}")

    # ---------------------------------------------------------------------------------------
    # Support creation of installable packages
    # ---------------------------------------------------------------------------------------
    #include(cmake/${PROJECT_NAME}CPack.cmake)

endif()

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
