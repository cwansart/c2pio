cmake_minimum_required(VERSION 3.10)
project(c2pio VERSION 1.0.0)

find_package(CURL 7.87 REQUIRED)

configure_file(version.h.in version.h)

add_executable(c2pio main.cpp)

target_link_libraries(c2pio PRIVATE CURL::libcurl)

target_include_directories(c2pio PRIVATE "${PROJECT_BINARY_DIR}")

target_compile_features(c2pio PRIVATE cxx_std_11)
