set(JSON_VERSION 3.12.0)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/archive/refs/tags/v${JSON_VERSION}.tar.gz
)
FetchContent_MakeAvailable(nlohmann_json)
