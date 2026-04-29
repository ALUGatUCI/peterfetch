include(FetchContent)

set(CPR_USE_SYSTEM_CURL TRUE)
FetchContent_Declare(
    cpr
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/cpr
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(cpr)

target_link_libraries(peterfetch PRIVATE cpr::cpr)
