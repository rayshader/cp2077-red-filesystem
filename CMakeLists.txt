cmake_minimum_required(VERSION 3.27)
project(RedFileSystem VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_library(RedFileSystem INTERFACE)
target_compile_definitions(RedFileSystem INTERFACE HAS_REDFILESYSTEM)
target_include_directories(RedFileSystem INTERFACE include)