#include "Core/Component.hpp"

namespace Orenji
{
    Component::Component(const std::string &id)
        : m_id(id), m_parent(nullptr)
    {
    }
} // namespace Orenji