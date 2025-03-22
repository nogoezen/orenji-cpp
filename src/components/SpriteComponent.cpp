#include "SpriteComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

// Static texture cache to avoid loading the same texture multiple times
static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureCache;

SpriteComponent::SpriteComponent(const std::string &texturePath)
    : m_texturePath(texturePath), m_layer(0), m_visible(true)
{
}

void SpriteComponent::initialize()
{
    if (!m_texturePath.empty())
    {
        setTexture(m_texturePath);
    }
}

void SpriteComponent::update(float deltaTime)
{
    if (auto entity = getEntity())
    {
        if (auto transform = entity->getComponent<TransformComponent>())
        {
            m_sprite.setPosition(transform->getPosition());
            m_sprite.setRotation(transform->getRotation());
            m_sprite.setScale(transform->getScale());
        }
    }
}

void SpriteComponent::render()
{
    if (!m_visible)
        return;

    // This is just a placeholder - actual rendering will depend on your render system
    // For example, you might add this sprite to a list of sprites to be rendered by a RenderSystem
}

void SpriteComponent::setTexture(const std::string &texturePath)
{
    m_texturePath = texturePath;

    // Check if the texture is already cached
    auto it = textureCache.find(texturePath);
    if (it != textureCache.end())
    {
        m_texture = it->second;
    }
    else
    {
        // Load the texture and cache it
        m_texture = std::make_shared<sf::Texture>();
        if (m_texture->loadFromFile(texturePath))
        {
            textureCache[texturePath] = m_texture;
        }
        else
        {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            m_texture.reset();
            return;
        }
    }

    m_sprite.setTexture(*m_texture);

    // Center the origin of the sprite
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}
