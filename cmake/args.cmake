include(FetchContent)

set(ARGS_BUILD_EXAMPLE CACHE BOOL OFF)
set(ARGS_BUILD_UNITTESTS CACHE BOOL OFF)
FetchContent_Declare(
    args
    GIT_REPOSITORY "https://github.com/Taywee/args.git"
    GIT_TAG "6.4.12"
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(args)

target_link_libraries(peterfetch PRIVATE args)
