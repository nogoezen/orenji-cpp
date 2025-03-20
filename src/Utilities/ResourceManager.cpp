#include "../../include/Utilities/ResourceManager.hpp"
#include <iostream>

namespace Orenji
{

    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
        clearAllResources();
    }

    ResourceManager &ResourceManager::getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    sf::Texture *ResourceManager::loadTexture(const std::string &name, const std::string &filename)
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            return it->second.get();
        }

        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(filename))
        {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            return nullptr;
        }

        sf::Texture *result = texture.get();
        m_textures[name] = std::move(texture);
        return result;
    }

    sf::Texture *ResourceManager::getTexture(const std::string &name)
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    sf::Font *ResourceManager::loadFont(const std::string &name, const std::string &filename)
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
        {
            return it->second.get();
        }

        auto font = std::make_unique<sf::Font>();
        if (!font->loadFromFile(filename))
        {
            std::cerr << "Failed to load font: " << filename << std::endl;
            return nullptr;
        }

        sf::Font *result = font.get();
        m_fonts[name] = std::move(font);
        return result;
    }

    sf::Font *ResourceManager::getFont(const std::string &name)
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    sf::SoundBuffer *ResourceManager::loadSoundBuffer(const std::string &name, const std::string &filename)
    {
        auto it = m_soundBuffers.find(name);
        if (it != m_soundBuffers.end())
        {
            return it->second.get();
        }

        auto soundBuffer = std::make_unique<sf::SoundBuffer>();
        if (!soundBuffer->loadFromFile(filename))
        {
            std::cerr << "Failed to load sound: " << filename << std::endl;
            return nullptr;
        }

        sf::SoundBuffer *result = soundBuffer.get();
        m_soundBuffers[name] = std::move(soundBuffer);
        return result;
    }

    sf::SoundBuffer *ResourceManager::getSoundBuffer(const std::string &name)
    {
        auto it = m_soundBuffers.find(name);
        if (it != m_soundBuffers.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void ResourceManager::clearAllResources()
    {
        clearTextures();
        clearFonts();
        clearSounds();
    }

    void ResourceManager::clearTextures()
    {
        m_textures.clear();
    }

    void ResourceManager::clearFonts()
    {
        m_fonts.clear();
    }

    void ResourceManager::clearSounds()
    {
        m_soundBuffers.clear();
    }

} // namespace Orenji
