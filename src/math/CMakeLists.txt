#find_package(Eigen3 REQUIRED)
#if(NOT TARGET Eigen3::Eigen3)
#  add_library(Eigen3::Eigen3 INTERFACE IMPORTED)
#  set_target_properties(Eigen3::Eigen3 PROPERTIES
#    INTERFACE_INCLUDE_DIRECTORIES ${EIGEN3_INCLUDE_DIRS}
#    INTERFACE_COMPILE_DEFINITIONS "${EIGEN3_DEFINITIONS}"
#  )
#endif()


add_library(math_lib
    src/math.cpp
    )

target_include_directories(math_lib
    PUBLIC include
    PRIVATE src
    )

target_link_libraries(math_lib
    PRIVATE
    project_options
    project_warnings
    PUBLIC
#    Eigen3::Eigen3
    )

install(TARGETS math_lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR})
