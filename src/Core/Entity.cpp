#include "../../include/Core/Entity.hpp"
#include <algorithm>

namespace Core
{

    Entity::Entity(unsigned int id, const std::string &name)
        : m_id(id), m_name(name), m_active(true)
    {
    }

    Entity::~Entity()
    {
        removeAllComponents();
    }

    unsigned int Entity::getId() const
    {
        return m_id;
    }

    const std::string &Entity::getName() const
    {
        return m_name;
    }

    void Entity::setName(const std::string &name)
    {
        m_name = name;
    }

    void Entity::removeAllComponents()
    {
        m_components.clear();
    }

    void Entity::setActive(bool active)
    {
        m_active = active;
    }

    bool Entity::isActive() const
    {
        return m_active;
    }

} // namespace Core