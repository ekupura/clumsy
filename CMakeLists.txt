cmake_minimum_required(VERSION 3.9)
project(Clumsy CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14")

add_executable(clumsy main.cpp)
target_link_libraries(clumsy readline)
