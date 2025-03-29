#include "Graphics/Components/SpriteComponent.hpp"

namespace Graphics
{
    namespace Components
    {

        SpriteComponent::SpriteComponent()
            : m_layer(0), m_visible(true)
        {
        }

        SpriteComponent::SpriteComponent(std::shared_ptr<sf::Texture> texture)
            : m_layer(0), m_visible(true)
        {
            setTexture(texture);
        }

        void SpriteComponent::setTexture(std::shared_ptr<sf::Texture> texture)
        {
            if (texture)
            {
                m_sprite.setTexture(*texture);
            }
        }

        sf::Sprite &SpriteComponent::getSprite()
        {
            return m_sprite;
        }

        void SpriteComponent::setTextureRect(const sf::IntRect &rect)
        {
            m_sprite.setTextureRect(rect);
        }

        void SpriteComponent::setOrigin(float x, float y)
        {
            m_sprite.setOrigin(x, y);
        }

        void SpriteComponent::setColor(const sf::Color &color)
        {
            m_sprite.setColor(color);
        }

        void SpriteComponent::setLayer(int layer)
        {
            m_layer = layer;
        }

        int SpriteComponent::getLayer() const
        {
            return m_layer;
        }

        void SpriteComponent::setVisible(bool visible)
        {
            m_visible = visible;
        }

        bool SpriteComponent::isVisible() const
        {
            return m_visible;
        }

    }
} // namespace Graphics::Components