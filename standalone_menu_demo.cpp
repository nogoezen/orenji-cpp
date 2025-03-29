#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <cstdint>

// SimplifiedMenuDemo - Une version simplifiée du menu principal pour SFML 3
int main()
{
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1024, 768)), "Orenji Engine - Menu Demo");
    window.setFramerateLimit(60);

    // Chargement des ressources
    sf::Texture backgroundTexture;
    sf::Texture overlayTexture;
    sf::Font mainFont;
    sf::Music *music = nullptr;

    bool resourcesLoaded = true;

    // Chargement de la police principale
    if (!mainFont.openFromFile("resources/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Impossible de charger la police" << std::endl;
        resourcesLoaded = false;
    }

    // Chargement des textures
    if (!backgroundTexture.loadFromFile("resources/textures/Titles/title-bg.png"))
    {
        std::cerr << "Impossible de charger la texture de fond" << std::endl;
        resourcesLoaded = false;
    }

    if (!overlayTexture.loadFromFile("resources/textures/Titles/001-Title01.jpg"))
    {
        std::cerr << "Impossible de charger la texture d'overlay" << std::endl;
        resourcesLoaded = false;
    }

    // Charger la musique (optionnel)
    try
    {
        music = new sf::Music();
        if (!music->openFromFile("resources/sounds/BGM/012-Theme01.mp3"))
        {
            std::cerr << "Impossible de charger la musique, mais on continue sans" << std::endl;
            delete music;
            music = nullptr;
        }
        else
        {
            music->setLooping(true);
            music->setVolume(70.0f);
            music->play();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement de la musique: " << e.what() << std::endl;
        std::cerr << "Poursuite sans musique" << std::endl;
        if (music)
        {
            delete music;
            music = nullptr;
        }
    }

    // Création des éléments d'interface si les ressources sont chargées
    if (!resourcesLoaded)
    {
        std::cerr << "Impossible de charger toutes les ressources nécessaires" << std::endl;
        if (music)
        {
            music->stop();
            delete music;
        }
        return 1;
    }

    // Création du fond
    sf::Sprite background(backgroundTexture);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);
    background.setScale(sf::Vector2f(scaleX, scaleY));

    // Création de l'overlay
    sf::Sprite overlay(overlayTexture);
    sf::Vector2u overlaySize = overlayTexture.getSize();
    float overlayScale = static_cast<float>(windowSize.x) / static_cast<float>(overlaySize.x);
    overlay.setScale(sf::Vector2f(overlayScale, overlayScale));
    float overlayHeight = overlaySize.y * overlayScale;
    overlay.setPosition(sf::Vector2f(0, windowSize.y - overlayHeight));
    overlay.setColor(sf::Color(255, 255, 255, 0)); // Commencer transparent

    // Création du titre
    sf::Text titleText(mainFont, "Orenji Engine", 80);
    titleText.setFillColor(sf::Color(255, 128, 0, 0)); // Commencer transparent
    titleText.setOutlineColor(sf::Color(128, 64, 0, 0));
    titleText.setOutlineThickness(3.0f);
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(sf::Vector2f(titleBounds.size.x / 2.0f,
                                     titleBounds.size.y / 2.0f));
    titleText.setPosition(sf::Vector2f(windowSize.x / 2.0f, 120.0f));

    // Création des options de menu
    std::vector<sf::Text> menuItems;
    std::vector<std::string> menuTexts = {"New Game", "Examples", "Options", "Exit"};
    float startY = 280.0f;
    float spacing = 80.0f;

    for (size_t i = 0; i < menuTexts.size(); ++i)
    {
        sf::Text item(mainFont, menuTexts[i], 48);
        item.setFillColor(sf::Color(255, 255, 255, 0)); // Commencer transparent
        sf::FloatRect itemBounds = item.getLocalBounds();
        item.setOrigin(sf::Vector2f(itemBounds.size.x / 2.0f,
                                    itemBounds.size.y / 2.0f));
        item.setPosition(sf::Vector2f(windowSize.x / 2.0f, startY + i * spacing));
        menuItems.push_back(item);
    }

    // Variables d'état
    bool isTransitioning = true;
    float transitionAlpha = 0.0f;
    int selectedItem = 0;

    // Horloge pour les animations
    sf::Clock clock;
    float elapsedTime = 0.0f;

    // Boucle principale
    while (window.isOpen())
    {
        // Gestion du temps
        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        // Gestion des événements
        if (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (!isTransitioning)
            {
                if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyEvent->code == sf::Keyboard::Key::Up)
                    {
                        if (selectedItem > 0)
                        {
                            selectedItem--;
                        }
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Down)
                    {
                        if (selectedItem < static_cast<int>(menuItems.size()) - 1)
                        {
                            selectedItem++;
                        }
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Enter)
                    {
                        if (selectedItem == 3)
                        { // Exit
                            window.close();
                        }
                        else
                        {
                            // Effet visuel pour l'option sélectionnée
                            titleText.setFillColor(sf::Color(0, 255, 128));
                            titleText.setOutlineColor(sf::Color(0, 128, 64));
                        }
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Escape)
                    {
                        window.close();
                    }
                }
            }
        }

        // Mise à jour des animations
        if (isTransitioning)
        {
            // Animation de transition - fondu
            transitionAlpha += deltaTime * 255.0f * 0.7f; // 0.7 secondes pour le fondu

            if (transitionAlpha >= 255.0f)
            {
                transitionAlpha = 255.0f;
                isTransitioning = false;
            }

            // Application des alphas
            overlay.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(transitionAlpha)));

            float titleAlpha = std::min(255.0f, transitionAlpha * 1.5f);
            titleText.setFillColor(sf::Color(255, 128, 0, static_cast<std::uint8_t>(titleAlpha)));
            titleText.setOutlineColor(sf::Color(128, 64, 0, static_cast<std::uint8_t>(titleAlpha)));

            // Animation des options de menu en cascade
            for (size_t i = 0; i < menuItems.size(); ++i)
            {
                float delay = 0.1f * static_cast<float>(i); // Délai de 0.1s entre les options
                float itemAlpha = std::max(0.0f, std::min(255.0f, (transitionAlpha - (delay * 255.0f)) * 1.2f));

                if (i == selectedItem)
                {
                    menuItems[i].setFillColor(sf::Color(255, 200, 0, static_cast<std::uint8_t>(itemAlpha)));
                    menuItems[i].setOutlineColor(sf::Color(255, 100, 0, static_cast<std::uint8_t>(itemAlpha)));
                    menuItems[i].setOutlineThickness(2.0f);
                    menuItems[i].setStyle(sf::Text::Bold);
                }
                else
                {
                    menuItems[i].setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(itemAlpha)));
                }
            }
        }
        else
        {
            // Animation du titre (flottement)
            float offsetY = std::sin(elapsedTime * 1.5f) * 5.0f;
            titleText.setPosition(sf::Vector2f(windowSize.x / 2.0f, 120.0f + offsetY));

            // Effet de respiration pour l'option sélectionnée
            float breathingFactor = (std::sin(elapsedTime * 3.0f) + 1.0f) / 2.0f; // 0 à 1
            float scaleFactor = 1.0f + breathingFactor * 0.05f;                   // Entre 1.0 et 1.05

            for (size_t i = 0; i < menuItems.size(); ++i)
            {
                if (i == selectedItem)
                {
                    menuItems[i].setScale(sf::Vector2f(scaleFactor, scaleFactor));

                    // Effet de pulsation de couleur
                    sf::Color pulseColor = sf::Color(
                        255,
                        200 + static_cast<std::uint8_t>(breathingFactor * 55.0f), // 200-255
                        0 + static_cast<std::uint8_t>(breathingFactor * 100.0f)   // 0-100
                    );
                    menuItems[i].setFillColor(pulseColor);
                    menuItems[i].setOutlineThickness(2.0f);
                    menuItems[i].setOutlineColor(sf::Color(255, 100, 0));
                    menuItems[i].setStyle(sf::Text::Bold);
                }
                else
                {
                    menuItems[i].setScale(sf::Vector2f(1.0f, 1.0f));
                    menuItems[i].setFillColor(sf::Color::White);
                    menuItems[i].setOutlineThickness(0.0f);
                    menuItems[i].setStyle(sf::Text::Regular);
                }
            }
        }

        // Rendu
        window.clear(sf::Color(40, 40, 40));
        window.draw(background);
        window.draw(overlay);
        window.draw(titleText);

        for (const auto &item : menuItems)
        {
            window.draw(item);
        }

        window.display();
    }

    // Nettoyage
    if (music)
    {
        music->stop();
        delete music;
    }

    return 0;
}