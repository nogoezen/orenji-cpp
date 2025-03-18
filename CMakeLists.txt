cmake_minimum_required(VERSION 3.10)
project(UnchartedWaters VERSION 1.0)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir des options spécifiques pour MinGW
if(MINGW)
    add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
    # Lier explicitement avec la bibliothèque filesystem
    link_libraries(stdc++fs)
endif()

# Fichiers sources
set(SOURCES
    src/main.cpp
    src/utils/JsonLoader.cpp
    src/utils/GameConfig.cpp
    src/utils/DataSystem.cpp
    src/utils/DataManager.cpp
    src/models/Player.cpp
    src/game/World.cpp
    src/game/LanguageManager.cpp
    src/game/GameData.cpp
    src/game/Game.cpp
)

# Créer l'exécutable
add_executable(UnchartedWaters ${SOURCES})

# Inclure les répertoires
target_include_directories(UnchartedWaters PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Définir le répertoire de sortie
set_target_properties(UnchartedWaters PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copier les fichiers de données
add_custom_command(TARGET UnchartedWaters POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/bin/data
)

# Afficher des messages utiles
message(STATUS "Répertoire d'inclusion: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "Vérification du fichier json.hpp: ${CMAKE_SOURCE_DIR}/include/nlohmann/json.hpp") 