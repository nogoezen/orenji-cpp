#pragma once

#include "../core/Entity.hpp"
#include <SFML/System/Vector2.hpp>

class TransformComponent : public Component
{
public:
    TransformComponent(float x = 0.0f, float y = 0.0f, float rotation = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f)
        : m_position(x, y), m_rotation(rotation), m_scale(scaleX, scaleY) {}

    void initialize() override {}
    void update(float deltaTime) override {}
    void render() override {}

    void setPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }
    void setPosition(const sf::Vector2f &position) { m_position = position; }
    const sf::Vector2f &getPosition() const { return m_position; }

    void move(float x, float y)
    {
        m_position.x += x;
        m_position.y += y;
    }
    void move(const sf::Vector2f &offset) { m_position += offset; }

    void setRotation(float rotation) { m_rotation = rotation; }
    float getRotation() const { return m_rotation; }
    void rotate(float angle) { m_rotation += angle; }

    void setScale(float x, float y)
    {
        m_scale.x = x;
        m_scale.y = y;
    }
    void setScale(const sf::Vector2f &scale) { m_scale = scale; }
    const sf::Vector2f &getScale() const { return m_scale; }

private:
    sf::Vector2f m_position;
    float m_rotation;
    sf::Vector2f m_scale;
};
