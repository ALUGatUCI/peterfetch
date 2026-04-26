include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY "https://github.com/nlohmann/json.git"
    GIT_TAG "v3.12.0"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(peterfetch PRIVATE nlohmann_json::nlohmann_json)
