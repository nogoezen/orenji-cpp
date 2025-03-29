#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>

int main()
{
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Moving Box with Collision");
    window.setFramerateLimit(60);

    // Création de notre rectangle
    sf::RectangleShape box(sf::Vector2f(50.0f, 50.0f));
    box.setFillColor(sf::Color(255, 128, 0));   // Orange
    box.setOutlineColor(sf::Color(128, 64, 0)); // Marron
    box.setOutlineThickness(2.0f);
    box.setPosition(sf::Vector2f(375.0f, 275.0f)); // Centre de la fenêtre

    // Variables pour le mouvement
    sf::Vector2f velocity(0.0f, 0.0f);
    float speed = 200.0f; // Vitesse du rectangle en pixels par seconde

    // Variables pour les collisions
    sf::Vector2f windowSize(static_cast<float>(window.getSize().x),
                            static_cast<float>(window.getSize().y));

    // Horloge pour le framerate
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Gestion des événements
        if (auto event = window.pollEvent())
        {
            // Fermeture de la fenêtre
            if (event->is<sf::Event::Closed>())
                window.close();

            // Touche Escape
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        // Mise à jour de la vitesse en fonction des touches pressées
        velocity = sf::Vector2f(0.0f, 0.0f); // Reset velocity

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            velocity.x = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            velocity.x = speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            velocity.y = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            velocity.y = speed;

        // Mise à jour de la position
        sf::Vector2f position = box.getPosition() + velocity * deltaTime;

        // Détection des collisions avec les bords de la fenêtre
        sf::FloatRect boxBounds = box.getGlobalBounds();
        if (position.x < 0)
            position.x = 0;
        if (position.y < 0)
            position.y = 0;
        if (position.x + boxBounds.size.x > windowSize.x)
            position.x = windowSize.x - boxBounds.size.x;
        if (position.y + boxBounds.size.y > windowSize.y)
            position.y = windowSize.y - boxBounds.size.y;

        // Application de la nouvelle position
        box.setPosition(position);

        // Rendu
        window.clear(sf::Color(40, 40, 40)); // Fond gris foncé
        window.draw(box);
        window.display();
    }

    return 0;
}