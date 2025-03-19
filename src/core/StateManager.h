#pragma once

#include "State.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <functional>

class StateManager
{
public:
    StateManager();
    ~StateManager();

    template <typename T, typename... Args>
    std::shared_ptr<T> createState(Args &&...args);

    std::shared_ptr<State> getState(const std::string &name) const;

    template <typename T>
    std::shared_ptr<T> getState() const;

    bool pushState(const std::string &name);
    void popState();
    void clearStates();

    void handleInput();
    void update(float deltaTime);
    void render();

    std::shared_ptr<State> getCurrentState() const;
    bool isEmpty() const { return m_stack.empty(); }

private:
    std::unordered_map<std::string, std::shared_ptr<State>> m_states;
    std::vector<std::shared_ptr<State>> m_stack;
};

template <typename T, typename... Args>
std::shared_ptr<T> StateManager::createState(Args &&...args)
{
    static_assert(std::is_base_of<State, T>::value, "T must derive from State");

    auto state = std::make_shared<T>(std::forward<Args>(args)...);
    state->setManager(this);
    m_states[state->getName()] = state;
    return state;
}

template <typename T>
std::shared_ptr<T> StateManager::getState() const
{
    static_assert(std::is_base_of<State, T>::value, "T must derive from State");

    for (const auto &pair : m_states)
    {
        auto derived = std::dynamic_pointer_cast<T>(pair.second);
        if (derived)
        {
            return derived;
        }
    }
    return nullptr;
}