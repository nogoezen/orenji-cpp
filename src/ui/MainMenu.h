#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

// Forward declarations to avoid circular includes
class Game;

/**
 * @brief Classe représentant le menu principal du jeu
 */
class MainMenu
{
private:
    // Référence vers le gestionnaire de jeu
    std::shared_ptr<Game> m_game;

    // Méthodes privées pour afficher les différentes sections du menu
    void displayLogo() const;
    void displayMenu(const std::string &title, const std::vector<std::string> &options) const;
    void displayError(const std::string &message) const;
    void displayTitle(const std::string &title) const;
    void displayWelcomeScreen();

    // Méthodes utilitaires
    void clearScreen() const;
    void waitForKeyPress() const;
    void waitForEnter(const std::string &message = "Appuyez sur Entrée pour continuer...") const;

    // Actions du menu
    void startNewGame();
    void continueGame();
    void showOptions();
    void showCredits();
    void exit();

public:
    /**
     * @brief Constructeur
     * @param game Référence partagée vers le gestionnaire de jeu
     */
    explicit MainMenu(std::shared_ptr<Game> game);

    /**
     * @brief Affiche le menu principal et gère les sélections de l'utilisateur
     */
    void run();

    /**
     * @brief Affiche le menu principal
     */
    void displayMainMenu();

    /**
     * @brief Traite le choix de l'utilisateur
     * @param choice Numéro du choix dans le menu
     * @return true si le menu doit continuer, false sinon
     */
    bool handleMenuChoice(int choice);
};