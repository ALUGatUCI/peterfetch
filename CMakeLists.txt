cmake_minimum_required(VERSION 3.20)
project("peterfetch"
        VERSION 0.0
        DESCRIPTION "A terminal fetch application akin to neofetch but for UCI students."
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FETCHCONTENT_QUIET OFF)

find_package(PkgConfig OPTIONAL)
find_package(CURL REQUIRED)

if(NOT PkgConfig_FOUND)
    message(STATUS "")
    message(STATUS "pkg-config not found. Building all dependencies from source.")
    message(STATUS "")

    set(SRC_BUILD_CURLPP TRUE)
    set(SRC_BUILD_ARGS TRUE)
else()
    set(SRC_BUILD_CURLPP FALSE)
    set(SRC_BUILD_ARGS FALSE)
endif()

add_executable(peterfetch)
target_sources(peterfetch PRIVATE src/main.cpp)

include(cmake/cpr.cmake)
include(cmake/args.cmake)
include(cmake/json.cmake)

# Config
set(ANTEATERAPI_URL "https://anteaterapi.com")

configure_file(include/config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${CMAKE_BINARY_DIR}/generated/)

install(TARGETS peterfetch)
