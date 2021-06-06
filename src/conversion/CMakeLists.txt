set(MODULE_NAME conversion)

set(${MODULE_NAME}_SRC
    src/conversion.cpp
    )

set(${MODULE_NAME}_HEADER_FILES
    include/${PROJECT_NAME}/${MODULE_NAME}/conversion.hpp
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
    mcu_project_options
    mcu_project_warnings
    PUBLIC
    )

set_target_properties(${MODULE_NAME} PROPERTIES PUBLIC_HEADER "${${MODULE_NAME}_HEADER_FILES}")

apply_module_options(${MODULE_NAME})

install(TARGETS ${MODULE_NAME} EXPORT ${MODULE_NAME}Targets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${MODULE_NAME}
    RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${MODULE_NAME}
    ARCHIVE DESTINATION       ${CMAKE_INSTALL_SHAREDSTATEDIR}
    )

install(EXPORT ${MODULE_NAME}Targets
    FILE ${MODULE_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::${MODULE_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${MODULE_NAME}
    )
