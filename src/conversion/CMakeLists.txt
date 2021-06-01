add_library(conversion_lib
    src/conversion.cpp
    )

target_include_directories(conversion_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(conversion_lib
    PRIVATE
    project_options
    project_warnings
    PUBLIC
    )

install(TARGETS conversion_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
