# Fichier CMakeLists.txt pour les exemples
# Ce fichier définit comment compiler les exemples de particules

# Liste des exemples à compiler
set(EXAMPLES
    ParticleExample
    SimpleParticleDemo
)

# Créer un exécutable pour chaque exemple
foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE} PRIVATE OrenjicEngine)
    
    # Copie des ressources pour chaque exemple
    add_custom_command(TARGET ${EXAMPLE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../resources $<TARGET_FILE_DIR:${EXAMPLE}>/resources
    )
    
    # Copie des DLLs SFML pour Windows
    if(WIN32)
        add_custom_command(TARGET ${EXAMPLE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-system> $<TARGET_FILE:sfml-window> $<TARGET_FILE:sfml-graphics> $<TARGET_FILE:sfml-audio>
            $<TARGET_FILE_DIR:${EXAMPLE}>
        )
        
        if(HAVE_TGUI)
            add_custom_command(TARGET ${EXAMPLE} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:tgui>
                $<TARGET_FILE_DIR:${EXAMPLE}>
            )
        endif()
    endif()
endforeach()

# Message de confirmation
message(STATUS "Configured ${EXAMPLES} examples") 