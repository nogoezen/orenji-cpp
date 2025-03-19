#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <limits>

// Déclarations anticipées
class Player;
class World;
class GameData;
class TradingSystem;

/**
 * @brief Classe centrale du jeu qui gère la logique principale
 *
 * Cette classe sert de point d'entrée pour toutes les fonctionnalités du jeu
 * et coordonne les différents systèmes.
 */
class Game
{
private:
    // État du jeu
    bool m_initialized = false;
    bool m_gameRunning = false;

    // Composants principaux
    std::shared_ptr<Player> m_player;
    std::shared_ptr<World> m_world;
    std::shared_ptr<TradingSystem> m_tradingSystem;

    // Chemin du fichier de sauvegarde
    std::string m_saveFilePath = "save.json";

    // Méthodes privées
    bool initialize();
    void cleanup();

    // Méthodes utilitaires
    void clearScreen();
    void waitForEnter(const std::string &message = "Appuyez sur ENTRÉE pour continuer...");

    // État du jeu
    enum class GameState
    {
        MainMenu,
        Exploring,
        Trading,
        ShipManagement,
        CharacterMenu,
        Paused
    };

    GameState m_currentState = GameState::MainMenu;

public:
    /**
     * Constructeur
     */
    Game();

    /**
     * Destructeur
     */
    ~Game();

    /**
     * Démarre une nouvelle partie
     * @return true si le jeu a démarré avec succès
     */
    bool startNewGame();

    /**
     * Charge une partie existante
     * @return true si le jeu a été chargé avec succès
     */
    bool loadGame();

    /**
     * Sauvegarde la partie en cours
     * @return true si le jeu a été sauvegardé avec succès
     */
    bool saveGame();

    /**
     * Vérifie si une sauvegarde existe
     * @return true si une sauvegarde valide existe
     */
    bool hasSaveGame() const;

    /**
     * Exécute la boucle principale du jeu
     */
    void run();

    /**
     * Met fin à la partie en cours
     */
    void endGame();

    /**
     * Obtient le joueur actuel
     * @return référence partagée vers le joueur
     */
    std::shared_ptr<Player> getPlayer() const { return m_player; }

    /**
     * Obtient le monde de jeu
     * @return référence partagée vers le monde
     */
    std::shared_ptr<World> getWorld() const { return m_world; }

    /**
     * Obtient le système de commerce
     * @return référence partagée vers le système de commerce
     */
    std::shared_ptr<TradingSystem> getTradingSystem() const { return m_tradingSystem; }
};