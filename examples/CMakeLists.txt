cmake_minimum_required(VERSION 3.10)
project(OrenjiExamples)

set(CMAKE_CXX_STANDARD 17)

# Trouver les packages nécessaires
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(tmxlite REQUIRED)
find_package(thor CONFIG REQUIRED)

# Chemins d'inclusion
include_directories(${SFML_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIRS})
include_directories(${TMXLITE_INCLUDE_DIR})
include_directories(${THOR_INCLUDE_DIR})

# Ajouter les dossiers sources de la bibliothèque Orenji
include_directories(../include)

# Définir les fichiers sources de la bibliothèque Orenji
file(GLOB_RECURSE ORENJI_SOURCES
    "../src/*.cpp"
)

# Créer la bibliothèque Orenji
add_library(Orenji STATIC ${ORENJI_SOURCES})

# Lier Orenji avec les dépendances
target_link_libraries(Orenji
    ${SFML_LIBRARIES}
    ${SFML_DEPENDENCIES}
    box2d::box2d
    tmxlite
    thor::thor
)

# Fonction pour ajouter facilement un exemple
function(add_orenji_example name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} Orenji)
    
    # Copier les DLLs nécessaires
    if(WIN32)
        # Pour Windows, copier les DLLs au dossier de sortie
        add_custom_command(TARGET ${name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:box2d::box2d>
                $<TARGET_FILE_DIR:${name}>
        )
        
        # Si tmxlite est une DLL, la copier aussi
        # add_custom_command(TARGET ${name} POST_BUILD
        #    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #        "${TMXLITE_ROOT}/bin/libtmxlite.dll"
        #        $<TARGET_FILE_DIR:${name}>
        # )
    endif()
endfunction()

# Ajouter les exemples
add_orenji_example(TiledMapExample TiledMapExample.cpp)
add_orenji_example(Box2DExample Box2DExample.cpp)
add_orenji_example(ParticleExample ParticleExample.cpp)

# Créer un dossier resources dans le dossier de sortie
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/../resources
    ${CMAKE_BINARY_DIR}/resources
    COMMENT "Copying resources..."
)

# S'assurer que les resources sont copiés avant d'exécuter les exemples
add_dependencies(TiledMapExample copy_resources)
add_dependencies(Box2DExample copy_resources)
add_dependencies(ParticleExample copy_resources)

# Ajouter un message de configuration
message(STATUS "Examples configuration complete")

# Add more examples here as needed 