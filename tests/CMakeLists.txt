# Copyright (c) 2021 - present, Lars Johannsmeier
# All rights reserved.
# contact: lars.johannsmeier@tum.de

add_library(${PROJECT_NAME}_test_deps INTERFACE)

target_link_libraries(${PROJECT_NAME}_test_deps
    INTERFACE
    CONAN_PKG::catch2
    )

include(CTest)
include(Catch)
add_library(catch_main OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/unit_tests/tests_main.cpp)
target_link_libraries(catch_main
    PRIVATE
    ${PROJECT_NAME}_test_deps
    )



add_subdirectory(unit_tests)

# unit tests
#add_executable(test_benchmarking ${CMAKE_SOURCE_DIR}/tests/unit_tests/test_benchmarking.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_conversion unit_tests/test_conversion.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_files unit_tests/test_files.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_geometry unit_tests/test_geometry.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_json unit_tests/test_json.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_math unit_tests/test_math.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_network unit_tests/test_network.cpp $<TARGET_OBJECTS:catch_main>)
#add_executable(test_system unit_tests/test_system.cpp $<TARGET_OBJECTS:catch_main>)


