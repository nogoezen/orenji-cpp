#pragma once

#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>

#include "State.hpp"

namespace Orenji
{

    /**
     * @class StateMachine
     * @brief Manages game states
     */
    class StateMachine
    {
    public:
        /**
         * @brief Constructor
         */
        StateMachine();

        /**
         * @brief Destructor
         */
        ~StateMachine() = default;

        /**
         * @brief Push a new state to the stack
         * @tparam T State type
         * @tparam Args Constructor argument types
         * @param args Constructor arguments
         */
        template <typename T, typename... Args>
        void pushState(Args &&...args)
        {
            static_assert(std::is_base_of<State, T>::value, "T must derive from State");

            // Create the new state
            auto newState = std::make_shared<T>(*this, std::forward<Args>(args)...);

            // Request to push the state
            m_pendingChanges.push_back([this, newState]()
                                       {
            if (!m_states.empty()) {
                if (!newState->isTransparent()) {
                    // If the new state is not transparent, exit the current state
                    m_states.top()->onExit();
                }
            }
            
            // Push and enter the new state
            m_states.push(newState);
            m_states.top()->onEnter(); });
        }

        /**
         * @brief Pop the current state from the stack
         */
        void popState();

        /**
         * @brief Pop all states and push a new one
         * @tparam T State type
         * @tparam Args Constructor argument types
         * @param args Constructor arguments
         */
        template <typename T, typename... Args>
        void changeState(Args &&...args)
        {
            static_assert(std::is_base_of<State, T>::value, "T must derive from State");

            // Create the new state
            auto newState = std::make_shared<T>(*this, std::forward<Args>(args)...);

            // Request to change the state
            m_pendingChanges.push_back([this, newState]()
                                       {
            // Clear the state stack
            while (!m_states.empty()) {
                m_states.top()->onExit();
                m_states.pop();
            }
            
            // Push and enter the new state
            m_states.push(newState);
            m_states.top()->onEnter(); });
        }

        /**
         * @brief Process all pending state changes
         */
        void processPendingChanges();

        /**
         * @brief Handle input events
         * @param event SFML event to handle
         */
        void handleEvent(const sf::Event &event);

        /**
         * @brief Update the current state
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime);

        /**
         * @brief Render all visible states
         * @param target Render target
         */
        void render(sf::RenderTarget &target);

        /**
         * @brief Check if there are no states
         * @return True if the state stack is empty
         */
        bool isEmpty() const;

    private:
        std::stack<StatePtr> m_states;                       // Stack of states
        std::vector<std::function<void()>> m_pendingChanges; // Pending state changes
    };

} // namespace Orenji