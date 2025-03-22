cmake_minimum_required(VERSION 3.15)
project(orenji-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Add executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/tgui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/nlohmann
)

# Set library paths
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/tgui/lib
)

# Find SFML
find_package(SFML 3 COMPONENTS Graphics Window Audio
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml
    NO_DEFAULT_PATH
)

# Box2D manual setup
set(BOX2D_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d/lib)
if(EXISTS ${BOX2D_LIB_DIR}/libbox2d.a)
    set(BOX2D_LIBRARY ${BOX2D_LIB_DIR}/libbox2d.a)
elseif(EXISTS ${BOX2D_LIB_DIR}/box2d.lib)
    set(BOX2D_LIBRARY ${BOX2D_LIB_DIR}/box2d.lib)
else()
    message(WARNING "Box2D library not found.  Will continue without Box2D.")
    set(BOX2D_LIBRARY "")
endif()

# TGUI manual setup
set(TGUI_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/tgui/lib)
if(EXISTS ${TGUI_LIB_DIR}/tgui.lib)
    set(TGUI_LIBRARY ${TGUI_LIB_DIR}/tgui.lib)
elseif(EXISTS ${TGUI_LIB_DIR}/libtgui.a)
    set(TGUI_LIBRARY ${TGUI_LIB_DIR}/libtgui.a)
else()
    message(WARNING "TGUI library not found. Will continue without TGUI.")
    set(TGUI_LIBRARY "")
endif()

# Link libraries - use static libs directly
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/lib/libsfml-graphics.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/lib/libsfml-window.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/lib/libsfml-system.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/lib/libsfml-audio.a
    ${BOX2D_LIBRARY}
    ${TGUI_LIBRARY}
)

# Add definitions for SFML 3
target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_V3)

# Create resources directories if they don't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/sounds)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/maps)

# Copy resources to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

# Copy SFML DLLs (for Windows)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/tgui/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    
    # Copy Box2D DLLs if they exist
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d/bin)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/lib/box2d/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
endif() 