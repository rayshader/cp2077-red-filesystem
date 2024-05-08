cmake_minimum_required(VERSION 3.27)
project(RedFileSystem VERSION 0.7.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

configure_file(src/Config.h.in Config/Config.h)

set(INC_FILES
        src/File.h
        src/FileSystem.h
        src/FileSystemStorage.h

        src/FileSystemStatus.h
        src/FileSystemWriteMode.h
)

set(SRC_FILES
        src/main.cpp

        src/File.cpp
        src/FileSystem.cpp
        src/FileSystemStorage.cpp
)

source_group(include FILES INC_FILES)
source_group(source FILES SRC_FILES)

add_library(RedFileSystem SHARED
        ${INC_FILES}
        ${SRC_FILES}
)

target_include_directories(RedFileSystem PRIVATE src/)

# Include configured files
target_include_directories(RedFileSystem PUBLIC "${PROJECT_BINARY_DIR}/Config")

# Exclude unused Windows headers
add_compile_definitions(WIN32_LEAN_AND_MEAN)

## Library RED4ext.SDK
option(RED4EXT_HEADER_ONLY "" ON)
add_subdirectory(deps/RED4ext.SDK)
set_target_properties(RED4ext.SDK PROPERTIES FOLDER "Dependencies")
mark_as_advanced(RED4EXT_BUILD_EXAMPLES RED4EXT_HEADER_ONLY)
##

## Library RedLib
add_compile_definitions(NOMINMAX)
add_subdirectory(deps/RedLib)
set_target_properties(RedLib PROPERTIES FOLDER "Dependencies")
##

target_link_libraries(RedFileSystem PRIVATE
        RED4ext::SDK
        RedLib
)
