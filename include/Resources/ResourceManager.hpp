#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <functional>

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
     * @brief Structure representing sprite sheet information
     */
    struct SpriteSheet
    {
        sf::Texture *texture;                             // Texture containing the sprite sheet
        std::vector<sf::IntRect> frames;                  // Frames within the sprite sheet
        std::unordered_map<std::string, int> namedFrames; // Named frames for easy access
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
         * @brief Initialize the resource manager with default paths
         * @param basePath Base path for all resources
         */
        void init(const std::string &basePath = "resources/");

        /**
         * @brief Set the base path for a specific resource type
         * @param resourceType Resource type (textures, fonts, sounds, music, shaders)
         * @param path Path to the resource directory
         */
        void setResourcePath(const std::string &resourceType, const std::string &path);

        /**
         * @brief Get the path for a specific resource type
         * @param resourceType Resource type (textures, fonts, sounds, music, shaders)
         * @return Path to the resource directory
         */
        std::string getResourcePath(const std::string &resourceType) const;

        /**
         * @brief Load a texture from file
         * @param id Resource identifier
         * @param filePath Path to the texture file (relative to textures path)
         * @param smooth Whether to enable smooth filtering
         * @param repeated Whether the texture should be repeated
         * @return Reference to the loaded texture
         * @throws ResourceLoadException if the texture cannot be loaded
         */
        sf::Texture &loadTexture(const std::string &id, const std::string &filePath,
                                 bool smooth = false, bool repeated = false);

        /**
         * @brief Get a texture by ID
         * @param id Resource identifier
         * @return Reference to the texture
         * @throws std::out_of_range if the texture does not exist
         */
        sf::Texture &getTexture(const std::string &id);

        /**
         * @brief Load a sprite sheet from a texture
         * @param id Resource identifier
         * @param textureId ID of the texture to use
         * @param frameWidth Width of each frame
         * @param frameHeight Height of each frame
         * @param frameCount Number of frames to extract
         * @return Reference to the created sprite sheet
         * @throws ResourceLoadException if the sprite sheet cannot be created
         */
        SpriteSheet &loadSpriteSheet(const std::string &id, const std::string &textureId,
                                     int frameWidth, int frameHeight, int frameCount);

        /**
         * @brief Load a sprite sheet from a texture with named frames
         * @param id Resource identifier
         * @param textureId ID of the texture to use
         * @param frames Vector of frame rectangles
         * @param namedFrames Map of frame names to indices
         * @return Reference to the created sprite sheet
         * @throws ResourceLoadException if the sprite sheet cannot be created
         */
        SpriteSheet &loadSpriteSheet(const std::string &id, const std::string &textureId,
                                     const std::vector<sf::IntRect> &frames,
                                     const std::unordered_map<std::string, int> &namedFrames = {});

        /**
         * @brief Get a sprite sheet by ID
         * @param id Resource identifier
         * @return Reference to the sprite sheet
         * @throws std::out_of_range if the sprite sheet does not exist
         */
        SpriteSheet &getSpriteSheet(const std::string &id);

        /**
         * @brief Load a font from file
         * @param id Resource identifier
         * @param filePath Path to the font file (relative to fonts path)
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
         * @param filePath Path to the sound file (relative to sounds path)
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
         * @brief Load a music from file
         * @param id Resource identifier
         * @param filePath Path to the music file (relative to music path)
         * @return Reference to the loaded music
         * @throws ResourceLoadException if the music cannot be loaded
         */
        sf::Music &loadMusic(const std::string &id, const std::string &filePath);

        /**
         * @brief Get a music by ID
         * @param id Resource identifier
         * @return Reference to the music
         * @throws std::out_of_range if the music does not exist
         */
        sf::Music &getMusic(const std::string &id);

        /**
         * @brief Load a shader from file
         * @param id Resource identifier
         * @param vertexShaderPath Path to the vertex shader file (relative to shaders path)
         * @param fragmentShaderPath Path to the fragment shader file (relative to shaders path)
         * @return Reference to the loaded shader
         * @throws ResourceLoadException if the shader cannot be loaded
         */
        sf::Shader &loadShader(const std::string &id, const std::string &vertexShaderPath,
                               const std::string &fragmentShaderPath);

        /**
         * @brief Load a shader from file (fragment only)
         * @param id Resource identifier
         * @param fragmentShaderPath Path to the fragment shader file (relative to shaders path)
         * @return Reference to the loaded shader
         * @throws ResourceLoadException if the shader cannot be loaded
         */
        sf::Shader &loadFragmentShader(const std::string &id, const std::string &fragmentShaderPath);

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
         * @brief Check if a sprite sheet exists
         * @param id Resource identifier
         * @return true if the sprite sheet exists
         */
        bool hasSpriteSheet(const std::string &id) const;

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
         * @brief Check if a music exists
         * @param id Resource identifier
         * @return true if the music exists
         */
        bool hasMusic(const std::string &id) const;

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
         * @brief Remove a sprite sheet
         * @param id Resource identifier
         * @return true if the sprite sheet was removed
         */
        bool removeSpriteSheet(const std::string &id);

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
         * @brief Remove a music
         * @param id Resource identifier
         * @return true if the music was removed
         */
        bool removeMusic(const std::string &id);

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

        /**
         * @brief Load all resources from a directory
         * @param directory Directory containing resource subdirectories
         * @param loadCallback Callback function for each loaded resource
         */
        void loadAllResources(const std::string &directory,
                              std::function<void(const std::string &, const std::string &)> loadCallback = nullptr);

    private:
        std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
        std::unordered_map<std::string, SpriteSheet> m_spriteSheets;
        std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
        std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
        std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;
        std::unordered_map<std::string, std::unique_ptr<sf::Shader>> m_shaders;

        std::unordered_map<std::string, std::string> m_resourcePaths;
        std::string m_basePath;

        // Helper methods
        std::string getFullPath(const std::string &resourceType, const std::string &filePath) const;
    };

} // namespace Resources