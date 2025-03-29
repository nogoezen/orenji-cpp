#include "../../include/Scenes/GameScene.hpp"
#include "../../include/Engine.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace Scenes
{

    GameScene::GameScene(Core::Engine &engine)
        : Core::Scene("Game"), m_engine(engine)
    {
        std::cout << "GameScene created" << std::endl;
    }

    GameScene::~GameScene()
    {
        std::cout << "GameScene destroyed" << std::endl;
    }

    void GameScene::init()
    {
        std::cout << "GameScene initialized" << std::endl;

        // Dans une vraie implémentation, nous initialiserions:
        // - Charger les ressources
        // - Créer les entités de jeu
        // - Configurer la caméra
        // - Configurer les systèmes (physique, IA, etc.)
    }

    void GameScene::update(float deltaTime)
    {
        // Mise à jour de la logique de jeu
        // Ceci est généralement déjà géré par les systèmes (physique, IA, etc.)
        // mais nous pouvons ajouter une logique spécifique à la scène ici
    }

    void GameScene::render(sf::RenderWindow &window)
    {
        // Rendu spécifique à la scène
        // La plupart du rendu est déjà géré par le système de rendu,
        // mais nous pouvons ajouter du rendu spécifique à la scène ici
    }

    void GameScene::handleEvent(const sf::Event &event)
    {
        // Gestion des événements spécifiques à la scène
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::Escape)
            {
                // Mettre le jeu en pause
                std::cout << "Game paused" << std::endl;
                // TODO: Passer à PauseScene
            }
        }
    }

} // namespace Scenes