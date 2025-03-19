#include <iostream>
#include <memory>
#include "core/Game.h"
#include "ui/MainMenu.h"
#include "data/GameData.h"

int main()
{
    try
    {
        // Définir l'encodage pour les caractères spéciaux (Windows)
#ifdef _WIN32
        system("chcp 65001");
#endif

        std::cout << "Chargement du jeu..." << std::endl;

        // Créer l'instance du jeu
        auto game = std::make_shared<Game>();

        // Initialiser le jeu
        if (!game->initialize())
        {
            std::cerr << "Erreur lors de l'initialisation du jeu" << std::endl;
            return 1;
        }

        // Lancer le menu principal graphique
        game->launchMainMenu();

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