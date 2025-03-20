#include "../../include/States/State.hpp"

namespace Orenji
{

    State::State() : m_status(StateStatus::Running), m_parent(nullptr)
    {
    }

    State::~State()
    {
    }

    State::StateStatus State::getStatus() const
    {
        return m_status;
    }

    void State::setStatus(StateStatus status)
    {
        m_status = status;
    }

    void State::setParent(StateMachine *parent)
    {
        m_parent = parent;
    }

    StateMachine *State::getParent() const
    {
        return m_parent;
    }

} // namespace Orenji