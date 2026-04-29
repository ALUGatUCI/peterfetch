include(FetchContent)

set(ARGS_BUILD_EXAMPLE CACHE BOOL OFF)
set(ARGS_BUILD_UNITTESTS CACHE BOOL OFF)
FetchContent_Declare(
    args
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/args
)
FetchContent_MakeAvailable(args)

target_link_libraries(peterfetch PRIVATE args)
