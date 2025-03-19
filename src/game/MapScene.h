#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "TiledMap.h"

class Game;

/**
 * @brief Classe représentant une scène avec une carte Tiled
 */
class MapScene
{
private:
    // Référence vers le gestionnaire de jeu
    std::shared_ptr<Game> m_game;

    // Ressources SFML
    sf::RenderWindow m_window;

    // Carte Tiled
    std::unique_ptr<TiledMap> m_map;

    // Vue de la caméra
    sf::View m_camera;

    // Méthodes privées
    void initWindow();
    void handleEvents();
    void update(float deltaTime);
    void render();

    // Position du joueur sur la carte
    sf::Vector2f m_playerPosition;
    float m_playerSpeed;

public:
    /**
     * @brief Constructeur
     * @param game Référence partagée vers le gestionnaire de jeu
     * @param mapFilePath Chemin vers le fichier de carte Tiled
     */
    MapScene(std::shared_ptr<Game> game, const std::string &mapFilePath);

    /**
     * @brief Destructeur
     */
    ~MapScene();

    /**
     * @brief Exécute la scène
     */
    void run();
};