#include "States/StateMachine.hpp"
#include <algorithm>

namespace Orenji
{
    StateMachine::StateMachine()
    {
    }

    StateMachine::~StateMachine()
    {
        clearStates();
    }

    void StateMachine::pushState(const std::string &stateName)
    {
        PendingChange change;
        change.action = PendingChange::Action::Push;
        change.stateName = stateName;

        m_pendingChanges.push_back(change);
    }

    void StateMachine::popState()
    {
        PendingChange change;
        change.action = PendingChange::Action::Pop;

        m_pendingChanges.push_back(change);
    }

    void StateMachine::changeState(const std::string &stateName)
    {
        PendingChange change;
        change.action = PendingChange::Action::Change;
        change.stateName = stateName;

        m_pendingChanges.push_back(change);
    }

    void StateMachine::clearStates()
    {
        PendingChange change;
        change.action = PendingChange::Action::Clear;

        m_pendingChanges.push_back(change);
    }

    bool StateMachine::isEmpty() const
    {
        return m_states.empty();
    }

    void StateMachine::update(float dt)
    {
        // Appliquer les changements en attente
        applyPendingChanges();

        if (m_states.empty())
            return;

        // Mettre à jour l'état actif (sommet de la pile)
        m_states.back()->update(dt);

        // Mettre à jour les états en-dessous si ils sont visibles et non exclusifs
        for (auto it = m_states.rbegin() + 1; it != m_states.rend(); ++it)
        {
            if ((*it)->isTransparent() && !((*(it - 1))->isExclusive()))
            {
                (*it)->update(dt);
            }
            else
            {
                break;
            }
        }
    }

    void StateMachine::handleEvent(const sf::Event &event)
    {
        // Appliquer les changements en attente
        applyPendingChanges();

        if (m_states.empty())
            return;

        // Traiter les événements pour l'état actif (sommet de la pile)
        m_states.back()->handleEvent(event);

        // Traiter les événements pour les états en-dessous si ils sont visibles et non exclusifs
        for (auto it = m_states.rbegin() + 1; it != m_states.rend(); ++it)
        {
            if ((*it)->isTransparent() && !((*(it - 1))->isExclusive()))
            {
                (*it)->handleEvent(event);
            }
            else
            {
                break;
            }
        }
    }

    void StateMachine::draw(sf::RenderTarget &target)
    {
        // Appliquer les changements en attente
        applyPendingChanges();

        if (m_states.empty())
            return;

        // Collecter les états à afficher (l'état du dessous est affiché d'abord)
        std::vector<StatePtr> statesToRender;
        for (auto it = m_states.rbegin(); it != m_states.rend(); ++it)
        {
            statesToRender.push_back(*it);
            if (!(*it)->isTransparent())
                break;
        }

        // Dessiner les états dans l'ordre inverse (de bas en haut)
        for (auto it = statesToRender.rbegin(); it != statesToRender.rend(); ++it)
        {
            (*it)->draw(target);
        }
    }

    void StateMachine::applyPendingChanges()
    {
        for (auto &change : m_pendingChanges)
        {
            switch (change.action)
            {
            case PendingChange::Action::Push:
            {
                // Créer et empiler l'état
                auto it = m_factories.find(change.stateName);
                if (it != m_factories.end())
                {
                    auto state = it->second();
                    if (!m_states.empty())
                        m_states.back()->onExit();
                    m_states.push_back(state);
                    state->onEnter();
                }
                break;
            }
            case PendingChange::Action::Pop:
            {
                // Dépiler l'état
                if (!m_states.empty())
                {
                    m_states.back()->onExit();
                    m_states.pop_back();
                    if (!m_states.empty())
                        m_states.back()->onEnter();
                }
                break;
            }
            case PendingChange::Action::Change:
            {
                // Remplacer l'état du dessus
                auto it = m_factories.find(change.stateName);
                if (it != m_factories.end() && !m_states.empty())
                {
                    m_states.back()->onExit();
                    m_states.pop_back();
                    auto state = it->second();
                    m_states.push_back(state);
                    state->onEnter();
                }
                break;
            }
            case PendingChange::Action::Clear:
            {
                // Vider la pile d'états
                while (!m_states.empty())
                {
                    m_states.back()->onExit();
                    m_states.pop_back();
                }
                break;
            }
            }
        }

        m_pendingChanges.clear();
    }

} // namespace Orenji