#include "../../include/States/State.hpp"

namespace Orenji
{

    State::State() : m_status(StateStatus::Running), m_stateMachine(nullptr)
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

    void State::setStateMachine(StateMachine *stateMachine)
    {
        m_stateMachine = stateMachine;
    }

    StateMachine *State::getStateMachine() const
    {
        return m_stateMachine;
    }

} // namespace Orenji