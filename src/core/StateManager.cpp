#include "StateManager.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
    clearStates();
}

std::shared_ptr<State> StateManager::getState(const std::string &name) const
{
    auto it = m_states.find(name);
    if (it != m_states.end())
    {
        return it->second;
    }
    return nullptr;
}

bool StateManager::pushState(const std::string &name)
{
    auto state = getState(name);
    if (!state)
    {
        return false;
    }

    if (!m_stack.empty())
    {
        m_stack.back()->pause();
    }

    m_stack.push_back(state);
    state->enter();
    return true;
}

void StateManager::popState()
{
    if (!m_stack.empty())
    {
        m_stack.back()->exit();
        m_stack.pop_back();

        if (!m_stack.empty())
        {
            m_stack.back()->resume();
        }
    }
}

void StateManager::clearStates()
{
    while (!m_stack.empty())
    {
        m_stack.back()->exit();
        m_stack.pop_back();
    }
    m_states.clear();
}

void StateManager::handleInput()
{
    if (!m_stack.empty() && m_stack.back()->isActive())
    {
        m_stack.back()->handleInput();
    }
}

void StateManager::update(float deltaTime)
{
    if (!m_stack.empty() && m_stack.back()->isActive())
    {
        m_stack.back()->update(deltaTime);
    }
}

void StateManager::render()
{
    // Render from bottom to top of the stack
    for (auto &state : m_stack)
    {
        if (state->isActive())
        {
            state->render();
        }
    }
}

std::shared_ptr<State> StateManager::getCurrentState() const
{
    if (!m_stack.empty())
    {
        return m_stack.back();
    }
    return nullptr;
}