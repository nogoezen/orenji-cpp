#include "../../include/States/StateMachine.hpp"
#include <iostream>

namespace Orenji
{

    StateMachine::StateMachine()
        : m_isAdding(false), m_isRemoving(false), m_isReplacing(false),
          m_window(nullptr)
    {
    }

    StateMachine::~StateMachine()
    {
        while (!m_states.empty())
        {
            m_states.top()->onExit();
            m_states.pop();
        }
    }

    void StateMachine::pushState(std::unique_ptr<State> state)
    {
        m_isAdding = true;
        m_isReplacing = false;
        m_pendingState = std::move(state);
    }

    void StateMachine::popState()
    {
        m_isRemoving = true;
    }

    void StateMachine::changeState(std::unique_ptr<State> state)
    {
        m_isAdding = true;
        m_isReplacing = true;
        m_pendingState = std::move(state);
    }

    State *StateMachine::getCurrentState() const
    {
        if (m_states.empty())
        {
            return nullptr;
        }
        return m_states.top().get();
    }

    bool StateMachine::isEmpty() const
    {
        return m_states.empty();
    }

    void StateMachine::update(float deltaTime)
    {
        applyPendingChanges();

        if (!m_states.empty())
        {
            if (!m_states.top()->update(deltaTime))
            {
                std::cerr << "Error updating state" << std::endl;
            }

            if (m_states.top()->getStatus() == State::StateStatus::Finished)
            {
                popState();
                applyPendingChanges();
            }
        }
    }

    void StateMachine::render()
    {
        if (!m_states.empty())
        {
            if (!m_states.top()->render())
            {
                std::cerr << "Error rendering state" << std::endl;
            }
        }
    }

    void StateMachine::handleInput()
    {
        if (!m_states.empty())
        {
            if (!m_states.top()->handleInput())
            {
                std::cerr << "Error handling input in state" << std::endl;
            }
        }
    }

    void StateMachine::setWindow(sf::RenderWindow &window)
    {
        m_window = &window;
    }

    sf::RenderWindow &StateMachine::getWindow() const
    {
        if (!m_window)
        {
            throw std::runtime_error("Window has not been set in StateMachine");
        }
        return *m_window;
    }

    float StateMachine::getElapsedTime() const
    {
        return m_globalClock.getElapsedTime().asSeconds();
    }

    void StateMachine::applyPendingChanges()
    {
        if (m_isRemoving && !m_states.empty())
        {
            if (!m_states.top()->onExit())
            {
                std::cerr << "Error during state exit" << std::endl;
            }

            m_states.pop();
            m_isRemoving = false;

            if (!m_states.empty())
            {
                if (!m_states.top()->onEnter())
                {
                    std::cerr << "Error during state re-entry" << std::endl;
                }
            }
        }

        if (m_isAdding)
        {
            if (!m_states.empty() && m_isReplacing)
            {
                if (!m_states.top()->onExit())
                {
                    std::cerr << "Error during state exit on replacement" << std::endl;
                }

                m_states.pop();
            }

            if (m_pendingState)
            {
                m_pendingState->setStateMachine(this);

                if (m_pendingState->initialize())
                {
                    m_states.push(std::move(m_pendingState));

                    if (!m_states.top()->onEnter())
                    {
                        std::cerr << "Error during state entry" << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Error initializing state" << std::endl;
                    m_pendingState.reset();
                }
            }

            m_isAdding = false;
            m_isReplacing = false;
        }
    }

} // namespace Orenji