#include "../../include/States/State.hpp"
#include "../../include/States/StateMachine.hpp"

namespace Orenji
{

    State::State(StateMachine &stateMachine) : m_stateMachine(stateMachine)
    {
        // Initialize the state with a reference to the state machine
    }

} // namespace Orenji