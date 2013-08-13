cmake_minimum_required(VERSION 2.8)

project(named_pipe)

find_package(Boost REQUIRED)
include_directories("${Boost_INCLUDE_DIRS}")

add_executable(servertest servertest.cpp)
target_link_libraries(servertest boost_system boost_filesystem)

add_executable(clienttest clienttest.cpp)
target_link_libraries(clienttest boost_system boost_filesystem)

enable_testing()
add_test(round_trip "../runtest.sh")
