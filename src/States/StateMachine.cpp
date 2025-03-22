#include "../../include/States/StateMachine.hpp"

namespace Orenji
{

    StateMachine::StateMachine()
    {
        // Initialize the state machine
    }

    void StateMachine::popState()
    {
        // Request to pop the current state
        m_pendingChanges.push_back([this]()
                                   {
        if (!m_states.empty()) {
            // Exit and pop the current state
            m_states.top()->onExit();
            m_states.pop();
            
            // Enter the previous state if exists
            if (!m_states.empty()) {
                m_states.top()->onEnter();
            }
        } });
    }

    void StateMachine::processPendingChanges()
    {
        // Process all pending state changes
        for (auto &change : m_pendingChanges)
        {
            change();
        }
        m_pendingChanges.clear();
    }

    void StateMachine::handleEvent(const sf::Event &event)
    {
        // Handle events in the current state
        if (!m_states.empty())
        {
            m_states.top()->handleEvent(event);
        }
    }

    void StateMachine::update(float deltaTime)
    {
        // Process any pending state changes
        processPendingChanges();

        // Update the current state
        if (!m_states.empty())
        {
            m_states.top()->update(deltaTime);
        }
    }

    void StateMachine::render(sf::RenderTarget &target)
    {
        // Render all visible states from bottom to top
        if (m_states.empty())
            return;

        // Create a copy of the state stack to iterate through
        std::vector<StatePtr> visibleStates;
        auto tempStack = m_states;

        while (!tempStack.empty())
        {
            visibleStates.push_back(tempStack.top());
            tempStack.pop();

            // If the state is not transparent, stop the iteration
            if (!visibleStates.back()->isTransparent())
                break;
        }

        // Render the states in reverse order (bottom to top)
        for (auto it = visibleStates.rbegin(); it != visibleStates.rend(); ++it)
        {
            (*it)->render(target);
        }
    }

    bool StateMachine::isEmpty() const
    {
        return m_states.empty();
    }

} // namespace Orenji