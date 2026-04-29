cmake_minimum_required(VERSION 3.20)
project("peterfetch"
        VERSION 0.0
        DESCRIPTION "A terminal fetch application akin to neofetch but for UCI students."
        LANGUAGES CXX
)

# Default to full debug mode
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE CACHE STRING Debug FORCE)
endif()

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

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT
    )
    if(NOT GIT_SUBMOD_RESULT EQUAL 0)
        message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
    endif()
endif()

add_executable(peterfetch)
target_include_directories(peterfetch PRIVATE include)
target_sources(
    peterfetch PRIVATE
    src/main.cpp
    src/directory_info.cpp
)

include(cmake/cpr.cmake)
include(cmake/args.cmake)
include(cmake/json.cmake)
include(cmake/xml.cmake)

configure_file(include/config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${CMAKE_BINARY_DIR}/generated/)

install(TARGETS peterfetch)
