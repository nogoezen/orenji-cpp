#include "State.h"

State::State(const std::string &name)
    : m_name(name), m_active(false), m_manager(nullptr)
{
}

State::~State()
{
}

void State::pause()
{
    m_active = false;
}

void State::resume()
{
    m_active = true;
}