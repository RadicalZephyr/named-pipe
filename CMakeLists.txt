cmake_minimum_required(VERSION 2.8)

project(named_pipe)

find_package(Boost REQUIRED)
include_directories("${Boost_INCLUDE_DIRS}")

add_executable(test test.cpp)
target_link_libraries(test boost_system boost_filesystem)
