cmake_minimum_required(VERSION 3.9)
project(basic_cpp)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(basic_cpp basic_cpp.cpp)

target_link_libraries(basic_cpp Threads::Threads)