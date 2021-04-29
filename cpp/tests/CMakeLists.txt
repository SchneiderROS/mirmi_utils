# Copyright (c) 2021 - present, Lars Johannsmeier
# All rights reserved.
# contact: lars.johannsmeier@tum.de

# Get Catch2
find_package(Catch2 2.13.3)
if(catch2_FOUND)
    include(Catch)
    # TODO test this
    add_library(catch_main OBJECT tests_main.cpp ${catch2_INCLUDE_DIRS})
    target_include_directories(catch_main PRIVATE ${catch2_INCLUDE_DIRS})
else()
    message(STATUS "Catch2 not found: getting it now")
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.3)

    FetchContent_MakeAvailable(Catch2)
    include(${catch2_SOURCE_DIR}/contrib/Catch.cmake)
    include(${catch2_SOURCE_DIR}/contrib/ParseAndAddCatchTests.cmake)
    add_library(catch_main OBJECT tests_main.cpp ${catch2_SOURCE_DIR}/single_include/catch2)
    target_include_directories(catch_main PRIVATE ${catch2_SOURCE_DIR}/single_include)
endif()

include(CTest)

# unit tests
add_executable(test_benchmarking unit_tests/test_benchmarking.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_conversion unit_tests/test_conversion.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_files.cpp unit_tests/test_files.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_geometry.cpp unit_tests/test_geometry.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_json.cpp unit_tests/test_json.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_math.cpp unit_tests/test_math.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_network.cpp unit_tests/test_network.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_system.cpp unit_tests/test_system.cpp $<TARGET_OBJECTS:catch_main>)

# integration tests

set(TEST_TARGETS
    test_benchmarking
    test_conversion
    test_files
    test_geometry
    test_json
    test_math
    test_network
    test_system
)

foreach(name ${TEST_TARGETS})
    target_link_libraries(${name} PRIVATE
        Catch2::Catch2
        msrm_cpp_utils::msrm_cpp_utils
        simple-websocket-server
        spdlog::spdlog_header_only
        nlohmann_json::nlohmann_json
    )

    set_target_properties(${name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS OFF
    )

    # Add desired warnings
    if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
        target_compile_options( ${name}  PRIVATE -Wall -Wextra -Wunreachable-code )
    endif()
    # Clang specific warning go here
    if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
        # Actually keep these
        target_compile_options( ${name}  PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn )
    endif()
    if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
        target_compile_options( ${name}  PRIVATE /W4 /w44265 /WX )
    endif()
    catch_discover_tests(${name})
endforeach()
