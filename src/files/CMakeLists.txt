add_library(files_lib
    src/files.cpp
    )

add_library(${CMAKE_PROJECT_NAME}::files_lib ALIAS files_lib)

target_include_directories(files_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(files_lib
    PRIVATE
    project_options
    project_warnings
    PUBLIC
    )

install(TARGETS files_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
