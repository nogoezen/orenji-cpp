cmake_minimum_required(VERSION 3.16)
project(OrenjicEngine VERSION 0.1.0 LANGUAGES CXX)

# Options de compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Répertoires de sortie
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Options du projet
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

# Inclusion des en-têtes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Recherche des dépendances
# SFML
find_package(SFML 3 COMPONENTS system window graphics audio REQUIRED)

# Box2D
find_package(box2d 3 REQUIRED)

# nlohmann_json
find_package(nlohmann_json 3 REQUIRED)

# TGUI (optionnel)
find_package(TGUI 1 QUIET)
if(TGUI_FOUND)
    message(STATUS "TGUI found: ${TGUI_VERSION}")
    set(HAVE_TGUI ON)
else()
    message(WARNING "TGUI not found, UI features will be disabled")
    set(HAVE_TGUI OFF)
endif()

# Récupération des sources
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Création de la bibliothèque principale
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Configuration des fonctionnalités
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Config.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Config.hpp
)

# Liaisons des bibliothèques
target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    box2d
    nlohmann_json::nlohmann_json
)

if(HAVE_TGUI)
    target_link_libraries(${PROJECT_NAME} PUBLIC tgui)
endif()

# Activation des warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Application principale
add_executable(OrenjicGame main.cpp)
target_link_libraries(OrenjicGame PRIVATE ${PROJECT_NAME})

# Copie des ressources
add_custom_command(TARGET OrenjicGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:OrenjicGame>/resources
)

# Copie des données JSON
add_custom_command(TARGET OrenjicGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:OrenjicGame>/data
)

# Exemples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Copie des DLLs SFML
if(WIN32)
    add_custom_command(TARGET OrenjicGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system> $<TARGET_FILE:sfml-window> $<TARGET_FILE:sfml-graphics> $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE_DIR:OrenjicGame>
    )
    
    if(HAVE_TGUI)
        add_custom_command(TARGET OrenjicGame POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:tgui>
            $<TARGET_FILE_DIR:OrenjicGame>
        )
    endif()
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.hpp"
)

# Message de fin
message(STATUS "OrenjicEngine configuration complete. Build type: ${CMAKE_BUILD_TYPE}") 