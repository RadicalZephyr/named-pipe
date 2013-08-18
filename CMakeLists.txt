cmake_minimum_required(VERSION 2.8)

project(named_pipe)

set(Boost_USE_MULTITHREAD OFF)

find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(servertest servertest.cpp)
target_link_libraries(servertest ${Boost_LIBRARIES})

add_executable(clienttest clienttest.cpp)
target_link_libraries(clienttest ${Boost_LIBRARIES})

enable_testing()
add_test(round_trip "../runtest.sh")

add_executable(echoserver echoserver.cpp)
target_link_libraries(echoserver ${Boost_LIBRARIES})

add_executable(echoclient echoclient.cpp)
target_link_libraries(echoclient ${Boost_LIBRARIES})
