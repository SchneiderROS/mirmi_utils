include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/_deps)

FetchContent_Declare(
    jsonrpc
    GIT_REPOSITORY https://github.com/jsonrpcx/json-rpc-cxx.git
    GIT_TAG v0.3.0)

set(COMPILE_TESTS OFF CACHE INTERNAL "Build SHARED libraries")
set(COMPILE_EXAMPLES OFF CACHE INTERNAL "Build SHARED libraries")

FetchContent_Declare(
    cpp-httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.8.9)

FetchContent_MakeAvailable(jsonrpc cpp-httplib)

