#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "SFML Simple Window");
    window.setFramerateLimit(60);

    // Boucle principale
    while (window.isOpen())
    {
        // Gestion des événements
        if (auto event = window.pollEvent())
        {
            // Si on ferme la fenêtre
            if (event->is<sf::Event::Closed>())
                window.close();

            // Si on appuie sur Escape
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        // Effacement de la fenêtre en bleu
        window.clear(sf::Color(64, 128, 255));

        // Affichage du contenu de la fenêtre
        window.display();
    }

    return 0;
}