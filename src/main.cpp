#include <SFML/Graphics.hpp>
#include "core/StateManager.h"
#include "states/TitleScreenState.h"
#include "states/MainMenuState.h"
#include "states/CharacterCreationState.h"
#include "states/GameState.h"
#include <iostream>

// Variable globale pour la fenêtre
sf::RenderWindow *g_window = nullptr;

int main()
{
    // Création de la fenêtre
    g_window = new sf::RenderWindow(sf::VideoMode(800, 600), "Uncharted Waters");
    g_window->setFramerateLimit(60);

    // Création du gestionnaire d'états
    StateManager stateManager;

    // Enregistrement des états
    stateManager.registerState("TitleScreen", std::make_shared<TitleScreenState>());
    stateManager.registerState("MainMenu", std::make_shared<MainMenuState>());
    stateManager.registerState("CharacterCreation", std::make_shared<CharacterCreationState>());
    stateManager.registerState("Game", std::make_shared<GameState>());

    // Démarrage avec l'écran titre
    stateManager.pushState("TitleScreen");

    // Boucle principale
    sf::Clock clock;
    while (g_window->isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        // Mise à jour des états
        stateManager.update(deltaTime);

        // Rendu
        g_window->clear(sf::Color::Black);
        stateManager.render();
        g_window->display();
    }

    // Nettoyage
    delete g_window;
    return 0;
}