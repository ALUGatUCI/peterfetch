include(FetchContent)

set(CPR_USE_SYSTEM_CURL TRUE)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY "https://github.com/libcpr/cpr.git"
    GIT_TAG "1.14.2"
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(cpr)

target_link_libraries(peterfetch PRIVATE cpr::cpr)
