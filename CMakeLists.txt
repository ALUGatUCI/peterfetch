cmake_minimum_required(VERSION 3.20)
project("peterfetch")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURLPP REQUIRED curlpp)

add_executable(peterfetch)
target_sources(peterfetch PRIVATE src/main.cpp)
target_link_libraries(peterfetch ${CURLPP_LIBRARIES})
target_include_directories(peterfetch PUBLIC ${CURLPP_INCLUDE_DIRS})
target_compile_options(peterfetch PUBLIC ${CURLPP_CFLAGS_OTHER})

install(TARGETS peterfetch)
