#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <memory>
#include <vector>

#include "../include/Physics/PhysicsWorld.hpp"
#include "../include/Physics/PhysicsComponent.hpp"
#include "../include/Core/Entity.hpp"

// Exemple simple d'utilisation du module Physics avec Box2D 2.4.x
int main()
{
    // Créer la fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Orenji Engine - Box2D Example");
    window.setFramerateLimit(60);

    // Obtenir l'instance du monde physique
    Orenji::PhysicsWorld &physics = Orenji::PhysicsWorld::getInstance();

    // Activer le dessin de debug
    physics.setDebugDrawEnabled(true);

    // Créer un sol statique
    b2BodyId groundBody = physics.createBody(sf::Vector2f(400.0f, 550.0f), b2_staticBody);
    physics.addBoxFixture(groundBody, sf::Vector2f(800.0f, 20.0f), 0.0f, 0.3f, 0.0f);

    // Créer des entités avec des composants physiques
    std::vector<std::unique_ptr<Orenji::Entity>> entities;
    unsigned int nextEntityId = 1;

    // Créer quelques boîtes
    for (int i = 0; i < 10; i++)
    {
        auto entity = std::make_unique<Orenji::Entity>(nextEntityId++, "Box_" + std::to_string(i));
        auto physicsComp = entity->addComponent<Orenji::PhysicsComponent>(entity.get());

        // Position aléatoire en haut de l'écran
        float x = 100.0f + static_cast<float>(rand() % 600);
        float y = 50.0f + static_cast<float>(i * 40);

        physicsComp->setPosition(sf::Vector2f(x, y));
        physicsComp->setBodyType(b2_dynamicBody);

        // Initialiser le composant physique
        physicsComp->initialize();

        // Créer une fixture en forme de boîte
        float size = 20.0f + static_cast<float>(rand() % 30);
        physicsComp->createBoxFixture(sf::Vector2f(size, size), 1.0f, 0.3f, 0.3f);

        // Ajouter l'entité à notre collection
        entities.push_back(std::move(entity));
    }

    // Créer quelques cercles
    for (int i = 0; i < 10; i++)
    {
        auto entity = std::make_unique<Orenji::Entity>(nextEntityId++, "Circle_" + std::to_string(i));
        auto physicsComp = entity->addComponent<Orenji::PhysicsComponent>(entity.get());

        // Position aléatoire en haut de l'écran
        float x = 100.0f + static_cast<float>(rand() % 600);
        float y = 50.0f + static_cast<float>(i * 40);

        physicsComp->setPosition(sf::Vector2f(x, y));
        physicsComp->setBodyType(b2_dynamicBody);

        // Initialiser le composant physique
        physicsComp->initialize();

        // Créer une fixture en forme de cercle
        float radius = 10.0f + static_cast<float>(rand() % 15);
        physicsComp->createCircleFixture(radius, 1.0f, 0.3f, 0.7f);

        // Ajouter l'entité à notre collection
        entities.push_back(std::move(entity));
    }

    // Horloge pour le calcul du temps écoulé
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Gérer les événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Réinitialiser la simulation en appuyant sur R
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                // Vider les entités existantes
                entities.clear();

                // Créer de nouvelles entités
                for (int i = 0; i < 10; i++)
                {
                    auto entity = std::make_unique<Orenji::Entity>(nextEntityId++, "Entity_" + std::to_string(nextEntityId));
                    auto physicsComp = entity->addComponent<Orenji::PhysicsComponent>(entity.get());

                    float x = 100.0f + static_cast<float>(rand() % 600);
                    float y = 50.0f + static_cast<float>(i * 40);

                    physicsComp->setPosition(sf::Vector2f(x, y));
                    physicsComp->setBodyType(b2_dynamicBody);
                    physicsComp->initialize();

                    // Choisir aléatoirement entre boîte et cercle
                    if (rand() % 2 == 0)
                    {
                        float size = 20.0f + static_cast<float>(rand() % 30);
                        physicsComp->createBoxFixture(sf::Vector2f(size, size), 1.0f, 0.3f, 0.3f);
                    }
                    else
                    {
                        float radius = 10.0f + static_cast<float>(rand() % 15);
                        physicsComp->createCircleFixture(radius, 1.0f, 0.3f, 0.7f);
                    }

                    entities.push_back(std::move(entity));
                }
            }

            // Ajouter une boîte en appuyant sur B
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
            {
                auto entity = std::make_unique<Orenji::Entity>(nextEntityId++, "Box_" + std::to_string(nextEntityId));
                auto physicsComp = entity->addComponent<Orenji::PhysicsComponent>(entity.get());

                // Positionner sous le curseur de la souris
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                physicsComp->setPosition(sf::Vector2f(static_cast<float>(mousePos.x),
                                                      static_cast<float>(mousePos.y)));
                physicsComp->setBodyType(b2_dynamicBody);
                physicsComp->initialize();

                float size = 20.0f + static_cast<float>(rand() % 30);
                physicsComp->createBoxFixture(sf::Vector2f(size, size), 1.0f, 0.3f, 0.3f);

                entities.push_back(std::move(entity));
            }

            // Ajouter un cercle en appuyant sur C
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C)
            {
                auto entity = std::make_unique<Orenji::Entity>(nextEntityId++, "Circle_" + std::to_string(nextEntityId));
                auto physicsComp = entity->addComponent<Orenji::PhysicsComponent>(entity.get());

                // Positionner sous le curseur de la souris
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                physicsComp->setPosition(sf::Vector2f(static_cast<float>(mousePos.x),
                                                      static_cast<float>(mousePos.y)));
                physicsComp->setBodyType(b2_dynamicBody);
                physicsComp->initialize();

                float radius = 10.0f + static_cast<float>(rand() % 15);
                physicsComp->createCircleFixture(radius, 1.0f, 0.3f, 0.7f);

                entities.push_back(std::move(entity));
            }
        }

        // Calculer le temps écoulé
        float deltaTime = clock.restart().asSeconds();

        // Mettre à jour le monde physique
        physics.update(deltaTime);

        // Mettre à jour les entités
        for (auto &entity : entities)
        {
            // Mettre à jour les composants physiques des entités
            auto physicsComp = entity->getComponent<Orenji::PhysicsComponent>();
            if (physicsComp)
            {
                physicsComp->update(deltaTime);
            }
        }

        // Effacer la fenêtre
        window.clear(sf::Color(50, 50, 50));

        // Dessiner les formes physiques (debug)
        physics.debugDraw(window);

        // Afficher
        window.display();
    }

    return 0;
}