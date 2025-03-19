#include <iostream>
#include <memory>
#include "game/MainMenu.h"
#include "game/GameState.h"
#include "game/TradingSystem.h"
#include "game/World.h"
#include "utils/JsonLoader.h"
#include "utils/GameData.h"

int main()
{
    try
    {
// Définir l'encodage pour les caractères spéciaux (Windows)
#ifdef _WIN32
        system("chcp 65001");
#endif

        std::cout << "Chargement du jeu..." << std::endl;

        // Initialize game data
        GameData::getInstance().loadAllData();

        // Initialize world
        World::getInstance().initialize();

        // Initialize trading system
        TradingSystem::getInstance().initialize();

        // Initialize game state
        GameState::getInstance().initialize();

        // Create main menu and run the game
        MainMenu mainMenu;
        mainMenu.run();

        std::cout << "Merci d'avoir joué !" << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour quitter..." << std::endl;
        std::cin.get();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERREUR: " << e.what() << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour quitter..." << std::endl;
        std::cin.get();
        return 1;
    }
}