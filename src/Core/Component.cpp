#include "../../include/Core/Component.hpp"

namespace Core
{

    Component::Component()
        : m_owner(nullptr), m_active(true)
    {
    }

    Component::~Component()
    {
    }

    void Component::init()
    {
        // Default implementation does nothing
    }

    void Component::update(float deltaTime)
    {
        // Default implementation does nothing
    }

    void Component::setOwner(Entity *owner)
    {
        m_owner = owner;
    }

    Entity *Component::getOwner() const
    {
        return m_owner;
    }

    bool Component::isActive() const
    {
        return m_active;
    }

    void Component::setActive(bool active)
    {
        m_active = active;
    }

} // namespace Core