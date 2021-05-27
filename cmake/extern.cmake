FetchContent_Declare(
    jsonrpc
    GIT_REPOSITORY https://github.com/jsonrpcx/json-rpc-cxx.git
    GIT_TAG v0.3.0)

set(COMPILE_TESTS OFF CACHE INTERNAL "Build SHARED libraries")
set(COMPILE_EXAMPLES OFF CACHE INTERNAL "Build SHARED libraries")

FetchContent_MakeAvailable(jsonrpc)

