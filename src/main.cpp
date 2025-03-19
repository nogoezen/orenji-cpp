#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Orenji - Uncharted Waters");
    window.setFramerateLimit(60);

    // Police pour le texte
    sf::Font font;
    if (!font.loadFromFile("assets/font/UbuntuMono-Regular.ttf"))
    {
        std::cerr << "Erreur: Impossible de charger la police" << std::endl;
        return EXIT_FAILURE;
    }

    // Texte de titre
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Uncharted Waters");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    // Centrer le texte
    sf::FloatRect textRect = titleText.getLocalBounds();
    titleText.setOrigin(textRect.width / 2, textRect.height / 2);
    titleText.setPosition(window.getSize().x / 2, window.getSize().y / 3);

    // Texte "Appuyez sur Entrée"
    sf::Text startText;
    startText.setFont(font);
    startText.setString("Appuyez sur une touche pour quitter");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);

    // Centrer le texte
    textRect = startText.getLocalBounds();
    startText.setOrigin(textRect.width / 2, textRect.height / 2);
    startText.setPosition(window.getSize().x / 2, window.getSize().y * 2 / 3);

    // Boucle principale
    while (window.isOpen())
    {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed)
            {
                window.close();
            }
        }

        // Effacer l'écran
        window.clear(sf::Color::Blue);

        // Dessiner les textes
        window.draw(titleText);
        window.draw(startText);

        // Afficher le résultat
        window.display();
    }

    return EXIT_SUCCESS;
}