#include "Scene/SpriteNode.hpp"

namespace Orenji
{
    SpriteNode::SpriteNode(const std::string &name)
        : SceneNode(name)
    {
    }

    SpriteNode::SpriteNode(const sf::Texture &texture, const std::string &name)
        : SceneNode(name)
    {
        m_sprite.setTexture(texture);
    }

    SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect, const std::string &name)
        : SceneNode(name)
    {
        m_sprite.setTexture(texture);
        m_sprite.setTextureRect(textureRect);
    }

    void SpriteNode::setTexture(const sf::Texture &texture)
    {
        m_sprite.setTexture(texture);
    }

    void SpriteNode::setTextureRect(const sf::IntRect &rect)
    {
        m_sprite.setTextureRect(rect);
    }

    void SpriteNode::setColor(const sf::Color &color)
    {
        m_sprite.setColor(color);
    }

    sf::Sprite &SpriteNode::getSprite()
    {
        return m_sprite;
    }

    const sf::Sprite &SpriteNode::getSprite() const
    {
        return m_sprite;
    }

    void SpriteNode::centerOrigin()
    {
        sf::FloatRect bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }

    void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }
} // namespace Orenji