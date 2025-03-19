#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Game;

/**
 * @brief Classe représentant l'écran titre du jeu avec un fond graphique
 */
class TitleScreen
{
private:
    // Référence vers le gestionnaire de jeu
    std::shared_ptr<Game> m_game;

    // Ressources SFML
    sf::RenderWindow m_window;
    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::Font m_font;

    // Éléments du menu
    std::vector<sf::Text> m_menuItems;
    int m_selectedIndex;

    // État de l'écran titre
    bool m_isStartScreen;
    sf::Text m_pressAnyKeyText;
    float m_textAlpha;
    bool m_fadeIn;

    // Méthodes privées
    void initWindow();
    void loadResources();
    void createMenuItems();
    void createStartScreen();
    void handleEvents();
    void update();
    void render();

    // Actions du menu
    void startNewGame();
    void continueGame();
    void testTiledMap();
    void showOptions();
    void showCredits();
    void exit();

public:
    /**
     * @brief Constructeur
     * @param game Référence partagée vers le gestionnaire de jeu
     */
    explicit TitleScreen(std::shared_ptr<Game> game);

    /**
     * @brief Destructeur
     */
    ~TitleScreen();

    /**
     * @brief Affiche l'écran titre et gère les interactions de l'utilisateur
     */
    void run();

    /**
     * @brief Vérifie si une sauvegarde existe
     */
    bool hasSaveGame() const;
};