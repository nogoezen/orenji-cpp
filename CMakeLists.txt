cmake_minimum_required(VERSION 3.10)
project(OrenjicGame VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define options for MinGW
if(MINGW)
    add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
    # Link explicitly with the filesystem library
    link_libraries(stdc++fs)
endif()

# Find the SFML package
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Source files
set(SOURCES
    main.cpp
    
    # Core
    src/Core/Entity.cpp
    src/Core/Component.cpp
    src/Core/EntityManager.cpp
    src/Core/SystemManager.cpp
    src/Core/RenderSystem.cpp
    
    # States
    src/States/State.cpp
    src/States/StateMachine.cpp
    src/States/MainMenuState.cpp
    src/States/GameState.cpp
    
    # Utilities
    src/Utilities/ResourceManager.cpp
    src/Utilities/InputHandler.cpp
    
    # Game
    src/Game.cpp
)

# Create the executable
add_executable(OrenjicGame ${SOURCES})

# Include directories
target_include_directories(OrenjicGame PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${SFML_INCLUDE_DIR}
)

# Link SFML libraries
target_link_libraries(OrenjicGame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Set output directory
set_target_properties(OrenjicGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy resource files
add_custom_command(TARGET OrenjicGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/bin/resources
)

# Display helpful messages
message(STATUS "Include directory: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "SFML found: ${SFML_FOUND}")
message(STATUS "SFML include dir: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML libraries: ${SFML_LIBRARIES}") 