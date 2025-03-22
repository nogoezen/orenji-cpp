#include "../../include/Resources/ResourceManager.hpp"
#include <iostream>
#include <filesystem>

namespace Resources
{

    ResourceManager::ResourceManager()
    {
        std::cout << "ResourceManager created" << std::endl;
    }

    ResourceManager::~ResourceManager()
    {
        clear();
        std::cout << "ResourceManager destroyed" << std::endl;
    }

    sf::Texture &ResourceManager::loadTexture(const std::string &id, const std::string &filePath)
    {
        try
        {
            auto texturePtr = std::make_unique<sf::Texture>(std::filesystem::path(filePath));
            auto inserted = m_textures.insert(std::make_pair(id, std::move(texturePtr)));
            std::cout << "Texture loaded: " << filePath << std::endl;
            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load texture: " + filePath + " - " + e.what());
        }
    }

    sf::Texture &ResourceManager::getTexture(const std::string &id)
    {
        auto it = m_textures.find(id);
        if (it == m_textures.end())
        {
            throw std::out_of_range("Texture does not exist: " + id);
        }

        return *it->second;
    }

    sf::Font &ResourceManager::loadFont(const std::string &id, const std::string &filePath)
    {
        try
        {
            auto fontPtr = std::make_unique<sf::Font>();
            if (!fontPtr->openFromFile(filePath))
            {
                throw ResourceLoadException("Failed to load font: " + filePath);
            }
            auto inserted = m_fonts.insert(std::make_pair(id, std::move(fontPtr)));
            std::cout << "Font loaded: " << filePath << std::endl;
            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load font: " + filePath + " - " + e.what());
        }
    }

    sf::Font &ResourceManager::getFont(const std::string &id)
    {
        auto it = m_fonts.find(id);
        if (it == m_fonts.end())
        {
            throw std::out_of_range("Font does not exist: " + id);
        }

        return *it->second;
    }

    sf::SoundBuffer &ResourceManager::loadSoundBuffer(const std::string &id, const std::string &filePath)
    {
        auto bufferPtr = std::make_unique<sf::SoundBuffer>();

        if (!bufferPtr->loadFromFile(std::filesystem::path(filePath)))
        {
            throw ResourceLoadException("Failed to load sound: " + filePath);
        }

        auto inserted = m_soundBuffers.insert(std::make_pair(id, std::move(bufferPtr)));
        std::cout << "Sound loaded: " << filePath << std::endl;

        return *inserted.first->second;
    }

    sf::SoundBuffer &ResourceManager::getSoundBuffer(const std::string &id)
    {
        auto it = m_soundBuffers.find(id);
        if (it == m_soundBuffers.end())
        {
            throw std::out_of_range("Sound buffer does not exist: " + id);
        }

        return *it->second;
    }

    sf::Shader &ResourceManager::loadShader(const std::string &id, const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
    {
        try
        {
            auto shaderPtr = std::make_unique<sf::Shader>(
                std::filesystem::path(vertexShaderPath),
                std::filesystem::path(fragmentShaderPath));
            auto inserted = m_shaders.insert(std::make_pair(id, std::move(shaderPtr)));
            std::cout << "Shader loaded: " << vertexShaderPath << ", " << fragmentShaderPath << std::endl;
            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load shader: " + vertexShaderPath + ", " + fragmentShaderPath + " - " + e.what());
        }
    }

    sf::Shader &ResourceManager::getShader(const std::string &id)
    {
        auto it = m_shaders.find(id);
        if (it == m_shaders.end())
        {
            throw std::out_of_range("Shader does not exist: " + id);
        }

        return *it->second;
    }

    bool ResourceManager::hasTexture(const std::string &id) const
    {
        return m_textures.find(id) != m_textures.end();
    }

    bool ResourceManager::hasFont(const std::string &id) const
    {
        return m_fonts.find(id) != m_fonts.end();
    }

    bool ResourceManager::hasSoundBuffer(const std::string &id) const
    {
        return m_soundBuffers.find(id) != m_soundBuffers.end();
    }

    bool ResourceManager::hasShader(const std::string &id) const
    {
        return m_shaders.find(id) != m_shaders.end();
    }

    bool ResourceManager::removeTexture(const std::string &id)
    {
        return m_textures.erase(id) > 0;
    }

    bool ResourceManager::removeFont(const std::string &id)
    {
        return m_fonts.erase(id) > 0;
    }

    bool ResourceManager::removeSoundBuffer(const std::string &id)
    {
        return m_soundBuffers.erase(id) > 0;
    }

    bool ResourceManager::removeShader(const std::string &id)
    {
        return m_shaders.erase(id) > 0;
    }

    void ResourceManager::clear()
    {
        m_textures.clear();
        m_fonts.clear();
        m_soundBuffers.clear();
        m_shaders.clear();

        std::cout << "All resources cleared" << std::endl;
    }

} // namespace Resources