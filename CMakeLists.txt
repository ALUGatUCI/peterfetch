cmake_minimum_required(VERSION 3.20)
project("peterfetch")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



add_executable(peterfetch)
target_sources(peterfetch PRIVATE src/main.cpp)

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
    target_compile_options(peterfetch PRIVATE ${CURLPP_CFLAGS_OTHER})
endif()

install(TARGETS peterfetch)
