include(FetchContent)

if (WIN32)
    set(CPR_USE_SYSTEM_CURL OFF)
     set(CPR_CURL_USE_LIBPSL OFF)
else()
    set(CPR_USE_SYSTEM_CURL ON)
endif()

FetchContent_Declare(
    cpr
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/cpr
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(cpr)

target_link_libraries(peterfetch PRIVATE cpr::cpr)
