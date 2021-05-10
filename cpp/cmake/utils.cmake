# Get msrm_utils version from include/commons/version.hpp and put it in MSRM_UTILS_VERSION
function(get_msrm_utils_version)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/include/commons/version.hpp" file_contents)
    string(REGEX MATCH "MSRM_UTILS_VER_MAJOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number from include/commons/version.hpp")
    endif()
    set(ver_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "MSRM_UTILS_VER_MINOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number from include/commons/version.hpp")
    endif()

    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "MSRM_UTILS_VER_PATCH ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number from include/commons/version.hpp")
    endif()
    set(ver_patch ${CMAKE_MATCH_1})

    set(MSRM_UTILS_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
    set(MSRM_UTILS_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
    set(MSRM_UTILS_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
    set(MSRM_UTILS_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()

