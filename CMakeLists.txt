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

# Set up custom library paths
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(SFML_DIR "${LIB_DIR}/sfml")
set(BOX2D_DIR "${LIB_DIR}/box2d")
set(TMXLITE_DIR "${LIB_DIR}/tmxlite")

# Add the custom paths to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH ${SFML_DIR} ${BOX2D_DIR} ${TMXLITE_DIR})

# Find the SFML package
find_package(SFML 3.0 COMPONENTS graphics window system audio)
if(NOT SFML_FOUND)
    # Fallback to manual configuration if package not found
    message(STATUS "SFML package not found, using manual configuration")
    set(SFML_INCLUDE_DIR "${SFML_DIR}/include")
    set(SFML_LIBRARIES
        "${SFML_DIR}/lib/libsfml-graphics.a"
        "${SFML_DIR}/lib/libsfml-window.a"
        "${SFML_DIR}/lib/libsfml-system.a"
        "${SFML_DIR}/lib/libsfml-audio.a"
    )
endif()

# Find Box2D
find_package(box2d CONFIG QUIET)
if(NOT box2d_FOUND)
    message(STATUS "Box2D package not found, using manual configuration")
    set(BOX2D_INCLUDE_DIR "${BOX2D_DIR}/include")
    if(EXISTS "${BOX2D_DIR}/lib/libbox2d.a")
        set(BOX2D_LIBRARIES "${BOX2D_DIR}/lib/libbox2d.a")
    else()
        message(WARNING "Box2D library not found in ${BOX2D_DIR}/lib")
    endif()
else()
    set(BOX2D_INCLUDE_DIR "${box2d_DIR}/../../../include")
endif()

# Box2D version check
if(EXISTS "${BOX2D_INCLUDE_DIR}/box2d/box2d.h")
    message(STATUS "Box2D library found")
    add_definitions(-DBOX2D_2_4_X)
endif()

# Find TMXLite
find_package(tmxlite CONFIG QUIET)
if(NOT tmxlite_FOUND)
    message(STATUS "TMXLite package not found, using manual configuration")
    set(TMXLITE_INCLUDE_DIR "${TMXLITE_DIR}/include")
    if(EXISTS "${TMXLITE_DIR}/lib/libtmxlite.a")
        set(TMXLITE_LIBRARIES "${TMXLITE_DIR}/lib/libtmxlite.a")
    else()
        message(WARNING "TMXLite library not found in ${TMXLITE_DIR}/lib")
    endif()
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
    
    # Models
    src/models/Character.cpp
    src/models/City.cpp
    src/models/Fleet.cpp
    src/models/Player.cpp
    src/models/Ship.cpp
    src/models/ShipCrew.cpp
    src/models/TradeGood.cpp
    src/models/TradingSystem.cpp
    src/models/World.cpp
)

# Define game state and Thor as disabled by default
# Change these to enable specific modules
option(ENABLE_GAMESTATE "Enable the game state" ON)
option(ENABLE_THOR "Enable Thor-based particle and animation systems" OFF)

# Set compile definitions based on options
if(NOT ENABLE_GAMESTATE)
    add_definitions(-DDISABLE_GAMESTATE)
endif()

# Force disable Thor - we'll use our own particle system implementation
add_definitions(-DDISABLE_THOR)

# Create the executable
add_executable(OrenjicGame ${SOURCES})

# Include directories
target_include_directories(OrenjicGame PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${SFML_INCLUDE_DIR}
    ${BOX2D_INCLUDE_DIR}
    ${TMXLITE_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(OrenjicGame PRIVATE
    ${SFML_LIBRARIES}
)

# Link Box2D
if(box2d_FOUND)
    target_link_libraries(OrenjicGame PRIVATE box2d::box2d)
elseif(BOX2D_LIBRARIES)
    target_link_libraries(OrenjicGame PRIVATE ${BOX2D_LIBRARIES})
endif()

# Link TMXLite if available
if(tmxlite_FOUND)
    target_link_libraries(OrenjicGame PRIVATE tmxlite::tmxlite)
elseif(TMXLITE_LIBRARIES)
    target_link_libraries(OrenjicGame PRIVATE ${TMXLITE_LIBRARIES})
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

# Copy SFML DLLs
if(MSVC OR MINGW)
    # Use the new SFML DLL location
    set(SFML_DLL_DIR "${SFML_DIR}/bin")
    
    if(EXISTS "${SFML_DLL_DIR}")
        message(STATUS "SFML DLLs found at: ${SFML_DLL_DIR}")
        
        # Check for SFML 3.0 DLLs and copy them
        if(EXISTS "${SFML_DLL_DIR}/sfml-graphics-3.dll")
            add_custom_command(TARGET OrenjicGame POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SFML_DLL_DIR}/sfml-graphics-3.dll"
                "${SFML_DLL_DIR}/sfml-window-3.dll"
                "${SFML_DLL_DIR}/sfml-system-3.dll"
                "${SFML_DLL_DIR}/sfml-audio-3.dll"
                "${CMAKE_BINARY_DIR}/bin"
            )
            message(STATUS "Release DLLs will be copied to bin directory")
        endif()
        
        # Check for debug SFML 3.0 DLLs
        if(EXISTS "${SFML_DLL_DIR}/sfml-graphics-d-3.dll")
            add_custom_command(TARGET OrenjicGame POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SFML_DLL_DIR}/sfml-graphics-d-3.dll"
                "${SFML_DLL_DIR}/sfml-window-d-3.dll"
                "${SFML_DLL_DIR}/sfml-system-d-3.dll"
                "${SFML_DLL_DIR}/sfml-audio-d-3.dll"
                "${CMAKE_BINARY_DIR}/bin"
            )
            message(STATUS "Debug DLLs will be copied to bin directory")
        endif()
    else()
        message(WARNING "SFML DLL directory not found at ${SFML_DLL_DIR}. You'll need to copy DLLs manually.")
    endif()
    
    # Copy Box2D DLLs if they exist
    set(BOX2D_DLL_DIR "${BOX2D_DIR}/bin")
    if(EXISTS "${BOX2D_DLL_DIR}")
        file(GLOB BOX2D_DLLS "${BOX2D_DLL_DIR}/*.dll")
        if(BOX2D_DLLS)
            add_custom_command(TARGET OrenjicGame POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${BOX2D_DLLS}
                "${CMAKE_BINARY_DIR}/bin"
            )
            message(STATUS "Box2D DLLs will be copied to bin directory")
        endif()
    endif()
    
    # Copy TMXLite DLLs if they exist
    set(TMXLITE_DLL_DIR "${TMXLITE_DIR}/bin")
    if(EXISTS "${TMXLITE_DLL_DIR}")
        file(GLOB TMXLITE_DLLS "${TMXLITE_DLL_DIR}/*.dll")
        if(TMXLITE_DLLS)
            add_custom_command(TARGET OrenjicGame POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${TMXLITE_DLLS}
                "${CMAKE_BINARY_DIR}/bin"
            )
            message(STATUS "TMXLite DLLs will be copied to bin directory")
        endif()
    endif()
endif()

# Display helpful messages
message(STATUS "Include directory: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "SFML include dir: ${SFML_INCLUDE_DIR}")
message(STATUS "Box2D include dir: ${BOX2D_INCLUDE_DIR}")
message(STATUS "TMXLite include dir: ${TMXLITE_INCLUDE_DIR}")
message(STATUS "Game state enabled: ${ENABLE_GAMESTATE}")
message(STATUS "Thor disabled: using integrated particle system") 