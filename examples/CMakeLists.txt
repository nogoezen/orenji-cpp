cmake_minimum_required(VERSION 3.10)
project(OrenjiExamples VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Find TMXLite
find_package(tmxlite CONFIG QUIET)
if(NOT tmxlite_FOUND)
    message(STATUS "TMXLite not found via find_package, will use stub headers for compilation.")
endif()

# Create the TiledMap example executable
add_executable(TiledMapExample TiledMapExample.cpp
    ../src/TiledMap/TiledMap.cpp
    ../src/TiledMap/TiledMapLoader.cpp
    ../src/Core/ResourceCache.cpp
)

# Include directories
target_include_directories(TiledMapExample PRIVATE
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/../deps/tmxlite/include
    ${SFML_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(TiledMapExample PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Link TMXLite if available
if(tmxlite_FOUND)
    target_link_libraries(TiledMapExample PRIVATE tmxlite::tmxlite)
endif()

# Create example map directory
add_custom_command(TARGET TiledMapExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_BINARY_DIR}/assets/maps
)

# Copy a sample .tmx file (if it exists)
if(EXISTS ${CMAKE_SOURCE_DIR}/../assets/maps/example.tmx)
    add_custom_command(TARGET TiledMapExample POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/../assets/maps/example.tmx
        ${CMAKE_BINARY_DIR}/assets/maps/example.tmx
    )
endif()

# Display info
message(STATUS "TiledMapExample will be built. Run it from the build directory.") 