#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <stdexcept>

namespace Orenji
{

    /**
     * @class ResourceManager
     * @brief Manages game resources like textures, fonts, sounds, etc.
     */
    class ResourceManager
    {
    public:
        /**
         * @brief Get the singleton instance
         * @return Reference to the singleton instance
         */
        static ResourceManager &getInstance();

        /**
         * @brief Load a texture from file
         * @param id Identifier for the texture
         * @param filePath Path to the texture file
         */
        void loadTexture(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a texture by its id
         * @param id Identifier of the texture
         * @return Reference to the texture
         * @throws std::runtime_error if the texture is not found
         */
        sf::Texture &getTexture(const std::string &id);

        /**
         * @brief Load a font from file
         * @param id Identifier for the font
         * @param filePath Path to the font file
         */
        void loadFont(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a font by its id
         * @param id Identifier of the font
         * @return Reference to the font
         * @throws std::runtime_error if the font is not found
         */
        sf::Font &getFont(const std::string &id);

        /**
         * @brief Load a sound buffer from file
         * @param id Identifier for the sound buffer
         * @param filePath Path to the sound file
         */
        void loadSoundBuffer(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a sound buffer by its id
         * @param id Identifier of the sound buffer
         * @return Reference to the sound buffer
         * @throws std::runtime_error if the sound buffer is not found
         */
        sf::SoundBuffer &getSoundBuffer(const std::string &id);

        /**
         * @brief Load a music from file
         * @param id Identifier for the music
         * @param filePath Path to the music file
         */
        void loadMusic(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a music by its id
         * @param id Identifier of the music
         * @return Shared pointer to the music
         * @throws std::runtime_error if the music is not found
         */
        std::shared_ptr<sf::Music> getMusic(const std::string &id);

        /**
         * @brief Clear all resources
         */
        void clear();

    private:
        /**
         * @brief Constructor
         */
        ResourceManager() = default;

        /**
         * @brief Destructor
         */
        ~ResourceManager() = default;

        /**
         * @brief Copy constructor (deleted)
         */
        ResourceManager(const ResourceManager &) = delete;

        /**
         * @brief Assignment operator (deleted)
         */
        ResourceManager &operator=(const ResourceManager &) = delete;

        std::unordered_map<std::string, sf::Texture> m_textures;
        std::unordered_map<std::string, sf::Font> m_fonts;
        std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
        std::unordered_map<std::string, std::shared_ptr<sf::Music>> m_music;
    };

} // namespace Orenji