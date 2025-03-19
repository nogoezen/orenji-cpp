#pragma once

#include "ConsoleUI.h"
#include <memory>

// Déclarations anticipées
class Game;

/**
 * @brief Menu principal du jeu
 *
 * Cette classe gère l'interface utilisateur du menu principal et coordonne
 * les différentes sections du jeu.
 */
class MainMenu : public ConsoleUI
{
private:
    // Référence vers le gestionnaire de jeu
    std::shared_ptr<Game> m_game;

    // État du menu
    bool m_running = false;

    // Actions de menu
    void startNewGame();
    void continueGame();
    void showOptions();
    void showCredits();
    void exit();

    // Utilitaires d'interface
    void displayWelcomeScreen();
    void displayMainMenu();

public:
    /**
     * Constructeur
     * @param game Référence partagée vers le gestionnaire de jeu
     */
    explicit MainMenu(std::shared_ptr<Game> game);

    /**
     * Exécute le menu principal
     */
    void run();
};