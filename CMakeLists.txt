cmake_minimum_required(VERSION 3.28)

project(LearnRTSeries)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src/private)
include_directories(stb src/public)
