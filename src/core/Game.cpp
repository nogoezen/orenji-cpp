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
#include "../states/MainMenuState.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <SFML/Window/Event.hpp>

// Global pointer to the window for use by states
sf::RenderWindow *g_window = nullptr;

Game::Game()
    : m_running(false), m_targetFPS(60.0f)
{
    // L'initialisation complète se fait dans la méthode initialize()
}

Game::~Game()
{
    cleanup();
}

bool Game::initialize()
{
    // Initialize window
    initWindow();

    // Set the global window pointer
    g_window = m_window.get();

    // Initialize state manager
    m_stateManager = std::make_unique<StateManager>();

    // Initialize game states
    initStates();

    m_running = true;
    return true;
}

void Game::run()
{
    if (!m_window || !m_stateManager)
    {
        std::cerr << "Game not properly initialized" << std::endl;
        return;
    }

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.0f / m_targetFPS);

    while (m_running && m_window->isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            processInput();
            update(timePerFrame.asSeconds());
        }

        render();
    }
}

void Game::processInput()
{
    if (!m_stateManager->isEmpty())
    {
        m_stateManager->handleInput();
    }
}

void Game::update(float deltaTime)
{
    if (!m_stateManager->isEmpty())
    {
        m_stateManager->update(deltaTime);
    }
}

void Game::render()
{
    m_window->clear();

    if (!m_stateManager->isEmpty())
    {
        m_stateManager->render();
    }

    m_window->display();
}

void Game::initWindow()
{
    m_window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(800, 600),
        "Orenji - Uncharted Waters",
        sf::Style::Close);
    m_window->setFramerateLimit(static_cast<unsigned int>(m_targetFPS));
}

void Game::initStates()
{
    // Create and register states
    auto mainMenuState = m_stateManager->createState<MainMenuState>();

    // Push the initial state
    m_stateManager->pushState("MainMenu");
}

void Game::cleanup()
{
    if (m_stateManager)
    {
        m_stateManager->clearStates();
    }

    if (m_window && m_window->isOpen())
    {
        m_window->close();
    }

    g_window = nullptr;
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
    // Vérifier si le fichier de sauvegarde existe
    std::ifstream file(m_saveFilePath);
    return file.good();
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