#include <SFML/Graphics.hpp>
#include <Physics/Box2DWrapper.hpp>
#include <iostream>

int main()
{
    // Create SFML window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Physics Test");
    window.setFramerateLimit(60);

    // Create physics world
    Physics::Box2DWrapper physics({0.0f, 9.8f}); // Normal gravity
    physics.setDebugDraw(true);                  // Enable debug drawing

    // Create ground
    b2BodyId groundBodyId = physics.createStaticBody({400.0f, 550.0f});
    b2ShapeId groundShapeId = physics.addBoxFixture(groundBodyId, 800.0f, 20.0f);
    physics.setFilterData(groundShapeId, Physics::CollisionCategory::Platform);

    // Create player box - collides with platforms
    b2BodyId playerBodyId = physics.createDynamicBody({400.0f, 300.0f});
    b2ShapeId playerShapeId = physics.addBoxFixture(playerBodyId, 30.0f, 50.0f, 1.0f);
    physics.setFilterData(playerShapeId, Physics::CollisionCategory::Player,
                          Physics::CollisionCategory::Platform | Physics::CollisionCategory::Enemy);

    // Create enemy box - collides with platforms and player
    b2BodyId enemyBodyId = physics.createDynamicBody({500.0f, 300.0f});
    b2ShapeId enemyShapeId = physics.addBoxFixture(enemyBodyId, 30.0f, 30.0f, 1.0f);
    physics.setFilterData(enemyShapeId, Physics::CollisionCategory::Enemy,
                          Physics::CollisionCategory::Platform | Physics::CollisionCategory::Player);

    // Create sensor that doesn't collide with anything but detects overlaps
    b2BodyId sensorBodyId = physics.createStaticBody({300.0f, 300.0f});
    b2ShapeId sensorShapeId = physics.addCircleFixture(sensorBodyId, 50.0f, 0.0f);
    physics.setSensor(sensorShapeId, true);
    physics.setFilterData(sensorShapeId, Physics::CollisionCategory::Sensor,
                          Physics::CollisionCategory::Player | Physics::CollisionCategory::Enemy);

    // Create projectile that only collides with enemies
    b2BodyId projectileBodyId = physics.createDynamicBody({200.0f, 200.0f});
    b2ShapeId projectileShapeId = physics.addCircleFixture(projectileBodyId, 10.0f, 0.5f);
    physics.setFilterData(projectileShapeId, Physics::CollisionCategory::Projectile,
                          Physics::CollisionCategory::Enemy);
    physics.setLinearVelocity(projectileBodyId, {3.0f, 0.0f});

    // Game loop
    sf::Clock clock;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
                else if (keyEvent->code == sf::Keyboard::Key::Left)
                {
                    physics.applyForce(playerBodyId, {-100.0f, 0.0f}, true);
                }
                else if (keyEvent->code == sf::Keyboard::Key::Right)
                {
                    physics.applyForce(playerBodyId, {100.0f, 0.0f}, true);
                }
                else if (keyEvent->code == sf::Keyboard::Key::Up)
                {
                    physics.applyForce(playerBodyId, {0.0f, -200.0f}, true);
                }
            }
        }

        // Step physics
        float deltaTime = clock.restart().asSeconds();
        physics.step(deltaTime);

        // Clear window
        window.clear();

        // Draw physics debug shapes
        physics.debugDraw(window);

        // Display window
        window.display();
    }

    return 0;
}