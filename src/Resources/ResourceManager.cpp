#include "../../include/Resources/ResourceManager.hpp"
#include <iostream>
#include <filesystem>

namespace Resources
{

    ResourceManager::ResourceManager()
        : m_basePath("resources/")
    {
        std::cout << "ResourceManager created" << std::endl;
    }

    ResourceManager::~ResourceManager()
    {
        clear();
        std::cout << "ResourceManager destroyed" << std::endl;
    }

    void ResourceManager::init(const std::string &basePath)
    {
        m_basePath = basePath;

        // Ensure the base path ends with a slash
        if (!m_basePath.empty() && m_basePath.back() != '/' && m_basePath.back() != '\\')
        {
            m_basePath += '/';
        }

        // Set default resource paths
        m_resourcePaths["textures"] = m_basePath + "textures/";
        m_resourcePaths["fonts"] = m_basePath + "fonts/";
        m_resourcePaths["sounds"] = m_basePath + "sounds/";
        m_resourcePaths["music"] = m_basePath + "music/";
        m_resourcePaths["shaders"] = m_basePath + "shaders/";

        std::cout << "ResourceManager initialized with base path: " << m_basePath << std::endl;
    }

    void ResourceManager::setResourcePath(const std::string &resourceType, const std::string &path)
    {
        m_resourcePaths[resourceType] = path;

        // Ensure the path ends with a slash
        if (!m_resourcePaths[resourceType].empty() &&
            m_resourcePaths[resourceType].back() != '/' &&
            m_resourcePaths[resourceType].back() != '\\')
        {
            m_resourcePaths[resourceType] += '/';
        }
    }

    std::string ResourceManager::getResourcePath(const std::string &resourceType) const
    {
        auto it = m_resourcePaths.find(resourceType);
        if (it != m_resourcePaths.end())
        {
            return it->second;
        }
        return m_basePath;
    }

    std::string ResourceManager::getFullPath(const std::string &resourceType, const std::string &filePath) const
    {
        // If the file path is absolute, return it as is
        if (std::filesystem::path(filePath).is_absolute())
        {
            return filePath;
        }

        // Otherwise, prepend the resource path
        auto path = getResourcePath(resourceType);
        return path + filePath;
    }

    sf::Texture &ResourceManager::loadTexture(const std::string &id, const std::string &filePath,
                                              bool smooth, bool repeated)
    {
        try
        {
            std::string fullPath = getFullPath("textures", filePath);
            auto texturePtr = std::make_unique<sf::Texture>();

            // Load the texture
            if (!texturePtr->loadFromFile(std::filesystem::path(fullPath)))
            {
                throw ResourceLoadException("Failed to load texture: " + fullPath);
            }

            // Apply settings
            texturePtr->setSmooth(smooth);
            texturePtr->setRepeated(repeated);

            // Store the texture
            auto inserted = m_textures.insert(std::make_pair(id, std::move(texturePtr)));
            std::cout << "Texture loaded: " << fullPath << std::endl;

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

    SpriteSheet &ResourceManager::loadSpriteSheet(const std::string &id, const std::string &textureId,
                                                  int frameWidth, int frameHeight, int frameCount)
    {
        // Get the texture
        sf::Texture *texture = nullptr;
        try
        {
            texture = &getTexture(textureId);
        }
        catch (const std::exception &e)
        {
            throw ResourceLoadException("Failed to load sprite sheet: Texture '" + textureId + "' not found - " + e.what());
        }

        // Create the frames
        std::vector<sf::IntRect> frames;
        sf::Vector2u textureSize = texture->getSize();
        int cols = static_cast<int>(textureSize.x) / frameWidth;

        for (int i = 0; i < frameCount; i++)
        {
            int col = i % cols;
            int row = i / cols;
            frames.emplace_back(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
        }

        // Create the sprite sheet
        SpriteSheet spriteSheet{texture, frames, {}};
        m_spriteSheets[id] = spriteSheet;

        std::cout << "Sprite sheet created: " << id << " with " << frameCount << " frames" << std::endl;
        return m_spriteSheets[id];
    }

    SpriteSheet &ResourceManager::loadSpriteSheet(const std::string &id, const std::string &textureId,
                                                  const std::vector<sf::IntRect> &frames,
                                                  const std::unordered_map<std::string, int> &namedFrames)
    {
        // Get the texture
        sf::Texture *texture = nullptr;
        try
        {
            texture = &getTexture(textureId);
        }
        catch (const std::exception &e)
        {
            throw ResourceLoadException("Failed to load sprite sheet: Texture '" + textureId + "' not found - " + e.what());
        }

        // Create the sprite sheet
        SpriteSheet spriteSheet{texture, frames, namedFrames};
        m_spriteSheets[id] = spriteSheet;

        std::cout << "Sprite sheet created: " << id << " with " << frames.size() << " frames" << std::endl;
        return m_spriteSheets[id];
    }

    SpriteSheet &ResourceManager::getSpriteSheet(const std::string &id)
    {
        auto it = m_spriteSheets.find(id);
        if (it == m_spriteSheets.end())
        {
            throw std::out_of_range("Sprite sheet does not exist: " + id);
        }

        return it->second;
    }

    sf::Font &ResourceManager::loadFont(const std::string &id, const std::string &filePath)
    {
        try
        {
            std::string fullPath = getFullPath("fonts", filePath);
            auto fontPtr = std::make_unique<sf::Font>();

            if (!fontPtr->openFromFile(std::filesystem::path(fullPath)))
            {
                throw ResourceLoadException("Failed to load font: " + fullPath);
            }

            auto inserted = m_fonts.insert(std::make_pair(id, std::move(fontPtr)));
            std::cout << "Font loaded: " << fullPath << std::endl;

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
        try
        {
            std::string fullPath = getFullPath("sounds", filePath);
            auto bufferPtr = std::make_unique<sf::SoundBuffer>();

            if (!bufferPtr->loadFromFile(std::filesystem::path(fullPath)))
            {
                throw ResourceLoadException("Failed to load sound: " + fullPath);
            }

            auto inserted = m_soundBuffers.insert(std::make_pair(id, std::move(bufferPtr)));
            std::cout << "Sound loaded: " << fullPath << std::endl;

            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load sound: " + filePath + " - " + e.what());
        }
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

    sf::Music &ResourceManager::loadMusic(const std::string &id, const std::string &filePath)
    {
        try
        {
            std::string fullPath = getFullPath("music", filePath);
            auto musicPtr = std::make_unique<sf::Music>();

            if (!musicPtr->openFromFile(std::filesystem::path(fullPath)))
            {
                throw ResourceLoadException("Failed to load music: " + fullPath);
            }

            auto inserted = m_music.insert(std::make_pair(id, std::move(musicPtr)));
            std::cout << "Music loaded: " << fullPath << std::endl;

            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load music: " + filePath + " - " + e.what());
        }
    }

    sf::Music &ResourceManager::getMusic(const std::string &id)
    {
        auto it = m_music.find(id);
        if (it == m_music.end())
        {
            throw std::out_of_range("Music does not exist: " + id);
        }

        return *it->second;
    }

    sf::Shader &ResourceManager::loadShader(const std::string &id, const std::string &vertexShaderPath,
                                            const std::string &fragmentShaderPath)
    {
        try
        {
            std::string vertexPath = getFullPath("shaders", vertexShaderPath);
            std::string fragmentPath = getFullPath("shaders", fragmentShaderPath);

            auto shaderPtr = std::make_unique<sf::Shader>();
            if (!shaderPtr->loadFromFile(std::filesystem::path(vertexPath),
                                         std::filesystem::path(fragmentPath)))
            {
                throw ResourceLoadException("Failed to load shader: " + vertexPath + ", " + fragmentPath);
            }

            auto inserted = m_shaders.insert(std::make_pair(id, std::move(shaderPtr)));
            std::cout << "Shader loaded: " << vertexPath << ", " << fragmentPath << std::endl;

            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load shader: " + vertexShaderPath + ", " + fragmentShaderPath + " - " + e.what());
        }
    }

    sf::Shader &ResourceManager::loadFragmentShader(const std::string &id, const std::string &fragmentShaderPath)
    {
        try
        {
            std::string fragmentPath = getFullPath("shaders", fragmentShaderPath);

            auto shaderPtr = std::make_unique<sf::Shader>();
            if (!shaderPtr->loadFromFile(std::filesystem::path(fragmentPath), sf::Shader::Type::Fragment))
            {
                throw ResourceLoadException("Failed to load fragment shader: " + fragmentPath);
            }

            auto inserted = m_shaders.insert(std::make_pair(id, std::move(shaderPtr)));
            std::cout << "Fragment shader loaded: " << fragmentPath << std::endl;

            return *inserted.first->second;
        }
        catch (const sf::Exception &e)
        {
            throw ResourceLoadException("Failed to load fragment shader: " + fragmentShaderPath + " - " + e.what());
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

    bool ResourceManager::hasSpriteSheet(const std::string &id) const
    {
        return m_spriteSheets.find(id) != m_spriteSheets.end();
    }

    bool ResourceManager::hasFont(const std::string &id) const
    {
        return m_fonts.find(id) != m_fonts.end();
    }

    bool ResourceManager::hasSoundBuffer(const std::string &id) const
    {
        return m_soundBuffers.find(id) != m_soundBuffers.end();
    }

    bool ResourceManager::hasMusic(const std::string &id) const
    {
        return m_music.find(id) != m_music.end();
    }

    bool ResourceManager::hasShader(const std::string &id) const
    {
        return m_shaders.find(id) != m_shaders.end();
    }

    bool ResourceManager::removeTexture(const std::string &id)
    {
        return m_textures.erase(id) > 0;
    }

    bool ResourceManager::removeSpriteSheet(const std::string &id)
    {
        return m_spriteSheets.erase(id) > 0;
    }

    bool ResourceManager::removeFont(const std::string &id)
    {
        return m_fonts.erase(id) > 0;
    }

    bool ResourceManager::removeSoundBuffer(const std::string &id)
    {
        return m_soundBuffers.erase(id) > 0;
    }

    bool ResourceManager::removeMusic(const std::string &id)
    {
        return m_music.erase(id) > 0;
    }

    bool ResourceManager::removeShader(const std::string &id)
    {
        return m_shaders.erase(id) > 0;
    }

    void ResourceManager::clear()
    {
        m_textures.clear();
        m_spriteSheets.clear();
        m_fonts.clear();
        m_soundBuffers.clear();
        m_music.clear();
        m_shaders.clear();

        std::cout << "All resources cleared" << std::endl;
    }

    void ResourceManager::loadAllResources(const std::string &directory,
                                           std::function<void(const std::string &, const std::string &)> loadCallback)
    {
        if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory))
        {
            std::cerr << "Directory not found: " << directory << std::endl;
            return;
        }

        // Initialize with the directory as base path
        init(directory);

        // Load all textures
        if (std::filesystem::exists(m_resourcePaths["textures"]) &&
            std::filesystem::is_directory(m_resourcePaths["textures"]))
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(m_resourcePaths["textures"]))
            {
                if (entry.is_regular_file())
                {
                    std::string path = entry.path().string();
                    std::string relativePath = path.substr(m_resourcePaths["textures"].length());
                    std::string id = entry.path().stem().string();

                    try
                    {
                        loadTexture(id, relativePath);
                        if (loadCallback)
                        {
                            loadCallback("texture", id);
                        }
                    }
                    catch (const ResourceLoadException &e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
        }

        // Load all fonts
        if (std::filesystem::exists(m_resourcePaths["fonts"]) &&
            std::filesystem::is_directory(m_resourcePaths["fonts"]))
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(m_resourcePaths["fonts"]))
            {
                if (entry.is_regular_file())
                {
                    std::string path = entry.path().string();
                    std::string relativePath = path.substr(m_resourcePaths["fonts"].length());
                    std::string id = entry.path().stem().string();

                    try
                    {
                        loadFont(id, relativePath);
                        if (loadCallback)
                        {
                            loadCallback("font", id);
                        }
                    }
                    catch (const ResourceLoadException &e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
        }

        // Load all sounds
        if (std::filesystem::exists(m_resourcePaths["sounds"]) &&
            std::filesystem::is_directory(m_resourcePaths["sounds"]))
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(m_resourcePaths["sounds"]))
            {
                if (entry.is_regular_file())
                {
                    std::string path = entry.path().string();
                    std::string relativePath = path.substr(m_resourcePaths["sounds"].length());
                    std::string id = entry.path().stem().string();

                    try
                    {
                        loadSoundBuffer(id, relativePath);
                        if (loadCallback)
                        {
                            loadCallback("sound", id);
                        }
                    }
                    catch (const ResourceLoadException &e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
        }

        // Load all music
        if (std::filesystem::exists(m_resourcePaths["music"]) &&
            std::filesystem::is_directory(m_resourcePaths["music"]))
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(m_resourcePaths["music"]))
            {
                if (entry.is_regular_file())
                {
                    std::string path = entry.path().string();
                    std::string relativePath = path.substr(m_resourcePaths["music"].length());
                    std::string id = entry.path().stem().string();

                    try
                    {
                        loadMusic(id, relativePath);
                        if (loadCallback)
                        {
                            loadCallback("music", id);
                        }
                    }
                    catch (const ResourceLoadException &e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
            }
        }

        std::cout << "All resources loaded from: " << directory << std::endl;
    }

} // namespace Resources