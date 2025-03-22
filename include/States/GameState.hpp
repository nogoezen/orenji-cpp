#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/box2d.h>
#include "../Core/EntityManager.hpp"
#include "State.hpp"

namespace Orenji
{

    /**
     * @class GameState
     * @brief State for the main game
     */
    class GameState : public State
    {
    public:
        /**
         * @brief Constructor
         * @param stateMachine Reference to the state machine
         */
        explicit GameState(StateMachine &stateMachine);

        /**
         * @brief Called when the state is entered
         */
        virtual void onEnter() override;

        /**
         * @brief Called when the state is exited
         */
        virtual void onExit() override;

        /**
         * @brief Handle input events
         * @param event SFML event to handle
         */
        virtual void handleEvent(const sf::Event &event) override;

        /**
         * @brief Update the state
         * @param deltaTime Time elapsed since last update
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Render the state
         * @param target Render target
         */
        virtual void render(sf::RenderTarget &target) override;

    private:
        EntityManager m_entityManager;
        sf::View m_gameView;

        // Box2D physics
        std::unique_ptr<b2World> m_world;
        const float m_timeStep = 1.0f / 60.0f;
        const int m_velocityIterations = 6;
        const int m_positionIterations = 2;

        // Debug drawing
        bool m_debugDraw = false;
    };

} // namespace Orenji