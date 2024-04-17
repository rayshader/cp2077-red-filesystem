cmake_minimum_required(VERSION 3.27)
project(RedFileSystemApi VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(RedFileSystemApi INTERFACE)
target_include_directories(RedFileSystemApi INTERFACE include)