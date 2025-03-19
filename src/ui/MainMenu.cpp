#include "MainMenu.h"
#include "../core/Game.h"
#include "../utils/FileSystem.h"
#include <iostream>
#include <limits>

MainMenu::MainMenu(std::shared_ptr<Game> game)
    : m_game(game)
{
}

void MainMenu::run()
{
    bool running = true;
    int choice;

    while (running)
    {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        running = handleMenuChoice(choice);
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
    clearScreen();

    displayLogo();

    std::cout << "=== MENU PRINCIPAL ===" << std::endl
              << std::endl;
    std::cout << "1. Nouvelle partie" << std::endl;

    // Option "Continuer partie" disponible uniquement si une sauvegarde existe
    if (m_game && FileSystem::fileExists("bin/save.json"))
    {
        std::cout << "2. Continuer partie" << std::endl;
    }
    else
    {
        std::cout << "2. Continuer partie (non disponible)" << std::endl;
    }

    std::cout << "3. Options" << std::endl;
    std::cout << "4. Crédits" << std::endl;
    std::cout << "5. Quitter" << std::endl;

    std::cout << std::endl
              << "Votre choix: ";
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
    clearScreen();
    std::cout << "=== CONTINUER PARTIE ===" << std::endl
              << std::endl;

    // Vérifier que le jeu est initialisé
    if (m_game)
    {
        if (FileSystem::fileExists("bin/save.json"))
        {
            if (!m_game->loadGame("bin/save.json"))
            {
                std::cout << "Erreur: Impossible de charger la partie sauvegardée." << std::endl;
                waitForKeyPress();
                return;
            }

            std::cout << "Partie chargée !" << std::endl;
            std::cout << "Bienvenue, " << m_game->getPlayer()->getName() << " !" << std::endl;
            waitForKeyPress();

            // Lancer le jeu
            m_game->run();
        }
        else
        {
            std::cout << "Aucune partie sauvegardée trouvée." << std::endl;
            waitForKeyPress();
        }
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
    std::cout << "=== AU REVOIR ===" << std::endl
              << std::endl;
    std::cout << "Merci d'avoir joué!" << std::endl;
}

void MainMenu::clearScreen() const
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MainMenu::waitForKeyPress() const
{
    std::cout << "Appuyez sur Entrée pour continuer...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MainMenu::displayLogo() const
{
    std::cout << " _    _            _                _           _  " << std::endl;
    std::cout << "| |  | |          | |              | |         | | " << std::endl;
    std::cout << "| |  | |_ __   ___| |__   __ _ _ __| |_ ___  __| | " << std::endl;
    std::cout << "| |  | | '_ \\ / __| '_ \\ / _` | '__| __/ _ \\/ _` | " << std::endl;
    std::cout << "| |__| | | | | (__| | | | (_| | |  | ||  __/ (_| | " << std::endl;
    std::cout << " \\____/|_| |_|\\___|_| |_|\\__,_|_|   \\__\\___|\\__,_| " << std::endl;
    std::cout << "                                                   " << std::endl;
    std::cout << "                  W A T E R S                      " << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << std::endl;
}

void MainMenu::displayMenu(const std::string &title, const std::vector<std::string> &options) const
{
    clearScreen();
    displayLogo();

    std::cout << "=== " << title << " ===" << std::endl
              << std::endl;

    for (size_t i = 0; i < options.size(); i++)
    {
        std::cout << (i + 1) << ". " << options[i] << std::endl;
    }

    std::cout << std::endl
              << "Votre choix: ";
}

void MainMenu::displayError(const std::string &message) const
{
    std::cout << "\nERREUR: " << message << std::endl;
}

void MainMenu::displayTitle(const std::string &title) const
{
    clearScreen();
    displayLogo();
    std::cout << "=== " << title << " ===" << std::endl
              << std::endl;
}

void MainMenu::waitForEnter(const std::string &message) const
{
    std::cout << std::endl
              << message;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool MainMenu::handleMenuChoice(int choice)
{
    switch (choice)
    {
    case 1:
        startNewGame();
        return true;
    case 2:
        if (FileSystem::fileExists("bin/save.json"))
        {
            continueGame();
        }
        else
        {
            std::cout << "Option non disponible." << std::endl;
            waitForKeyPress();
        }
        return true;
    case 3:
        showOptions();
        return true;
    case 4:
        showCredits();
        return true;
    case 5:
        exit();
        return false;
    default:
        std::cout << "Choix invalide. Réessayez." << std::endl;
        waitForKeyPress();
        return true;
    }
}