cmake_minimum_required(VERSION 3.10)
project(UnchartedWaters VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Executable
add_executable(unchartedwaters ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(unchartedwaters PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Copy data directory to build directory
add_custom_command(TARGET unchartedwaters POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:unchartedwaters>/data
)

# Set output directory
set_target_properties(unchartedwaters PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Enable warnings
if(MSVC)
    target_compile_options(unchartedwaters PRIVATE /W4)
else()
    target_compile_options(unchartedwaters PRIVATE -Wall -Wextra -Wpedantic)
endif() 