#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <map>

namespace Orenji {

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Singleton pattern
    static ResourceManager& getInstance();

    // Texture loading and retrieval
    sf::Texture* loadTexture(const std::string& name, const std::string& filename);
    sf::Texture* getTexture(const std::string& name);

    // Font loading and retrieval
    sf::Font* loadFont(const std::string& name, const std::string& filename);
    sf::Font* getFont(const std::string& name);

    // Sound loading and retrieval
    sf::SoundBuffer* loadSoundBuffer(const std::string& name, const std::string& filename);
    sf::SoundBuffer* getSoundBuffer(const std::string& name);

    // Resource cleanup
    void clearAllResources();
    void clearTextures();
    void clearFonts();
    void clearSounds();

private:
    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
};

} // namespace Orenji 