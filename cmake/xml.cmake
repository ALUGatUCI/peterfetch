include(FetchContent)

set(LIBXML2_WITH_TESTS CACHE BOOL OFF)
set(LIBXML2_WITH_PROGRAMS CACHE BOOL OFF)
set(LIBXML2_WITH_MODULES CACHE BOOL OFF)
set(BUILD_SHARED_LIBS CACHE BOOL OFF)
FetchContent_Declare(
    libxml2
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/libxml2
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(libxml2)

target_link_libraries(peterfetch PRIVATE LibXml2)
