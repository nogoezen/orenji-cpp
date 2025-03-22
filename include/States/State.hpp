#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

namespace Orenji
{

    // Forward declaration
    class StateMachine;

    /**
     * @class State
     * @brief Base class for all game states
     */
    class State
    {
    public:
        /**
         * @brief Constructor
         * @param stateMachine Reference to the state machine
         */
        explicit State(StateMachine &stateMachine);

        /**
         * @brief Virtual destructor
         */
        virtual ~State() = default;

        /**
         * @brief Called when the state is entered
         */
        virtual void onEnter() = 0;

        /**
         * @brief Called when the state is exited
         */
        virtual void onExit() = 0;

        /**
         * @brief Handle input events
         * @param event SFML event to handle
         */
        virtual void handleEvent(const sf::Event &event) = 0;

        /**
         * @brief Update the state
         * @param deltaTime Time elapsed since last update
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Render the state
         * @param target Render target
         */
        virtual void render(sf::RenderTarget &target) = 0;

        /**
         * @brief Check if the state is transparent (previous state visible)
         * @return True if the state is transparent
         */
        virtual bool isTransparent() const { return false; }

        /**
         * @brief Get reference to the state machine
         * @return Reference to the state machine
         */
        StateMachine &getStateMachine() { return m_stateMachine; }

    protected:
        StateMachine &m_stateMachine; // Reference to the state machine
    };

    using StatePtr = std::shared_ptr<State>;

} // namespace Orenji