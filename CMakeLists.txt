cmake_minimum_required(VERSION 3.27)
project(RedFS VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(INC_FILES
        src/Plugin.h

        src/File.h
        src/FileSystem.h
        src/FileSystemPrefix.h
        src/FileSystemStatus.h
        src/FileSystemWriteMode.h

        src/json/JsonType.h
        src/json/JsonVariant.h
        src/json/JsonNull.h
        src/json/JsonBool.h
        src/json/JsonInt64.h
        src/json/JsonDouble.h
        src/json/JsonString.h
        src/json/JsonArray.h
        src/json/JsonObject.h
        src/json/JsonFactory.h
)

set(SRC_FILES
        src/main.cpp
        src/Plugin.cpp

        src/File.cpp
        src/FileSystem.cpp

        src/json/JsonVariant.cpp
        src/json/JsonNull.cpp
        src/json/JsonBool.cpp
        src/json/JsonInt64.cpp
        src/json/JsonDouble.cpp
        src/json/JsonString.cpp
        src/json/JsonArray.cpp
        src/json/JsonObject.cpp
)

source_group(include FILES INC_FILES)
source_group(source FILES SRC_FILES)

add_library(RedFS SHARED
        ${INC_FILES}
        ${SRC_FILES}
)

target_include_directories(RedFS PRIVATE src/)
target_include_directories(RedFS PRIVATE src/json/)

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

## Library simdjson
include(FetchContent)

FetchContent_Declare(
        simdjson
        GIT_REPOSITORY https://github.com/simdjson/simdjson.git
        GIT_TAG  tags/v3.7.0
        GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(simdjson)
##

target_link_libraries(RedFS PRIVATE
        RED4ext::SDK
        RedLib
        simdjson
)
