#include "States/State.hpp"

namespace Orenji
{
    State::State(StateMachine &machine)
        : m_machine(machine)
    {
    }
} // namespace Orenji