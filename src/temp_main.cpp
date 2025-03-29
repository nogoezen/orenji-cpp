#include "include/Engine.hpp" 
#include "include/Scenes/MainMenuScene.hpp" 
#include <iostream> 
#include <memory> 
 
int main() 
{ 
    try 
    { 
        sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1024, 768)), "Orenji Engine - Demo"); 
        window.setFramerateLimit(60); 
 
        // Créer le gestionnaire de ressources 
        Resources::ResourceManager resourceManager; 
 
        // Créer la scène de menu 
        Scenes::MainMenuScene mainMenu(window, resourceManager); 
        mainMenu.init(); 
 
        // Boucle principale 
        sf::Clock clock; 
        while (window.isOpen()) 
        { 
            // Calculer le temps delta 
            float deltaTime = clock.restart().asSeconds(); 
 
            // Traiter les événements 
            if (auto event = window.pollEvent()) 
            { 
                if (event->type == sf::Event::Closed) 
                    window.close(); 
 
                mainMenu.handleEvent(*event); 
            } 
 
            // Mise à jour 
            mainMenu.update(deltaTime); 
 
            // Rendu 
            window.clear(sf::Color(40, 40, 40)); 
            mainMenu.render(window); 
            window.display(); 
        } 
 
        return 0; 
    } 
    { 
        std::cerr << "Exception: " << e.what() << std::endl; 
        return 1; 
    } 
} 
