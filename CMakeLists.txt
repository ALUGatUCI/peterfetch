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

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURLPP REQUIRED curlpp)
pkg_check_modules(LIBARGS REQUIRED args)

add_executable(peterfetch)
target_sources(peterfetch PRIVATE src/main.cpp)

# Deps: Curl
if(APPLE)
    execute_process(
        COMMAND brew --prefix curlpp
        OUTPUT_VARIABLE CURLPP_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE BREW_RESULT
    )
    if(NOT BREW_RESULT EQUAL 0)
        message(FATAL_ERROR "curlpp not found. Install with: brew install curlpp")
    endif()
    target_include_directories(peterfetch PRIVATE ${CURLPP_PREFIX}/include)
    target_link_directories(peterfetch PRIVATE ${CURLPP_PREFIX}/lib)
    target_link_libraries(peterfetch PRIVATE curlpp curl)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CURLPP REQUIRED curlpp)
    target_include_directories(peterfetch PRIVATE ${CURLPP_INCLUDE_DIRS})
    target_link_directories(peterfetch PRIVATE ${CURLPP_LIBRARY_DIRS})
    target_link_libraries(peterfetch PRIVATE ${CURLPP_LIBRARIES})
endif()

# Deps: lib args
target_include_directories(peterfetch PUBLIC ${LIBARGS_INCLUDE_DIRS})

# Config
set(ANTEATERAPI_URL "https://anteaterapi.com")

configure_file(include/config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${CMAKE_BINARY_DIR}/generated/)

install(TARGETS peterfetch)
