#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

namespace Orenji
{
    class TransformComponent : public Component
    {
    public:
        TransformComponent(Entity *entity = nullptr);
        ~TransformComponent() override = default;

        // Position
        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f &position);
        sf::Vector2f getPosition() const;

        // Rotation
        void setRotation(float angle);
        float getRotation() const;

        // Échelle
        void setScale(float x, float y);
        void setScale(const sf::Vector2f &scale);
        sf::Vector2f getScale() const;

        virtual void update(float deltaTime) override;

    private:
        sf::Vector2f m_position;
        float m_rotation;
        sf::Vector2f m_scale;
    };
} // namespace Orenji