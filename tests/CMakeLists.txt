# Copyright (c) 2021 - present, Lars Johannsmeier
# All rights reserved.
# contact: lars.johannsmeier@tum.de

# Get Catch2
include(Catch)
add_library(catch_main OBJECT ${CMAKE_SOURCE_DIR}/tests/unit_tests/tests_main.cpp ${catch2_INCLUDE_DIRS})
target_include_directories(catch_main PRIVATE ${catch2_INCLUDE_DIRS})

include(CTest)

# unit tests
add_executable(test_benchmarking ${CMAKE_SOURCE_DIR}/tests/unit_tests/test_benchmarking.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_conversion unit_tests/test_conversion.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_files unit_tests/test_files.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_geometry unit_tests/test_geometry.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_json unit_tests/test_json.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_math unit_tests/test_math.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_network unit_tests/test_network.cpp $<TARGET_OBJECTS:catch_main>)
add_executable(test_system unit_tests/test_system.cpp $<TARGET_OBJECTS:catch_main>)

## integration tests

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
        msrm_cpp_utils::msrm_cpp_utils
        $<BUILD_INTERFACE:json-rpc-cxx>
        pthread
        crypto
        dl
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
