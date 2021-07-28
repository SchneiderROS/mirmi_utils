set(MODULE_NAME json)

set(${MODULE_NAME}_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json.cpp
    )

set(${MODULE_NAME}_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/${MODULE_NAME}/json.hpp
    )

add_library(${MODULE_NAME}
    ${${MODULE_NAME}_SRC}
    ${${MODULE_NAME}_HEADER_FILES}
    )

add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
    src
    )

find_package(Eigen3 REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(${MODULE_NAME}
    PRIVATE
    ${PROJECT_NAME}_project_options
    ${PROJECT_NAME}_project_warnings
    ${PROJECT_NAME}_project_deps
    spdlog::spdlog
    PUBLIC
    Eigen3::Eigen
    nlohmann_json::nlohmann_json
    )


set_target_properties(${MODULE_NAME} PROPERTIES PUBLIC_HEADER "${${MODULE_NAME}_HEADER_FILES}")

apply_module_options(${MODULE_NAME})
