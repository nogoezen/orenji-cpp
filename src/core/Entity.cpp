#include "Core/Entity.hpp"
#include "Core/Component.hpp"
#include <algorithm>

namespace Orenji
{
    Entity::Entity(const std::string &name)
        : m_name(name), m_destroyed(false), m_visible(true), m_active(true)
    {
    }

    Entity::~Entity()
    {
        m_components.clear();
    }

    Entity::Entity(const Entity &other)
        : m_name(other.m_name + "_copy"), m_destroyed(false),
          m_visible(other.m_visible), m_active(other.m_active)
    {
        // Ne pas copier les composants, mais ils peuvent être ajoutés ensuite
    }

    void Entity::initialize()
    {
        for (auto &component : m_components)
        {
            component.second->initialize();
        }
    }

    void Entity::update(float dt)
    {
        for (auto &component : m_components)
        {
            if (component.second->isActive())
                component.second->update(dt);
        }
    }

    void Entity::draw(sf::RenderTarget &target)
    {
        if (!m_visible)
            return;

        for (auto &component : m_components)
        {
            if (component.second->isVisible())
                component.second->draw(target);
        }
    }

    void Entity::destroy()
    {
        m_destroyed = true;
    }

    bool Entity::isDestroyed() const
    {
        return m_destroyed;
    }

    bool Entity::hasComponent(const std::string &id) const
    {
        return m_components.find(id) != m_components.end();
    }

    Component *Entity::getComponent(const std::string &id)
    {
        auto it = m_components.find(id);
        if (it != m_components.end())
            return it->second.get();
        return nullptr;
    }

    void Entity::setPosition(float x, float y)
    {
        m_transform.setPosition(sf::Vector2f(x, y));
    }

    void Entity::setPosition(const sf::Vector2f &position)
    {
        m_transform.setPosition(position);
    }

    sf::Vector2f Entity::getPosition() const
    {
        return m_transform.getPosition();
    }

    void Entity::move(float x, float y)
    {
        m_transform.move(sf::Vector2f(x, y));
    }

    void Entity::move(const sf::Vector2f &offset)
    {
        m_transform.move(offset);
    }

    float Entity::getRotation() const
    {
        return m_transform.getRotation().asDegrees();
    }

    void Entity::setRotation(float angle)
    {
        m_transform.setRotation(sf::degrees(angle));
    }

    void Entity::rotate(float angle)
    {
        m_transform.rotate(sf::degrees(angle));
    }

    void Entity::setScale(float factorX, float factorY)
    {
        m_transform.setScale(sf::Vector2f(factorX, factorY));
    }

    void Entity::setScale(const sf::Vector2f &factors)
    {
        m_transform.setScale(factors);
    }

    sf::Vector2f Entity::getScale() const
    {
        return m_transform.getScale();
    }

    void Entity::scale(float factorX, float factorY)
    {
        m_transform.scale(sf::Vector2f(factorX, factorY));
    }

    void Entity::scale(const sf::Vector2f &factors)
    {
        m_transform.scale(factors);
    }

    sf::Transform Entity::getTransform() const
    {
        return m_transform.getTransform();
    }

    sf::Transform Entity::getInverseTransform() const
    {
        return m_transform.getInverseTransform();
    }

    const sf::Transformable &Entity::getTransformable() const
    {
        return m_transform;
    }

    sf::Transformable &Entity::getTransformable()
    {
        return m_transform;
    }

    void Entity::setName(const std::string &name)
    {
        m_name = name;
    }

    const std::string &Entity::getName() const
    {
        return m_name;
    }

    void Entity::setVisible(bool visible)
    {
        m_visible = visible;
    }

    bool Entity::isVisible() const
    {
        return m_visible;
    }

    void Entity::setActive(bool active)
    {
        m_active = active;
    }

    bool Entity::isActive() const
    {
        return m_active;
    }
}