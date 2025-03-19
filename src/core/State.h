#pragma once

#include <string>
#include <memory>
#include "EntityManager.h"

class StateManager;

class State
{
public:
    State(const std::string &name = "");
    virtual ~State();

    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void pause();
    virtual void resume();

    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;

    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

    const std::string &getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    EntityManager &getEntityManager() { return m_entityManager; }
    const EntityManager &getEntityManager() const { return m_entityManager; }

    StateManager *getManager() { return m_manager; }

private:
    friend class StateManager;
    std::string m_name;
    bool m_active;
    StateManager *m_manager;
    EntityManager m_entityManager;

    void setManager(StateManager *manager) { m_manager = manager; }
};