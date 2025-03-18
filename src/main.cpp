#include <iostream>
#include <stdexcept>
#include "game/Game.h"

int main() {
    try {
        // Définir l'encodage pour les caractères spéciaux (Windows)
        #ifdef _WIN32
            system("chcp 65001");
        #endif
        
        // Créer et démarrer le jeu
        Game game;
        game.start();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERREUR FATALE: " << e.what() << std::endl;
        return 1;
    }
} 