#include "State/StateManager.hpp"
#include <iostream>

namespace Orenji
{

    StateManager::StateManager()
        : m_pendingChange(false), m_pendingStateId("")
    {
    }

    StateManager::~StateManager()
    {
        clearStates();
    }

    bool StateManager::pushState(const std::string &id)
    {
        // Vérifie si la factory existe
        auto it = m_factories.find(id);
        if (it == m_factories.end())
        {
            std::cerr << "StateManager::pushState: État inconnu: " << id << std::endl;
            return false;
        }

        // Crée un nouvel état
        StatePtr state = it->second();
        if (!state)
        {
            std::cerr << "StateManager::pushState: Impossible de créer l'état: " << id << std::endl;
            return false;
        }

        // Initialise l'état
        if (!state->initialize())
        {
            std::cerr << "StateManager::pushState: Échec de l'initialisation de l'état: " << id << std::endl;
            return false;
        }

        // Désactive l'état actuel s'il existe
        if (!m_states.empty())
        {
            m_states.top()->onInactive();
        }

        // Active le nouvel état
        state->onActive();

        // Ajoute l'état à la pile
        m_states.push(std::move(state));

        return true;
    }

    void StateManager::popState()
    {
        if (m_states.empty())
        {
            return;
        }

        // Retire l'état du sommet
        m_states.pop();

        // Active l'état au sommet s'il existe
        if (!m_states.empty())
        {
            m_states.top()->onActive();
        }
    }

    bool StateManager::changeState(const std::string &id)
    {
        // Retire l'état actuel s'il existe
        if (!m_states.empty())
        {
            m_states.pop();
        }

        // Pousse le nouvel état
        return pushState(id);
    }

    void StateManager::clearStates()
    {
        // Vide la pile d'états
        while (!m_states.empty())
        {
            m_states.pop();
        }
    }

    void StateManager::update(float deltaTime)
    {
        if (m_states.empty())
        {
            return;
        }

        // Met à jour l'état au sommet
        m_states.top()->update(deltaTime);

        // Vérifie si l'état doit être retiré
        if (m_states.top()->shouldRemove())
        {
            popState();
        }
    }

    void StateManager::handleEvent(const sf::Event &event)
    {
        if (m_states.empty())
        {
            return;
        }

        // Traite l'événement pour l'état au sommet
        m_states.top()->handleEvent(event);
    }

    void StateManager::render(sf::RenderWindow &window)
    {
        if (m_states.empty())
        {
            return;
        }

        // Dessine l'état au sommet
        m_states.top()->render(window);
    }

    bool StateManager::isEmpty() const
    {
        return m_states.empty();
    }

} // namespace Orenji