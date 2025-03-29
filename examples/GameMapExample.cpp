#include <SFML/Graphics.hpp>
#include "../include/Resources/ResourceManager.hpp"
#include "../include/Gameplay/GameMap.hpp"
#include "../include/Physics/Box2DWrapper.hpp"
#include "../include/Resources/TiledMapLoader.hpp"
#include <iostream>

// Exemple d'utilisation de la classe GameMap
int main()
{
    // Créer une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode({800, 600}), "GameMap Example", sf::Style::Default);
    window.setFramerateLimit(60);

    // Créer le gestionnaire de ressources
    Resources::ResourceManager resourceManager;

    // Créer le système physique Box2D
    Physics::Box2DWrapper physics({0.0f, 9.8f}); // Gravité standard

    // Créer le chargeur de cartes Tiled
    Resources::TiledMapLoader mapLoader(resourceManager);

    // Créer la carte de jeu qui utilise le chargeur et le système physique
    Gameplay::GameMap gameMap(mapLoader, &physics);

    // Essayer de charger la carte d'exemple
    if (!gameMap.loadFromFile("examples/maps/sample_map.json", true))
    {
        std::cerr << "Failed to load map!" << std::endl;
        return -1;
    }

    std::cout << "Map loaded successfully!" << std::endl;

    // Obtenir les informations sur la carte
    sf::Vector2f mapSize = gameMap.getSize();
    sf::Vector2i tileSize = gameMap.getTileSize();

    std::cout << "Map size: " << mapSize.x << "x" << mapSize.y << std::endl;
    std::cout << "Tile size: " << tileSize.x << "x" << tileSize.y << std::endl;

    // Trouver le point de départ du joueur
    auto spawnPoints = gameMap.getObjectsByType("spawn");
    sf::Vector2f playerPos = {400.0f, 300.0f}; // Position par défaut

    if (!spawnPoints.empty())
    {
        playerPos = spawnPoints[0].position;
        std::cout << "Player start position: " << playerPos.x << ", " << playerPos.y << std::endl;
    }

    // Trouver les ennemis
    auto enemies = gameMap.getObjectsByType("enemy");
    std::cout << "Found " << enemies.size() << " enemies" << std::endl;

    for (const auto &enemy : enemies)
    {
        std::cout << "Enemy at: " << enemy.position.x << ", " << enemy.position.y << std::endl;
        // On pourrait créer des entités ennemies ici
    }

    // Créer une vue centrée sur la carte
    sf::View view(sf::FloatRect({0, 0}, {800, 600}));
    window.setView(view);

    // Créer une forme pour représenter le joueur
    sf::CircleShape playerShape(16.0f);
    playerShape.setFillColor(sf::Color::Red);
    playerShape.setOrigin({16.0f, 16.0f});
    playerShape.setPosition(playerPos);

    // Horloge pour le temps delta
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Gestion des événements pour SFML 3.0
        while (const auto event = window.pollEvent())
        {
            // Fermeture de la fenêtre
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Échap pour quitter
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }

        // Calcul du temps delta
        float deltaTime = clock.restart().asSeconds();

        // Mise à jour de la physique
        physics.step(deltaTime);

        // Mise à jour de la carte
        gameMap.update(deltaTime);

        // Effacer l'écran
        window.clear(sf::Color(64, 64, 64));

        // Dessiner la carte
        window.draw(gameMap);

        // Dessiner le joueur
        window.draw(playerShape);

        // Afficher le résultat
        window.display();
    }

    return 0;
}