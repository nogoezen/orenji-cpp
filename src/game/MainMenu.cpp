#include "MainMenu.h"
#include <iostream>
#include <iomanip>
#include "CharacterEditor.h"
#include "FleetManager.h"

MainMenu::MainMenu()
{
    // Constructor
}

void MainMenu::run()
{
    // Initialize the game state
    GameState::getInstance().initialize();

    // Main game loop
    while (GameState::getInstance().isRunning())
    {
        system("cls"); // Clear screen

        // Show the main menu
        showMainMenu();
    }

    // Game exit message
    std::cout << "Merci d'avoir joué à Uncharted Waters !" << std::endl;
}

void MainMenu::showMainMenu()
{
    // Get current game state
    auto &gameState = GameState::getInstance();
    auto player = gameState.getPlayer();

    if (!player)
    {
        std::cout << "Erreur : Joueur non initialisé." << std::endl;
        gameState.quitGame();
        return;
    }

    // Display game title
    std::cout << "=== UNCHARTED WATERS ===" << std::endl;
    std::cout << "Jour " << gameState.getCurrentDay() << std::endl;

    // Get current location name
    int locationId = gameState.getCurrentLocationId();
    std::string locationName = "Inconnu";
    auto location = World::getInstance().findCityById(locationId);
    if (location)
    {
        locationName = location->name;
    }

    std::cout << "Location actuelle : " << locationName << std::endl;
    std::cout << "Or : " << player->getGold() << " pièces" << std::endl;
    std::cout << std::endl;

    // Menu options
    std::vector<std::string> options = {
        "Explorer la ville",
        "Voyager vers une autre ville",
        "Gérer votre flotte",
        "Voir votre statut",
        "Options",
        "Quitter le jeu"};

    int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

    switch (choice)
    {
    case 0: // City menu
        showCityMenu();
        break;

    case 1: // Travel
        showTravelMenu();
        break;

    case 2: // Fleet management
        showFleetMenu();
        break;

    case 3: // Player status
        showPlayerStatusMenu();
        break;

    case 4: // Options
        showOptionsMenu();
        break;

    case 5: // Quit
        std::cout << "Êtes-vous sûr de vouloir quitter ? (O/N) : ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(); // Clear input buffer

        if (confirm == 'O' || confirm == 'o')
        {
            gameState.quitGame();
        }
        break;

    default:
        break;
    }
}

void MainMenu::showCityMenu()
{
    // Get current game state
    auto &gameState = GameState::getInstance();
    auto player = gameState.getPlayer();
    int locationId = gameState.getCurrentLocationId();

    // Get location info
    auto location = World::getInstance().findCityById(locationId);
    if (!location)
    {
        std::cout << "Erreur : Lieu actuel invalide." << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear screen

        // Display city title
        std::cout << "=== VILLE DE " << location->name << " ===" << std::endl;
        std::cout << "Population : " << location->population << std::endl;
        std::cout << std::endl;

        // Menu options
        std::vector<std::string> options = {
            "Commerce (achat/vente de marchandises)",
            "Taverne (informations et missions)",
            "Chantier naval (achat/réparation de navires)",
            "Auberge (repos et passage du temps)",
            "Retour au menu principal"};

        int choice = m_ui.displayMenu("Où souhaitez-vous aller ?", options);

        switch (choice)
        {
        case 0: // Trading
        {
            // Create trading UI and show menu
            TradingUI tradingUI(*player, m_ui);
            tradingUI.showTradingMenu(locationId);
            break;
        }

        case 1: // Tavern
            std::cout << "La taverne n'est pas encore implémentée." << std::endl;
            std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
            std::cin.get();
            break;

        case 2: // Shipyard
            std::cout << "Le chantier naval n'est pas encore implémenté." << std::endl;
            std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
            std::cin.get();
            break;

        case 3: // Inn
            // Rest at the inn and advance time
            std::cout << "Combien de jours souhaitez-vous vous reposer ? (0 pour annuler) : ";
            int days;
            std::cin >> days;
            std::cin.ignore(); // Clear input buffer

            if (days > 0)
            {
                // Calculate cost (10 gold per day)
                int cost = days * 10;

                // Check if player has enough gold
                if (player->getGold() < cost)
                {
                    std::cout << "Vous n'avez pas assez d'or pour vous reposer " << days << " jours." << std::endl;
                }
                else
                {
                    // Confirm
                    std::cout << "Se reposer " << days << " jours coûtera " << cost << " or." << std::endl;
                    std::cout << "Confirmer ? (O/N) : ";
                    char confirm;
                    std::cin >> confirm;
                    std::cin.ignore(); // Clear input buffer

                    if (confirm == 'O' || confirm == 'o')
                    {
                        // Pay and rest
                        player->setGold(player->getGold() - cost);
                        gameState.advanceTime(days);
                        std::cout << "Vous vous êtes reposé " << days << " jours." << std::endl;
                    }
                }

                std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
                std::cin.get();
            }
            break;

        case 4: // Exit
            exitMenu = true;
            break;

        default:
            break;
        }
    }
}

void MainMenu::showTravelMenu()
{
    // Get current game state
    auto &gameState = GameState::getInstance();
    auto player = gameState.getPlayer();
    int currentLocationId = gameState.getCurrentLocationId();

    system("cls"); // Clear screen

    // Display travel title
    std::cout << "=== VOYAGE ===" << std::endl;

    // Get all cities
    const auto &cities = World::getInstance().getCities();

    // Create options list excluding current city
    std::vector<std::string> options;
    std::vector<int> cityIds;

    for (const auto &city : cities)
    {
        if (city.id != currentLocationId)
        {
            // Format option text with distance
            auto currentLocation = World::getInstance().findCityById(currentLocationId);
            std::string optionText = city.name;

            if (currentLocation)
            {
                float dx = static_cast<float>(city.posX - currentLocation->posX);
                float dy = static_cast<float>(city.posY - currentLocation->posY);
                float distance = std::sqrt(dx * dx + dy * dy);

                optionText += " (Distance: " + std::to_string(static_cast<int>(distance)) + ")";
            }

            options.push_back(optionText);
            cityIds.push_back(city.id);
        }
    }

    options.push_back("Retour au menu principal");

    int choice = m_ui.displayMenu("Choisissez une destination :", options);

    if (choice >= 0 && choice < static_cast<int>(cityIds.size()))
    {
        // Travel to selected city
        gameState.travelToLocation(cityIds[choice]);
    }
}

void MainMenu::showPlayerStatusMenu()
{
    // Get current game state
    auto &gameState = GameState::getInstance();
    auto player = gameState.getPlayer();

    system("cls"); // Clear screen

    // Display player status title
    std::cout << "=== STATUT DU JOUEUR ===" << std::endl;

    // Player info
    std::cout << "Nom : " << player->getName() << std::endl;
    std::cout << "Or : " << player->getGold() << " pièces" << std::endl;
    std::cout << "Niveau : " << player->getLevel() << std::endl;
    std::cout << "Expérience : " << player->getExperience() << std::endl;
    std::cout << std::endl;

    // Character info
    auto character = player->getCharacter();
    if (character)
    {
        std::cout << "Personnage : " << character->getName() << std::endl;
        std::cout << "Classe : " << character->getClassName() << std::endl;

        // Display attributes
        const auto &attributes = character->getAttributes();
        std::cout << "Attributs :" << std::endl;
        for (const auto &[name, value] : attributes)
        {
            std::cout << "  " << name << ": " << value << std::endl;
        }

        std::cout << std::endl;
    }

    // Fleet info
    size_t fleetSize = player->getFleetSize();
    std::cout << "Taille de la flotte : " << fleetSize << " navires" << std::endl;

    if (fleetSize > 0)
    {
        auto flagship = player->getFlagship();
        if (flagship)
        {
            std::cout << "Navire amiral : " << flagship->getName() << std::endl;
            std::cout << "Équipage : " << flagship->getCurrentCrew() << "/" << flagship->getMaxCrew() << std::endl;
        }
    }

    std::cout << std::endl;

    // Cargo info
    const auto &cargo = player->getCargo();
    if (!cargo.empty())
    {
        std::cout << "Cargaison :" << std::endl;
        for (const auto &item : cargo)
        {
            std::cout << "  " << item.name << ": " << item.quantity << std::endl;
        }
    }
    else
    {
        std::cout << "Cargaison : Vide" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
    std::cin.get();
}

void MainMenu::showOptionsMenu()
{
    system("cls"); // Clear screen

    // Display options title
    std::cout << "=== OPTIONS ===" << std::endl;

    // Options menu
    std::vector<std::string> options = {
        "Changer le nom du joueur",
        "Réinitialiser le jeu",
        "Crédits",
        "Retour au menu principal"};

    int choice = m_ui.displayMenu("Options :", options);

    switch (choice)
    {
    case 0: // Change player name
    {
        auto &gameState = GameState::getInstance();
        auto player = gameState.getPlayer();

        std::cout << "Nom actuel : " << player->getName() << std::endl;
        std::cout << "Nouveau nom : ";
        std::string newName;
        std::getline(std::cin, newName);

        if (!newName.empty())
        {
            player->setName(newName);
            std::cout << "Nom changé avec succès." << std::endl;
        }

        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        break;
    }

    case 1: // Reset game
        std::cout << "Êtes-vous sûr de vouloir réinitialiser le jeu ? Toute progression sera perdue. (O/N) : ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore(); // Clear input buffer

        if (confirm == 'O' || confirm == 'o')
        {
            // Reinitialize game state
            GameState::getInstance().initialize();
            std::cout << "Jeu réinitialisé." << std::endl;
        }

        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        break;

    case 2:            // Credits
        system("cls"); // Clear screen
        std::cout << "=== CRÉDITS ===" << std::endl;
        std::cout << "Uncharted Waters - Clone" << std::endl;
        std::cout << "Développé par [Votre Nom]" << std::endl;
        std::cout << "Version 0.1" << std::endl;
        std::cout << std::endl;
        std::cout << "Inspiré par la série Uncharted Waters (Koei)" << std::endl;
        std::cout << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        break;

    default:
        break;
    }
}

void MainMenu::showFleetMenu()
{
    // Get current game state
    auto &gameState = GameState::getInstance();
    auto player = gameState.getPlayer();

    // Create fleet manager
    FleetManager fleetManager(*player, m_ui);

    system("cls"); // Clear screen

    // Display fleet management title
    std::cout << "=== GESTION DE LA FLOTTE ===" << std::endl;

    // Fleet menu options
    std::vector<std::string> options = {
        "Voir les navires de la flotte",
        "Acheter un nouveau navire",
        "Vendre un navire",
        "Réparer un navire",
        "Améliorer un navire",
        "Engager des membres d'équipage",
        "Retour au menu principal"};

    int choice = m_ui.displayMenu("Gestion de la flotte :", options);

    switch (choice)
    {
    case 0: // View fleet
        fleetManager.displayFleet();
        break;

    case 1: // Buy new ship
        fleetManager.buyNewShip();
        break;

    case 2: // Sell ship
        fleetManager.sellShip();
        break;

    case 3: // Repair ship
        fleetManager.repairShip();
        break;

    case 4: // Upgrade ship
        fleetManager.upgradeShip();
        break;

    case 5: // Hire crew
        fleetManager.hireCrew();
        break;

    default:
        // Return to main menu
        break;
    }
}