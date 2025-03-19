#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <limits>
#include "../models/Player.h"
#include "World.h"
#include "TradingSystem.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "StateManager.h"

// Forward declarations pour éviter les inclusions circulaires
class MainMenu;
class TitleScreen;
class MapScene;

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
    GameOver,
    MapView // Nouvel état pour l'affichage de carte
};

/**
 * @brief Classe principale gérant le jeu
 */
class Game
{
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
     * @param saveName Nom de la sauvegarde à charger (optionnel)
     * @return true si le chargement a réussi, false sinon
     */
    bool loadGame(const std::string &saveName = "");

    /**
     * @brief Sauvegarde la partie en cours
     * @param saveName Nom de la sauvegarde (optionnel)
     * @return true si la sauvegarde a réussi, false sinon
     */
    bool saveGame(const std::string &saveName = "");

    /**
     * @brief Vérifie si une sauvegarde existe
     * @return true si une sauvegarde existe, false sinon
     */
    bool hasSaveGame() const;

    /**
     * @brief Vérifie si le jeu est en cours d'exécution
     * @return true si le jeu est en cours d'exécution, false sinon
     */
    bool isRunning() const { return m_running; }

    /**
     * @brief Obtient l'état actuel du jeu
     * @return L'état actuel du jeu
     */
    GameState getCurrentState() const { return m_currentState; }

    /**
     * @brief Définit l'état actuel du jeu
     * @param state Nouvel état du jeu
     */
    void setCurrentState(GameState state) { m_currentState = state; }

    /**
     * @brief Termine le jeu
     */
    void endGame();

    /**
     * @brief Lance une scène avec une carte Tiled
     * @param mapFilePath Chemin vers le fichier de carte Tiled
     */
    void launchMapScene(const std::string &mapFilePath);

    /**
     * @brief Retourne à l'écran titre
     */
    void returnToTitleScreen();

    // Access methods
    sf::RenderWindow &getWindow() { return *m_window; }
    StateManager &getStateManager() { return *m_stateManager; }

private:
    // Composants de jeu
    std::shared_ptr<Player> m_player;
    std::shared_ptr<World> m_world;
    std::shared_ptr<TradingSystem> m_tradingSystem;
    std::shared_ptr<MainMenu> m_mainMenu;
    std::shared_ptr<TitleScreen> m_titleScreen;
    std::shared_ptr<MapScene> m_mapScene;

    // État du jeu
    bool m_initialized = false;
    bool m_running = false;
    GameState m_currentState = GameState::MainMenu;

    // Chemin du fichier de sauvegarde
    std::string m_saveFilePath = "bin/save.json";

    // Game loop methods
    void processInput();
    void update(float deltaTime);
    void render();

    // Init methods
    void initWindow();
    void initStates();

    // Game window and states
    std::unique_ptr<sf::RenderWindow> m_window;
    std::unique_ptr<StateManager> m_stateManager;

    // Game loop control
    float m_targetFPS;

    // Utilitaires
    void clearScreen();
    void waitForEnter(const std::string &message = "Appuyez sur Entrée pour continuer...");

    // Nettoyage des ressources
    void cleanup();

    // Accesseurs
    inline bool isInitialized() const { return m_initialized; }
    inline std::shared_ptr<Player> getPlayer() { return m_player; }
    inline std::shared_ptr<World> getWorld() { return m_world; }
    inline std::shared_ptr<TradingSystem> getTradingSystem() { return m_tradingSystem; }
    inline void setMainMenu(std::shared_ptr<MainMenu> mainMenu) { m_mainMenu = mainMenu; }
    inline void setTitleScreen(std::shared_ptr<TitleScreen> titleScreen) { m_titleScreen = titleScreen; }
    inline std::shared_ptr<TitleScreen> getTitleScreen() { return m_titleScreen; }
    inline std::shared_ptr<MapScene> getMapScene() { return m_mapScene; }

    // Mutateurs
    inline void setRunning(bool running) { m_running = running; }
};