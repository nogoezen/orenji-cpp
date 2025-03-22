#include "State/StateManager.hpp"
#include <iostream>

namespace Orenji
{

    StateManager::StateManager()
        : m_isProcessingChanges(false)
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

    bool StateManager::popState()
    {
        if (m_isProcessingChanges)
        {
            m_pendingChanges.push_back({Action::Pop, "", false});
            return true;
        }

        if (m_states.empty())
        {
            return false;
        }

        // Désactive et supprime l'état actuel
        m_states.back()->onInactive();
        m_states.pop_back();

        // Active le nouvel état en haut de la pile
        if (!m_states.empty())
        {
            m_states.back()->onActive();
        }

        return true;
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
        if (m_isProcessingChanges)
        {
            m_pendingChanges.push_back({Action::Clear, "", false});
            return;
        }

        // Désactive et supprime tous les états
        for (auto &state : m_states)
        {
            state->onInactive();
        }
        m_states.clear();
    }

    void StateManager::update(float deltaTime)
    {
        // Met à jour uniquement l'état au sommet
        if (!m_states.empty())
        {
            m_states.back()->update(deltaTime);
        }
    }

    void StateManager::handleEvent(const sf::Event &event)
    {
        // Gère les événements uniquement pour l'état au sommet
        if (!m_states.empty())
        {
            m_states.back()->handleEvent(event);
        }
    }

    void StateManager::render(sf::RenderWindow &window)
    {
        // Dessine tous les états visibles, du bas vers le haut
        // Ceci permet aux états transparents de laisser voir les états en dessous
        for (auto &state : m_states)
        {
            state->render(window);
        }
    }

    bool StateManager::isEmpty() const
    {
        return m_states.empty();
    }

    State *StateManager::getCurrentState()
    {
        return m_states.empty() ? nullptr : m_states.back().get();
    }

    void StateManager::processStateChanges()
    {
        // Marque que nous sommes en train de traiter des changements
        m_isProcessingChanges = true;

        // Vérifie si l'état actuel doit être retiré
        if (!m_states.empty() && m_states.back()->shouldRemove())
        {
            popState();
        }

        // Traite les changements en attente
        for (const auto &change : m_pendingChanges)
        {
            switch (change.action)
            {
            case Action::Push:
                if (change.replace && !m_states.empty())
                {
                    // Si on remplace l'état actuel, désactiver et supprimer l'ancien
                    m_states.back()->onInactive();
                    m_states.pop_back();
                }

                // Vérifie si la factory existe
                auto it = m_factories.find(change.stateId);
                if (it != m_factories.end())
                {
                    // Crée un nouvel état
                    StatePtr state = it->second();
                    if (state && state->initialize())
                    {
                        // Désactive l'état actuel si nécessaire
                        if (!m_states.empty() && !change.replace)
                        {
                            m_states.back()->onInactive();
                        }

                        // Ajoute le nouvel état et l'active
                        m_states.push_back(std::move(state));
                        m_states.back()->onActive();
                    }
                    else
                    {
                        std::cerr << "StateManager: Échec de l'initialisation de l'état: " << change.stateId << std::endl;
                    }
                }
                else
                {
                    std::cerr << "StateManager: État inconnu: " << change.stateId << std::endl;
                }
                break;

            case Action::Pop:
                if (!m_states.empty())
                {
                    // Désactive et supprime l'état actuel
                    m_states.back()->onInactive();
                    m_states.pop_back();

                    // Active le nouvel état en haut de la pile
                    if (!m_states.empty())
                    {
                        m_states.back()->onActive();
                    }
                }
                break;

            case Action::Clear:
                // Désactive et supprime tous les états
                for (auto &state : m_states)
                {
                    state->onInactive();
                }
                m_states.clear();
                break;
            }
        }

        // Efface la liste des changements en attente
        m_pendingChanges.clear();

        // Marque que nous avons terminé de traiter les changements
        m_isProcessingChanges = false;
    }

} // namespace Orenji