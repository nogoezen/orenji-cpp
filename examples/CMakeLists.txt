cmake_minimum_required(VERSION 3.10)
project(OrenjiExamples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(tmxlite REQUIRED)

# Directories
include_directories(../include)

# TiledMap Example
add_executable(TiledMapExample TiledMapExample.cpp 
    ../src/TiledMap/TiledMap.cpp
    ../src/TiledMap/TiledMapLoader.cpp)

target_link_libraries(TiledMapExample 
    PRIVATE sfml-system sfml-window sfml-graphics
    PRIVATE tmxlite)

# Copy assets for TiledMap example
add_custom_command(TARGET TiledMapExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:TiledMapExample>/assets/maps
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/../assets/maps
    $<TARGET_FILE_DIR:TiledMapExample>/assets/maps
)

# Add more examples here as needed 