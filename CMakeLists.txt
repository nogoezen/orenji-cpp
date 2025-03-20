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

# Find Box2D
find_package(box2d CONFIG REQUIRED)

# Find Thor
find_package(Thor CONFIG QUIET)
if(NOT Thor_FOUND)
    message(STATUS "Thor not found via find_package, will try to build from source")
    # Option 1: Use FetchContent to download and build Thor
    include(FetchContent)
    FetchContent_Declare(
        thor
        GIT_REPOSITORY https://github.com/Bromeon/Thor.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(thor)
    if(TARGET thor)
        message(STATUS "Thor downloaded and built successfully")
    else()
        message(WARNING "Failed to build Thor automatically. Please install Thor manually.")
    endif()
endif()

# Find TMXLite
find_package(tmxlite CONFIG QUIET)
if(NOT tmxlite_FOUND)
    message(STATUS "TMXLite not found via find_package, will use stub headers for compilation.")
    # Use stub headers for compilation until the actual library is installed
endif()

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
    src/States/TitleState.cpp
    
    # TiledMap
    src/TiledMap/TiledMap.cpp
    src/TiledMap/TiledMapLoader.cpp
    src/TiledMap/TiledMapRenderer.cpp
    src/TiledMap/TiledMapScene.cpp
    src/TiledMap/TiledMapCollision.cpp
    src/TiledMap/TiledMapFactory.cpp
    src/TiledMap/TiledMapModule.cpp
    
    # Physics
    src/Physics/PhysicsWorld.cpp
    src/Physics/PhysicsComponent.cpp
    
    # Particles
    src/Particles/ParticleSystem.cpp
    src/Particles/ParticleComponent.cpp
    
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
    ${CMAKE_SOURCE_DIR}/deps/tmxlite/include
    ${SFML_INCLUDE_DIR}
)

# Link SFML libraries
target_link_libraries(OrenjicGame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    box2d::box2d
)

# Link Thor if available
if(TARGET thor)
    target_link_libraries(OrenjicGame PRIVATE thor)
elseif(Thor_FOUND)
    target_link_libraries(OrenjicGame PRIVATE Thor::thor)
endif()

# Link TMXLite if available
if(tmxlite_FOUND)
    target_link_libraries(OrenjicGame PRIVATE tmxlite::tmxlite)
endif()

# Set output directory
set_target_properties(OrenjicGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy resource files
add_custom_command(TARGET OrenjicGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/bin/resources
)

# Copy maps directory
add_custom_command(TARGET OrenjicGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/maps ${CMAKE_BINARY_DIR}/bin/resources/maps
)

# Display helpful messages
message(STATUS "Include directory: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "SFML found: ${SFML_FOUND}")
message(STATUS "SFML include dir: ${SFML_INCLUDE_DIR}")
message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
message(STATUS "Box2D found: ${box2d_FOUND}")
if(Thor_FOUND)
    message(STATUS "Thor found via find_package")
elseif(TARGET thor)
    message(STATUS "Thor built from source")
else()
    message(WARNING "Thor not available, particle system functionality will be limited")
endif()
if(tmxlite_FOUND)
    message(STATUS "TMXLite found via find_package")
else()
    message(STATUS "TMXLite not found, using stub headers. Please install TMXLite for full functionality.")
endif() 