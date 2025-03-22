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

    sf::Vector2f Entity::getPosition() const
    {
        return m_transform.getPosition();
    }

    void Entity::setPosition(const sf::Vector2f &position)
    {
        m_transform.setPosition(position);
    }

    void Entity::setPosition(float x, float y)
    {
        m_transform.setPosition(sf::Vector2f(x, y));
    }

    void Entity::move(const sf::Vector2f &offset)
    {
        m_transform.move(offset);
    }

    void Entity::move(float offsetX, float offsetY)
    {
        m_transform.move(sf::Vector2f(offsetX, offsetY));
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

    sf::Vector2f Entity::getScale() const
    {
        return m_transform.getScale();
    }

    void Entity::setScale(const sf::Vector2f &scale)
    {
        m_transform.setScale(scale);
    }

    void Entity::setScale(float x, float y)
    {
        m_transform.setScale(sf::Vector2f(x, y));
    }

    sf::Transformable &Entity::getTransform()
    {
        return m_transform;
    }

    const sf::Transformable &Entity::getTransform() const
    {
        return m_transform;
    }

    void Entity::removeAllComponents()
    {
        m_components.clear();
    }

} // namespace Orenji