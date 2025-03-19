#include "Entity.h"

Entity::Entity(EntityId id, std::string name)
    : m_id(id), m_name(std::move(name)), m_active(true), m_manager(nullptr)
{
}

Entity::~Entity()
{
    removeAllComponents();
}

void Entity::removeAllComponents()
{
    m_components.clear();
}