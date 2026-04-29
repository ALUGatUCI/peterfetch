include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/nlohmann-json
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(peterfetch PRIVATE nlohmann_json::nlohmann_json)
