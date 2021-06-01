add_library(system_lib
    src/system.cpp
    )

add_library(${CMAKE_PROJECT_NAME}::system_lib ALIAS system_lib)


target_include_directories(system_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(system_lib
    PRIVATE
    project_options
    project_warnings
    PUBLIC
    )

install(TARGETS system_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
