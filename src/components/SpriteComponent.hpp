#pragma once

#include "../core/Entity.hpp"
#include "TransformComponent.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <memory>

class SpriteComponent : public Component
{
public:
    SpriteComponent(const std::string &texturePath = "");
    ~SpriteComponent() = default;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;

    void setTexture(const std::string &texturePath);
    sf::Sprite &getSprite() { return m_sprite; }
    const sf::Sprite &getSprite() const { return m_sprite; }

    void setColor(const sf::Color &color) { m_sprite.setColor(color); }
    const sf::Color &getColor() const { return m_sprite.getColor(); }

    void setTextureRect(const sf::IntRect &rect) { m_sprite.setTextureRect(rect); }
    const sf::IntRect &getTextureRect() const { return m_sprite.getTextureRect(); }

    void setOrigin(float x, float y) { m_sprite.setOrigin(x, y); }
    void setOrigin(const sf::Vector2f &origin) { m_sprite.setOrigin(origin); }
    const sf::Vector2f &getOrigin() const { return m_sprite.getOrigin(); }

    void setLayer(int layer) { m_layer = layer; }
    int getLayer() const { return m_layer; }

    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

private:
    sf::Sprite m_sprite;
    std::shared_ptr<sf::Texture> m_texture;
    std::string m_texturePath;
    int m_layer;
    bool m_visible;
};
