#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <limits>
#include "../models/Player.h"
#include "../models/World.h"
#include "TradingSystem.h"

// Forward declaration pour éviter les inclusions circulaires
class MainMenu;

/**
 * @brief Énumération des différents états possibles du jeu
 */
enum class GameState
{
    MainMenu,
    Exploring,
    Trading,
    Port,
    Combat,
    Paused,
    GameOver
};

/**
 * @brief Classe principale gérant le jeu
 */
class Game
{
private:
    // Composants de jeu
    std::shared_ptr<Player> m_player;
    std::shared_ptr<World> m_world;
    std::shared_ptr<TradingSystem> m_tradingSystem;
    std::shared_ptr<MainMenu> m_mainMenu;

    // État du jeu
    bool m_initialized = false;
    bool m_gameRunning = false;
    GameState m_currentState = GameState::MainMenu;

    // Utilitaires
    void clearScreen();
    void waitForEnter(const std::string &message = "Appuyez sur Entrée pour continuer...");

public:
    /**
     * @brief Constructeur par défaut
     */
    Game();

    /**
     * @brief Destructeur
     */
    ~Game();

    /**
     * @brief Initialise le jeu
     * @return true si l'initialisation a réussi, false sinon
     */
    bool initialize();

    /**
     * @brief Exécute le jeu
     */
    void run();

    /**
     * @brief Lance le menu principal
     */
    void launchMainMenu();

    /**
     * @brief Démarre une nouvelle partie
     */
    void startNewGame();

    /**
     * @brief Charge une partie sauvegardée
     * @param saveName Nom de la sauvegarde à charger
     * @return true si le chargement a réussi, false sinon
     */
    bool loadGame(const std::string &saveName);

    /**
     * @brief Sauvegarde la partie actuelle
     * @param saveName Nom de la sauvegarde
     * @return true si la sauvegarde a réussi, false sinon
     */
    bool saveGame(const std::string &saveName);

    /**
     * @brief Termine le jeu actuel
     */
    void endGame();

    /**
     * @brief Quitte l'application
     */
    void quit();

    // Accesseurs
    inline bool isRunning() const { return m_gameRunning; }
    inline bool isInitialized() const { return m_initialized; }
    inline GameState getCurrentState() const { return m_currentState; }

    inline std::shared_ptr<Player> getPlayer() { return m_player; }
    inline std::shared_ptr<World> getWorld() { return m_world; }
    inline std::shared_ptr<TradingSystem> getTradingSystem() { return m_tradingSystem; }
    inline void setMainMenu(std::shared_ptr<MainMenu> mainMenu) { m_mainMenu = mainMenu; }

    // Mutateurs
    inline void setGameRunning(bool running) { m_gameRunning = running; }
    inline void setCurrentState(GameState state) { m_currentState = state; }
};