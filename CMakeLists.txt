cmake_minimum_required(VERSION 3.16.3)
project(LFU)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} cache.cpp)
