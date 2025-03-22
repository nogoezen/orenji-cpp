#include "../../include/States/GameState.hpp"
#include <iostream>

namespace Orenji
{

    GameState::GameState(StateMachine &stateMachine) : State(stateMachine)
    {
        // Initialize game state
    }

    void GameState::onEnter()
    {
        // Create physics world with gravity
        b2Vec2 gravity(0.0f, 9.8f);
        m_world = std::make_unique<b2World>(gravity);

        // Setup game view
        m_gameView.setSize(800.0f, 600.0f);
        m_gameView.setCenter(400.0f, 300.0f);

        // Create player entity
        auto player = m_entityManager.createEntity();
        // TODO: Add player components

        // Create ground
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(400.0f / 30.0f, 550.0f / 30.0f); // Convert to Box2D units (meters)

        b2Body *groundBody = m_world->CreateBody(&groundBodyDef);

        b2PolygonShape groundBox;
        groundBox.SetAsBox(400.0f / 30.0f, 10.0f / 30.0f); // Half-width and half-height in meters

        groundBody->CreateFixture(&groundBox, 0.0f);
    }

    void GameState::onExit()
    {
        // Cleanup if needed
        m_world.reset();
    }

    void GameState::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                m_stateMachine.popState();
                break;

            case sf::Keyboard::D:
                // Toggle debug drawing
                m_debugDraw = !m_debugDraw;
                break;

            default:
                break;
            }
        }
    }

    void GameState::update(float deltaTime)
    {
        // Update physics world
        m_world->Step(m_timeStep, m_velocityIterations, m_positionIterations);

        // Update entities
        m_entityManager.update(deltaTime);

        // Update camera if needed
        // m_gameView.setCenter(...);
    }

    void GameState::render(sf::RenderTarget &target)
    {
        // Set the game view
        sf::View oldView = target.getView();
        target.setView(m_gameView);

        // Draw the game world
        // TODO: Draw background, tiles, etc.

        // Draw entities
        // The EntityManager does not directly support drawing, so we need to draw entities manually
        for (const auto &entity : m_entityManager.getEntities())
        {
            if (entity->isActive())
            {
                target.draw(*entity);
            }
        }

        // Draw debug information if enabled
        if (m_debugDraw)
        {
            // TODO: Draw debug shapes for physics bodies
        }

        // Restore the original view
        target.setView(oldView);
    }

} // namespace Orenji