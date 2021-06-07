set(MODULE_NAME files)

set(${MODULE_NAME}_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/files.cpp
    )

set(${MODULE_NAME}_HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/${MODULE_NAME}/files.hpp
    )

if(BUILD_SHARED_LIBS)
    add_library(${MODULE_NAME}
        SHARED
        ${${MODULE_NAME}_SRC}
        ${${MODULE_NAME}_HEADER_FILES}
        )
else()
    add_library(${MODULE_NAME}
        STATIC
        ${${MODULE_NAME}_SRC}
        ${${MODULE_NAME}_HEADER_FILES}
        )
endif()

add_library(${PROJECT_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
    src
    )

target_link_libraries(${MODULE_NAME}
    PRIVATE
    ${PROJECT_NAME}_project_options
    ${PROJECT_NAME}_project_warnings
    PUBLIC
    )

set_target_properties(${MODULE_NAME} PROPERTIES PUBLIC_HEADER "${${MODULE_NAME}_HEADER_FILES}")

apply_module_options(${MODULE_NAME})
