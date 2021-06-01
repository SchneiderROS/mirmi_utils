add_library(json_lib
    src/json.cpp
    )

add_library(${CMAKE_PROJECT_NAME}::json_lib ALIAS json_lib)

target_include_directories(json_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(json_lib
    PRIVATE
    project_options
    project_warnings
    PUBLIC
    $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
    )

install(TARGETS json_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
