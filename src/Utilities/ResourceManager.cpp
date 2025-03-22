#include "../../include/Utilities/ResourceManager.hpp"
#include <iostream>

namespace Orenji
{

    ResourceManager &ResourceManager::getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    void ResourceManager::loadTexture(const std::string &id, const std::string &filePath)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(filePath))
        {
            std::cerr << "Failed to load texture: " << filePath << std::endl;
            throw std::runtime_error("Failed to load texture: " + filePath);
        }

        m_textures[id] = std::move(texture);
    }

    sf::Texture &ResourceManager::getTexture(const std::string &id)
    {
        auto it = m_textures.find(id);
        if (it == m_textures.end())
        {
            throw std::runtime_error("Texture not found: " + id);
        }

        return it->second;
    }

    void ResourceManager::loadFont(const std::string &id, const std::string &filePath)
    {
        sf::Font font;
        if (!font.loadFromFile(filePath))
        {
            std::cerr << "Failed to load font: " << filePath << std::endl;
            throw std::runtime_error("Failed to load font: " + filePath);
        }

        m_fonts[id] = std::move(font);
    }

    sf::Font &ResourceManager::getFont(const std::string &id)
    {
        auto it = m_fonts.find(id);
        if (it == m_fonts.end())
        {
            throw std::runtime_error("Font not found: " + id);
        }

        return it->second;
    }

    void ResourceManager::loadSoundBuffer(const std::string &id, const std::string &filePath)
    {
        sf::SoundBuffer soundBuffer;
        if (!soundBuffer.loadFromFile(filePath))
        {
            std::cerr << "Failed to load sound buffer: " << filePath << std::endl;
            throw std::runtime_error("Failed to load sound buffer: " + filePath);
        }

        m_soundBuffers[id] = std::move(soundBuffer);
    }

    sf::SoundBuffer &ResourceManager::getSoundBuffer(const std::string &id)
    {
        auto it = m_soundBuffers.find(id);
        if (it == m_soundBuffers.end())
        {
            throw std::runtime_error("Sound buffer not found: " + id);
        }

        return it->second;
    }

    void ResourceManager::loadMusic(const std::string &id, const std::string &filePath)
    {
        auto music = std::make_shared<sf::Music>();
        if (!music->openFromFile(filePath))
        {
            std::cerr << "Failed to load music: " << filePath << std::endl;
            throw std::runtime_error("Failed to load music: " + filePath);
        }

        m_music[id] = music;
    }

    std::shared_ptr<sf::Music> ResourceManager::getMusic(const std::string &id)
    {
        auto it = m_music.find(id);
        if (it == m_music.end())
        {
            throw std::runtime_error("Music not found: " + id);
        }

        return it->second;
    }

    void ResourceManager::clear()
    {
        m_textures.clear();
        m_fonts.clear();
        m_soundBuffers.clear();
        m_music.clear();
    }

} // namespace Orenji