cmake_minimum_required(VERSION 3.27)
project(RedFileSystem VERSION 0.9.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

configure_file(src/Config.h.in Config/Config.h)

set(INC_FILES
        src/AsyncFile.h
        src/File.h
        src/FilePromise.h
        src/FileSystem.h
        src/FileSystemStorage.h

        src/FileSystemStatus.h
        src/FileSystemWriteMode.h

        src/Utils.h
)

set(SRC_FILES
        src/main.cpp

        src/AsyncFile.cpp
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

add_compile_definitions(
        # Exclude unused Windows headers
        WIN32_LEAN_AND_MEAN

        # Use Unicode charset.
        UNICODE
        _UNICODE
)

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

## Library RedData
add_subdirectory(deps/RedData)
set_target_properties(RedData PROPERTIES FOLDER "Dependencies")
##

target_link_libraries(RedFileSystem PRIVATE
        RED4ext::SDK
        RedLib
        RedData
)

## Debug mode: install scripts (+ tests) and plugin in game folder.
## Release mode: create archive with bundled scripts and plugin.
add_custom_command(
        TARGET RedFileSystem
        POST_BUILD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "$<$<CONFIG:Debug>:Install scripts with red-cli>" "$<$<CONFIG:Release>:Build archive with red-cli>"
        COMMAND "$<$<CONFIG:Debug>:red-cli;install;--debug>" "$<$<CONFIG:Release>:red-cli;pack>"
        COMMAND_EXPAND_LISTS
)