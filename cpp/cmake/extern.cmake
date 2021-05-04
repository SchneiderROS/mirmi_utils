FetchContent_Declare(
    simple-websocket-server
    GIT_REPOSITORY https://gitlab.com/eidheim/Simple-WebSocket-Server.git
    GIT_TAG v2.0.2)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
    GIT_TAG v3.9.1)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.8.8)

FetchContent_Declare(
    jsonrpc
    GIT_REPOSITORY https://github.com/jsonrpcx/json-rpc-cxx.git
    GIT_TAG v0.3.0)

FetchContent_MakeAvailable(simple-websocket-server json httplib jsonrpc)

