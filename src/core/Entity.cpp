#include "../../include/Core/Entity.hpp"

namespace Orenji
{

    Entity::Entity(unsigned int id, const std::string &name)
        : m_id(id), m_name(name)
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

    std::string Entity::getName() const
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

} // namespace Orenji