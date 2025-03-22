#include <SFML/Graphics.hpp>
#include "Scene/SceneManager.hpp"
#include "Scene/TitleScene.hpp"
#include <iostream>
#include <memory>

using namespace Orenji;

int main()
{
    // Création de la fenêtre
    sf::VideoMode videoMode(800, 600);
    sf::RenderWindow window(videoMode, "Orenji - Menu Titre");
    window.setFramerateLimit(60);

    // Création du gestionnaire de scène
    SceneManager sceneManager;

    // Enregistre le type de scène TitleScene
    sceneManager.registerScene<TitleScene>("title");

    // Crée et ajoute la scène de titre
    TitleScene *titleScene = static_cast<TitleScene *>(sceneManager.createScene("title", "TitleScene"));

    if (!titleScene)
    {
        std::cerr << "Erreur: Impossible de créer la scène de titre" << std::endl;
        return -1;
    }

    // Personnalise la scène de titre
    titleScene->setMainTitle("ORENJI");
    titleScene->setSubtitle("Un Jeu Magique");

    // Ajoute des options au menu
    titleScene->addMenuItem("Nouvelle Partie", []()
                            { std::cout << "Nouvelle partie sélectionnée" << std::endl; });

    titleScene->addMenuItem("Charger Partie", []()
                            { std::cout << "Charger partie sélectionnée" << std::endl; });

    titleScene->addMenuItem("Options", []()
                            { std::cout << "Options sélectionnées" << std::endl; });

    titleScene->addMenuItem("Quitter", [&window]()
                            { 
        std::cout << "Quitter sélectionné" << std::endl;
        window.close(); });

    // Active la scène de titre
    sceneManager.activateScene("TitleScene");

    // Horloge pour mesurer le temps entre les images
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Calcul du temps écoulé
        float deltaTime = clock.restart().asSeconds();

        // Traitement des événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed ||
                     event.type == sf::Event::MouseButtonPressed)
            {
                // Transmet l'événement à la scène de titre
                titleScene->handleEvent(event);
            }
        }

        // Mise à jour
        sceneManager.update(deltaTime);

        // Rendu
        window.clear(sf::Color(20, 20, 40));
        sceneManager.render(window);
        window.display();
    }

    return 0;
}