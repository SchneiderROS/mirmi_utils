set(MODULE_NAME json)

add_library(${MODULE_NAME}
    src/json.cpp
    )

add_library(${CMAKE_PROJECT_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

target_include_directories(${MODULE_NAME}
    PUBLIC include
    PRIVATE src
    )

set(${MODULE_NAME}_HEADER_FILES
    include/${CMAKE_PROJECT_NAME}/${MODULE_NAME}/json.hpp
    )

target_link_libraries(${MODULE_NAME}
    PRIVATE
    project_options
    project_warnings
    PUBLIC
    $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
    )

set_target_properties(${MODULE_NAME} PROPERTIES PUBLIC_HEADER "${${MODULE_NAME}_HEADER_FILES}")

apply_module_options(${MODULE_NAME})

install(TARGETS ${MODULE_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
