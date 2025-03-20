#include "../../include/Core/TransformComponent.hpp"
#include "../../include/Core/Entity.hpp"

namespace Orenji
{
    TransformComponent::TransformComponent(Entity *entity)
        : Component(entity), m_position(0.0f, 0.0f), m_rotation(0.0f), m_scale(1.0f, 1.0f)
    {
    }

    void TransformComponent::setPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }

    void TransformComponent::setPosition(const sf::Vector2f &position)
    {
        m_position = position;
    }

    sf::Vector2f TransformComponent::getPosition() const
    {
        return m_position;
    }

    void TransformComponent::setRotation(float angle)
    {
        m_rotation = angle;
    }

    float TransformComponent::getRotation() const
    {
        return m_rotation;
    }

    void TransformComponent::setScale(float x, float y)
    {
        m_scale.x = x;
        m_scale.y = y;
    }

    void TransformComponent::setScale(const sf::Vector2f &scale)
    {
        m_scale = scale;
    }

    sf::Vector2f TransformComponent::getScale() const
    {
        return m_scale;
    }

    void TransformComponent::update(float deltaTime)
    {
        // Pas d'implémentation spécifique pour le moment
    }
} // namespace Orenji