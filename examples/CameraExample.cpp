#include <SFML/Graphics.hpp>
#include "../include/Core/Camera.hpp"
#include <iostream>
#include <vector>

// Exemple d'utilisation de la classe Camera
int main()
{
    // Créer une fenêtre SFML
    sf::VideoMode mode(800, 600);
    sf::RenderWindow window(mode, "Camera Example", sf::Style::Default);
    window.setFramerateLimit(60);

    // Créer une caméra avec la taille du monde (2000x1500)
    Core::Camera camera(window, sf::Vector2f(2000.0f, 1500.0f));

    // Créer un "joueur" qui sera suivi par la caméra
    sf::CircleShape player(20.0f);
    player.setFillColor(sf::Color::Red);
    player.setOrigin(sf::Vector2f(20.0f, 20.0f));
    player.setPosition(sf::Vector2f(400.0f, 300.0f));

    // Créer une grille de fond pour mieux visualiser les déplacements
    sf::VertexArray grid(sf::PrimitiveType::Lines);
    const int gridSize = 100;
    const int worldWidth = 2000;
    const int worldHeight = 1500;

    // Lignes horizontales
    for (int y = 0; y <= worldHeight; y += gridSize)
    {
        grid.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(50, 50, 50)));
        grid.append(sf::Vertex(sf::Vector2f(worldWidth, y), sf::Color(50, 50, 50)));
    }

    // Lignes verticales
    for (int x = 0; x <= worldWidth; x += gridSize)
    {
        grid.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(50, 50, 50)));
        grid.append(sf::Vertex(sf::Vector2f(x, worldHeight), sf::Color(50, 50, 50)));
    }

    // Créer des objets dans le monde pour référence visuelle
    std::vector<sf::RectangleShape> objects;

    for (int i = 0; i < 20; ++i)
    {
        sf::RectangleShape obj(sf::Vector2f(40.0f, 40.0f));
        obj.setFillColor(sf::Color(100, 100, 255));
        obj.setOrigin(sf::Vector2f(20.0f, 20.0f));
        obj.setPosition(sf::Vector2f(std::rand() % worldWidth, std::rand() % worldHeight));
        objects.push_back(obj);
    }

    // Définir la caméra pour suivre le joueur
    sf::Vector2f playerPos = player.getPosition();
    camera.follow(&playerPos);

    // Vitesse de déplacement du joueur
    float playerSpeed = 300.0f;

    // Horloge pour le temps delta
    sf::Clock clock;
    sf::Clock effectClock; // Pour déclencher des effets périodiquement

    // Variable pour activer/désactiver le suivi
    bool followingEnabled = true;

    // Boucle principale
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Touche Échap pour quitter
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }

            // Touche F pour activer/désactiver le suivi
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F)
            {
                followingEnabled = !followingEnabled;
                if (followingEnabled)
                {
                    camera.follow(&playerPos);
                    std::cout << "Camera following enabled" << std::endl;
                }
                else
                {
                    camera.stopFollowing();
                    std::cout << "Camera following disabled" << std::endl;
                }
            }

            // Touche Z pour zoomer
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
            {
                float currentZoom = camera.getZoom();
                camera.setZoom(currentZoom * 1.2f);
                std::cout << "Zoom: " << camera.getZoom() << std::endl;
            }

            // Touche X pour dézoomer
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
            {
                float currentZoom = camera.getZoom();
                camera.setZoom(currentZoom / 1.2f);
                std::cout << "Zoom: " << camera.getZoom() << std::endl;
            }

            // Touche S pour effet de secousse
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            {
                camera.addEffect(Core::Camera::Effect::Shake, 0.5f, 20.0f);
                std::cout << "Shake effect added" << std::endl;
            }

            // Touche P pour effet de panoramique
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
            {
                camera.addEffect(Core::Camera::Effect::Pan, 2.0f, 300.0f);
                std::cout << "Pan effect added" << std::endl;
            }
        }

        // Calcul du temps delta
        float deltaTime = clock.restart().asSeconds();

        // Mouvement du joueur avec les touches fléchées
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            playerPos.x -= playerSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            playerPos.x += playerSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            playerPos.y -= playerSpeed * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            playerPos.y += playerSpeed * deltaTime;
        }

        // Limiter la position du joueur aux limites du monde
        playerPos.x = std::max(0.0f, std::min(playerPos.x, (float)worldWidth));
        playerPos.y = std::max(0.0f, std::min(playerPos.y, (float)worldHeight));

        // Mettre à jour la position du joueur
        player.setPosition(playerPos);

        // Mettre à jour la caméra
        camera.update(deltaTime);

        // Ajouter un effet de zoom aléatoire toutes les 10 secondes
        if (effectClock.getElapsedTime().asSeconds() > 10.0f)
        {
            effectClock.restart();
            // Effet de zoom positif ou négatif
            float zoomIntensity = (std::rand() % 2 == 0) ? 0.5f : -0.5f;
            camera.addEffect(Core::Camera::Effect::Zoom, 1.0f, zoomIntensity);
            std::cout << "Random zoom effect added" << std::endl;
        }

        // Appliquer la caméra à la fenêtre avant de dessiner
        camera.apply();

        // Effacer l'écran
        window.clear(sf::Color(20, 20, 20));

        // Dessiner la grille
        window.draw(grid);

        // Dessiner les objets
        for (const auto &obj : objects)
        {
            window.draw(obj);
        }

        // Dessiner le joueur
        window.draw(player);

        // Afficher le résultat
        window.display();
    }

    return 0;
}