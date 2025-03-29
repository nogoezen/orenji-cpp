#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Core/EntityManager.hpp"
#include "Physics/Box2DWrapper.hpp"
#include "Gameplay/Player.hpp"
#include "Gameplay/Enemy.hpp"
#include "Physics/CollisionCategories.hpp"

// Simple test demonstrating Player and Enemy entities with physics
int main()
{
    // Create window
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Gameplay Entities Test");
    window.setFramerateLimit(60);

    // Create physics world
    Physics::Box2DWrapper physics(sf::Vector2f(0.0f, 9.8f));

    // Create entity manager
    Core::EntityManager entityManager;

    // Create ground
    b2BodyId groundBody = physics.createStaticBody();
    physics.addBoxShape(groundBody, sf::Vector2f(800.0f, 20.0f), sf::Vector2f(400.0f, 580.0f));
    physics.setFilterData(groundBody, Physics::CollisionCategory::GROUND,
                          Physics::CollisionCategory::PLAYER | Physics::CollisionCategory::ENEMY);

    // Create player
    auto &player = entityManager.createEntity<Gameplay::Player>(entityManager, sf::Vector2f(200.0f, 400.0f));
    player.initialize();
    player.setMovementSpeed(150.0f);
    player.setJumpForce(250.0f);

    // Create enemies
    std::vector<std::unique_ptr<Gameplay::Enemy>> enemies;

    // Basic patrol enemy
    auto &patrolEnemy = entityManager.createEntity<Gameplay::Enemy>(
        entityManager, "resources/ai/patrol_behavior.xml", sf::Vector2f(500.0f, 400.0f));
    patrolEnemy.initialize();
    patrolEnemy.setMovementSpeed(70.0f);

    // Game loop
    sf::Clock clock;
    float accumulator = 0.0f;
    const float timeStep = 1.0f / 60.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (auto event = window.pollEvent())
        {
            if (event->type == sf::Event::Closed)
                window.close();
        }

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();
        accumulator += deltaTime;

        // Update physics with fixed timestep
        while (accumulator >= timeStep)
        {
            // Update entities
            entityManager.update(timeStep);

            // Step physics world
            physics.step(timeStep);

            accumulator -= timeStep;
        }

        // Render
        window.clear(sf::Color(64, 64, 64));

        // Draw entities
        entityManager.draw(window);

        // Draw physics debug
        physics.drawDebug(window);

        window.display();
    }

    return 0;
}