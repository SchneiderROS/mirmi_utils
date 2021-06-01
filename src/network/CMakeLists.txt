add_library(network_lib
    src/network.cpp
    )

add_library(${CMAKE_PROJECT_NAME}::network_lib ALIAS network_lib)

target_include_directories(network_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(network_lib
    PRIVATE
    project_options
    project_warnings
    ${CMAKE_PROJECT_NAME}::files_lib
    ${CMAKE_PROJECT_NAME}::json_lib
    PUBLIC
    $<BUILD_INTERFACE:json-rpc-cxx>
    $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
    $<BUILD_INTERFACE:simple-websocket-server>
    $<BUILD_INTERFACE:httplib>
    )

install(TARGETS network_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
