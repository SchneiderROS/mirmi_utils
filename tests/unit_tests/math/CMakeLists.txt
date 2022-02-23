set(TEST_NAME test_math)

add_executable(${TEST_NAME} test_math.cpp $<TARGET_OBJECTS:catch_main>)

target_link_libraries(${TEST_NAME}
    PRIVATE
    ${PROJECT_NAME}_test_deps
    mirmi_cpp_utils::math
    )

catch_discover_tests(${TEST_NAME})
