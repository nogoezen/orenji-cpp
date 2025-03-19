#include "MainMenu.h"
#include "../core/Game.h"
#include <iostream>

MainMenu::MainMenu(std::shared_ptr<Game> game)
    : m_game(game)
{
}

void MainMenu::run()
{
    m_running = true;

    displayWelcomeScreen();

    while (m_running)
    {
        displayMainMenu();
    }
}

void MainMenu::displayWelcomeScreen()
{
    clearScreen();

    std::cout << R"(
    _    _            _                _           _  __        __    _               
   | |  | |          | |              | |         | | \ \      / /   | |              
   | |  | |_ __   ___| |__   __ _ _ __| |_ ___  __| |  \ \    / /__ _| |_ ___ _ __ ___
   | |  | | '_ \ / __| '_ \ / _` | '__| __/ _ \/ _` |   \ \  / / __| | __/ _ \ '__/ __|
   | |__| | | | | (__| | | | (_| | |  | ||  __/ (_| |    \ \/ /\__ \ | ||  __/ |  \__ \
    \____/|_| |_|\___|_| |_|\__,_|_|   \__\___|\__,_|     \__/ |___/_|\__\___|_|  |___/
                                                                                      
    )" << std::endl;

    std::cout << "\n\n                       Un jeu de commerce maritime" << std::endl;
    std::cout << "\n\n                    Appuyez sur ENTRÉE pour continuer..." << std::endl;

    std::cin.get();
}

void MainMenu::displayMainMenu()
{
    std::vector<MenuItem> menuItems = {
        {"Nouvelle partie", [this]()
         { startNewGame(); }},
        {"Continuer partie", [this]()
         { continueGame(); }, false}, // Désactivé par défaut
        {"Options", [this]()
         { showOptions(); }},
        {"Crédits", [this]()
         { showCredits(); }},
        {"Quitter", [this]()
         { exit(); }}};

    // Activer l'option Continuer s'il y a une sauvegarde valide
    if (m_game && m_game->hasSaveGame())
    {
        menuItems[1].enabled = true;
    }

    displayMenu("MENU PRINCIPAL", menuItems);
}

void MainMenu::startNewGame()
{
    if (m_game)
    {
        m_game->startNewGame();
    }
    else
    {
        displayError("Erreur: Gestionnaire de jeu non initialisé");
        waitForEnter();
    }
}

void MainMenu::continueGame()
{
    if (m_game)
    {
        if (!m_game->loadGame())
        {
            displayError("Erreur: Impossible de charger la partie");
            waitForEnter();
        }
    }
    else
    {
        displayError("Erreur: Gestionnaire de jeu non initialisé");
        waitForEnter();
    }
}

void MainMenu::showOptions()
{
    // À implémenter: menu des options
    displayTitle("OPTIONS");
    std::cout << "Cette fonctionnalité n'est pas encore implémentée." << std::endl;
    waitForEnter();
}

void MainMenu::showCredits()
{
    displayTitle("CRÉDITS");

    std::cout << "UNCHARTED WATERS - Clone" << std::endl;
    std::cout << "Un jeu de commerce maritime inspiré par la série Uncharted Waters (Koei)." << std::endl;
    std::cout << std::endl;

    std::cout << "Développé par:" << std::endl;
    std::cout << "  - Votre nom ici" << std::endl;
    std::cout << std::endl;

    std::cout << "Remerciements:" << std::endl;
    std::cout << "  - Koei pour la série originale" << std::endl;
    std::cout << "  - nlohmann/json pour la bibliothèque JSON" << std::endl;

    waitForEnter();
}

void MainMenu::exit()
{
    clearScreen();
    std::cout << "Merci d'avoir joué!" << std::endl;
    m_running = false;
}