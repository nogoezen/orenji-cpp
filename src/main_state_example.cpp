#include <SFML/Graphics.hpp>
#include "State/StateManager.hpp"
#include "State/MenuState.hpp"
#include "State/GameState.hpp"
#include <iostream>
#include <memory>

using namespace Orenji;

// Exemple d'utilisation du système de State
// Ce programme montre comment:
// - Créer et gérer différents états
// - Passer d'un état à l'autre
// - Empiler des états

int main()
{
    // Création de la fenêtre
    sf::VideoMode videoMode(800, 600);
    sf::RenderWindow window(videoMode, "Exemple du Système d'États");
    window.setFramerateLimit(60);

    // Création du gestionnaire d'états
    StateManager stateManager;

    // Enregistre les types d'états
    stateManager.registerState<MenuState>("menu");
    stateManager.registerState<GameState>("game");

    // Crée un état de menu initial
    stateManager.pushState("menu");

    // Récupère l'état de menu pour le configurer
    if (auto menuState = dynamic_cast<MenuState *>(stateManager.getCurrentState()))
    {
        menuState->setTitle("Menu Principal");

        // Option pour commencer le jeu
        menuState->addMenuItem("Commencer le jeu", [&stateManager]()
                               {
                                   stateManager.pushState("game", true); // Remplace le menu par le jeu
                               });

        // Option pour quitter le jeu
        menuState->addMenuItem("Quitter", [&window]()
                               { window.close(); });
    }

    // Horloge pour le calcul du deltaTime
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Calcul du temps écoulé
        float deltaTime = clock.restart().asSeconds();

        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Type::Closed)
            {
                window.close();
            }
            else
            {
                // Passe l'événement à l'état actuel
                stateManager.handleEvent(event);
            }
        }

        // Met à jour l'état actuel
        stateManager.update(deltaTime);

        // Vérifie si tous les états sont terminés
        if (stateManager.isEmpty())
        {
            window.close();
        }

        // Efface l'écran
        window.clear(sf::Color(40, 40, 60));

        // Affiche l'état actuel
        stateManager.render(window);

        // Affiche le résultat
        window.display();

        // Traite les changements d'états
        stateManager.processStateChanges();
    }

    return 0;
}