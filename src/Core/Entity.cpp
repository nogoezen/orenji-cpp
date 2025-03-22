#include "Core/Entity.hpp"
#include "Core/Component.hpp"
#include <algorithm>

namespace Orenji
{

    Entity::Entity(unsigned int id, const std::string &name)
        : m_id(id), m_name(name), m_active(true)
    {
    }

    Entity::~Entity()
    {
        m_components.clear();
        m_componentList.clear();
    }

    bool Entity::initialize()
    {
        for (auto &component : m_componentList)
        {
            if (!component->initialize())
            {
                return false;
            }
        }
        return true;
    }

    void Entity::update(float deltaTime)
    {
        if (!m_active)
        {
            return;
        }

        for (auto &component : m_componentList)
        {
            if (component->isActive())
            {
                component->update(deltaTime);
            }
        }
    }

    void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_active)
        {
            return;
        }

        states.transform *= getTransform();

        for (auto &component : m_componentList)
        {
            if (component->isActive())
            {
                component->draw(target, states);
            }
        }
    }

} // namespace Orenji