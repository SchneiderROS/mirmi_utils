add_library(benchmarking_lib
    src/benchmarking.cpp
    )

target_include_directories(benchmarking_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(benchmarking_lib
    PRIVATE
    project_options
    project_warnings
    PUBLIC
    )

install(TARGETS benchmarking_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
