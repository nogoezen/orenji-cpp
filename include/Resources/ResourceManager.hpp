#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

namespace Resources
{

    /**
     * @brief Exception thrown when a resource cannot be loaded
     */
    class ResourceLoadException : public std::runtime_error
    {
    public:
        ResourceLoadException(const std::string &message) : std::runtime_error(message) {}
    };

    /**
     * @brief Manager class for all resources (textures, fonts, sounds, etc.)
     */
    class ResourceManager
    {
    public:
        /**
         * @brief Constructor
         */
        ResourceManager();

        /**
         * @brief Destructor
         */
        ~ResourceManager();

        /**
         * @brief Load a texture from file
         * @param id Resource identifier
         * @param filePath Path to the texture file
         * @return Reference to the loaded texture
         * @throws ResourceLoadException if the texture cannot be loaded
         */
        sf::Texture &loadTexture(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a texture by ID
         * @param id Resource identifier
         * @return Reference to the texture
         * @throws std::out_of_range if the texture does not exist
         */
        sf::Texture &getTexture(const std::string &id);

        /**
         * @brief Load a font from file
         * @param id Resource identifier
         * @param filePath Path to the font file
         * @return Reference to the loaded font
         * @throws ResourceLoadException if the font cannot be loaded
         */
        sf::Font &loadFont(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a font by ID
         * @param id Resource identifier
         * @return Reference to the font
         * @throws std::out_of_range if the font does not exist
         */
        sf::Font &getFont(const std::string &id);

        /**
         * @brief Load a sound buffer from file
         * @param id Resource identifier
         * @param filePath Path to the sound file
         * @return Reference to the loaded sound buffer
         * @throws ResourceLoadException if the sound buffer cannot be loaded
         */
        sf::SoundBuffer &loadSoundBuffer(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a sound buffer by ID
         * @param id Resource identifier
         * @return Reference to the sound buffer
         * @throws std::out_of_range if the sound buffer does not exist
         */
        sf::SoundBuffer &getSoundBuffer(const std::string &id);

        /**
         * @brief Load a shader from file
         * @param id Resource identifier
         * @param vertexShaderPath Path to the vertex shader file
         * @param fragmentShaderPath Path to the fragment shader file
         * @return Reference to the loaded shader
         * @throws ResourceLoadException if the shader cannot be loaded
         */
        sf::Shader &loadShader(const std::string &id, const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

        /**
         * @brief Get a shader by ID
         * @param id Resource identifier
         * @return Reference to the shader
         * @throws std::out_of_range if the shader does not exist
         */
        sf::Shader &getShader(const std::string &id);

        /**
         * @brief Check if a texture exists
         * @param id Resource identifier
         * @return true if the texture exists
         */
        bool hasTexture(const std::string &id) const;

        /**
         * @brief Check if a font exists
         * @param id Resource identifier
         * @return true if the font exists
         */
        bool hasFont(const std::string &id) const;

        /**
         * @brief Check if a sound buffer exists
         * @param id Resource identifier
         * @return true if the sound buffer exists
         */
        bool hasSoundBuffer(const std::string &id) const;

        /**
         * @brief Check if a shader exists
         * @param id Resource identifier
         * @return true if the shader exists
         */
        bool hasShader(const std::string &id) const;

        /**
         * @brief Remove a texture
         * @param id Resource identifier
         * @return true if the texture was removed
         */
        bool removeTexture(const std::string &id);

        /**
         * @brief Remove a font
         * @param id Resource identifier
         * @return true if the font was removed
         */
        bool removeFont(const std::string &id);

        /**
         * @brief Remove a sound buffer
         * @param id Resource identifier
         * @return true if the sound buffer was removed
         */
        bool removeSoundBuffer(const std::string &id);

        /**
         * @brief Remove a shader
         * @param id Resource identifier
         * @return true if the shader was removed
         */
        bool removeShader(const std::string &id);

        /**
         * @brief Clear all resources
         */
        void clear();

    private:
        std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
        std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
        std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
        std::unordered_map<std::string, std::unique_ptr<sf::Shader>> m_shaders;
    };

} // namespace Resources