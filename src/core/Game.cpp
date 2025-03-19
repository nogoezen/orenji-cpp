#include "Game.h"
#include "../models/Player.h"
#include "World.h"
#include "TradingSystem.h"
#include "../data/GameData.h"
#include "../ui/MainMenu.h"
#include "../ui/TitleScreen.h"
#include "../ui/CharacterCreationMenu.h"
#include "../models/TradeGood.h"
#include "../game/MapScene.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

Game::Game()
{
    // L'initialisation complète se fait dans la méthode initialize()
}

Game::~Game()
{
    cleanup();
}

bool Game::initialize()
{
    if (m_initialized)
    {
        return true;
    }

    try
    {
        std::cout << "Initialisation du jeu..." << std::endl;

        // Charger les données du jeu
        GameData::getInstance().loadAllData();

        // Créer le monde
        m_world = std::make_shared<World>();

        // Créer le système de commerce
        m_tradingSystem = std::make_shared<TradingSystem>();

        m_initialized = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation du jeu: " << e.what() << std::endl;
        cleanup();
        return false;
    }
}

void Game::cleanup()
{
    // Libérer explicitement les ressources
    m_player.reset();
    m_world.reset();
    m_tradingSystem.reset();

    m_initialized = false;
    m_gameRunning = false;
}

void Game::startNewGame()
{
    clearScreen();
    std::cout << "=== NOUVEL EMBARQUEMENT ===" << std::endl;

    // Création du joueur
    m_player = std::make_shared<Player>();

    // Lancer le menu de création de personnage
    CharacterCreationMenu characterCreation(this);
    if (!characterCreation.run())
    {
        // Si l'utilisateur annule la création, on revient au menu principal
        std::cout << "Création de personnage annulée." << std::endl;
        waitForEnter("Appuyez sur Entrée pour revenir au menu principal...");
        return;
    }

    // Initialisation du monde
    m_world = std::make_shared<World>();
    if (!m_world->initialize())
    {
        std::cout << "Erreur lors de l'initialisation du monde." << std::endl;
        waitForEnter("Appuyez sur Entrée pour revenir au menu principal...");
        return;
    }

    // Initialisation du système de commerce
    m_tradingSystem = std::make_shared<TradingSystem>();
    m_tradingSystem->initialize();

    std::cout << "Nouveau jeu initialisé !" << std::endl;
    std::cout << "Bienvenue, " << m_player->getName() << " !" << std::endl;
    waitForEnter("Appuyez sur Entrée pour commencer votre voyage...");

    m_gameRunning = true;
    run();
}

bool Game::loadGame(const std::string &saveName)
{
    // Si un nom de sauvegarde est fourni, le stocker
    if (!saveName.empty())
    {
        m_saveFilePath = saveName;
    }

    // Utiliser l'implémentation directement au lieu d'appeler loadGame() sans paramètre
    try
    {
        // Vérifier si le fichier de sauvegarde existe
        if (!hasSaveGame())
        {
            std::cerr << "Aucune sauvegarde trouvée." << std::endl;
            return false;
        }

        // S'assurer que le jeu est initialisé
        if (!m_initialized && !initialize())
        {
            return false;
        }

        // Charger les données de sauvegarde
        std::ifstream file(m_saveFilePath);
        if (!file.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier de sauvegarde." << std::endl;
            return false;
        }

        nlohmann::json saveData;
        file >> saveData;
        file.close();

        // Créer un nouveau joueur à partir des données sauvegardées
        m_player = std::make_shared<Player>(saveData["player"]);

        // Initialiser le monde avec les données sauvegardées
        if (m_world)
        {
            m_world->initialize(saveData["world"]);
        }

        // Initialiser le système de commerce avec les données sauvegardées
        if (m_tradingSystem)
        {
            m_tradingSystem->initialize(saveData["trading"]);
        }

        m_gameRunning = true;
        m_currentState = GameState::Exploring;

        // Exécuter le jeu
        run();

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement de la partie: " << e.what() << std::endl;
        return false;
    }
}

bool Game::saveGame(const std::string &saveName)
{
    // Si un nom de sauvegarde est fourni, le stocker
    if (!saveName.empty())
    {
        m_saveFilePath = saveName;
    }

    try
    {
        // Vérifier que le jeu est en cours
        if (!m_gameRunning || !m_player)
        {
            std::cerr << "Aucune partie en cours à sauvegarder." << std::endl;
            return false;
        }

        // Créer l'objet de sauvegarde
        nlohmann::json saveData;

        // Sauvegarder les données du joueur
        saveData["player"] = m_player->toJson();

        // Sauvegarder les données du monde
        if (m_world)
        {
            saveData["world"] = m_world->toJson();
        }

        // Sauvegarder les données du système de commerce
        if (m_tradingSystem)
        {
            saveData["trading"] = m_tradingSystem->toJson();
        }

        // Écrire dans le fichier
        std::ofstream file(m_saveFilePath);
        if (!file.is_open())
        {
            std::cerr << "Impossible de créer le fichier de sauvegarde." << std::endl;
            return false;
        }

        file << saveData.dump(4); // Indentation de 4 espaces pour lisibilité
        file.close();

        std::cout << "Partie sauvegardée avec succès." << std::endl;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de la sauvegarde: " << e.what() << std::endl;
        return false;
    }
}

bool Game::hasSaveGame() const
{
    return std::filesystem::exists(m_saveFilePath);
}

void Game::run()
{
    if (!m_gameRunning)
    {
        return;
    }

    std::cout << "Démarrage du jeu..." << std::endl;

    // Boucle principale
    while (m_gameRunning)
    {
        clearScreen(); // Ajout pour effacer l'écran
        std::cout << "=== MODE JEU EN COURS DE DÉVELOPPEMENT ===" << std::endl;
        std::cout << "Joueur: " << m_player->getName() << std::endl;
        std::cout << "Or: " << m_player->getGold() << std::endl;
        std::cout << std::endl;

        // Menu temporaire du jeu
        std::cout << "1. Explorer (non implémenté)" << std::endl;
        std::cout << "2. Commerce (non implémenté)" << std::endl;
        std::cout << "3. Gestion de flotte (non implémenté)" << std::endl;
        std::cout << "4. Menu du personnage (non implémenté)" << std::endl;
        std::cout << "5. Retour au menu principal" << std::endl;

        std::cout << std::endl
                  << "Votre choix: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1: // Explorer
            std::cout << "Exploration non implémentée." << std::endl;
            waitForEnter();
            break;

        case 2: // Commerce
            std::cout << "Commerce non implémenté." << std::endl;
            waitForEnter();
            break;

        case 3: // Gestion de flotte
            std::cout << "Gestion de flotte non implémentée." << std::endl;
            waitForEnter();
            break;

        case 4: // Menu du personnage
            std::cout << "Menu du personnage non implémenté." << std::endl;
            waitForEnter();
            break;

        case 5: // Retour au menu principal
            endGame();
            break;

        default:
            std::cout << "Choix invalide!" << std::endl;
            waitForEnter();
            break;
        }
    }
}

// Utilitaire pour attendre l'entrée de l'utilisateur
void Game::waitForEnter(const std::string &message)
{
    std::cout << std::endl
              << message;
    std::cin.get();
}

void Game::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Game::endGame()
{
    std::cout << "Fin de la partie." << std::endl;
    m_gameRunning = false;
}

void Game::launchMainMenu()
{
    // Créer et initialiser l'écran titre
    m_titleScreen = std::make_shared<TitleScreen>(shared_from_this());

    // Lancer l'écran titre
    m_titleScreen->run();
}

void Game::launchMapScene(const std::string &mapFilePath)
{
    // Créer et initialiser la scène de carte
    m_mapScene = std::make_shared<MapScene>(shared_from_this(), mapFilePath);

    // Mettre à jour l'état du jeu
    m_currentState = GameState::MapView;

    // Lancer la scène de carte
    m_mapScene->run();

    // Une fois la scène terminée, revenir à l'état précédent ou au menu principal
    if (m_gameRunning)
    {
        m_currentState = GameState::Exploring;
    }
    else
    {
        returnToTitleScreen();
    }
}

void Game::returnToTitleScreen()
{
    // Créer et initialiser l'écran titre
    m_titleScreen = std::make_shared<TitleScreen>(shared_from_this());

    // Mettre à jour l'état du jeu
    m_currentState = GameState::MainMenu;

    // Lancer l'écran titre
    m_titleScreen->run();
}