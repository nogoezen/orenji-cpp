#pragma once

#include "../Core/ResourceCache.hpp"
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

namespace Orenji
{
    /**
     * @brief Particle system management class based on Thor
     *
     * This class allows creating and managing particle systems
     * with different behaviors and appearances.
     */
    class ParticleSystem : public sf::Drawable
    {
    public:
        /**
         * @brief Predefined particle system type
         */
        enum class ParticleType
        {
            EXPLOSION, ///< Explosion effect
            FIRE,      ///< Fire effect
            SMOKE,     ///< Smoke effect
            SPARKLE,   ///< Sparkle effect
            CUSTOM     ///< Custom system
        };

        /**
         * @brief Get the unique instance of the particle system (singleton)
         * @return Reference to the instance
         */
        static ParticleSystem &getInstance();

        /**
         * @brief Destructor
         */
        ~ParticleSystem();

        /**
         * @brief Create a predefined particle system
         * @param id Unique identifier for the system
         * @param type Type of system to create
         * @param position Initial position of the system
         * @param color Main color of the particles
         * @return true if the system was created successfully
         */
        bool createSystem(const std::string &id,
                          ParticleType type,
                          const sf::Vector2f &position,
                          const sf::Color &color = sf::Color::White);

        /**
         * @brief Create a custom particle system
         * @param id Unique identifier for the system
         * @param position Initial position of the system
         * @param emissionRate Particle emission rate per second
         * @param particleLifetime Lifetime of particles in seconds
         * @return true if the system was created successfully
         */
        bool createCustomSystem(const std::string &id,
                                const sf::Vector2f &position,
                                float emissionRate = 30.f,
                                sf::Time particleLifetime = sf::seconds(1.f));

        /**
         * @brief Add an affector (modifier) to the particle system
         * @param id System identifier
         * @param affector Function that affects particles
         * @return true if the affector was added successfully
         */
        bool addAffector(const std::string &id, std::function<void(thor::Particle &, sf::Time)> affector);

        /**
         * @brief Configure the particle emitter
         * @param id System identifier
         * @param emitter Function that emits particles
         * @return true if the emitter was configured successfully
         */
        bool setEmitter(const std::string &id, std::function<void(thor::EmissionInterface &, sf::Time)> emitter);

        /**
         * @brief Set the texture for particles
         * @param id System identifier
         * @param texture Texture to use for particles
         * @param textureRect Area of the texture to use (optional)
         * @return true if the texture was set successfully
         */
        bool setTexture(const std::string &id,
                        const sf::Texture &texture,
                        const sf::IntRect &textureRect = sf::IntRect());

        /**
         * @brief Activate or deactivate a particle system
         * @param id System identifier
         * @param active Activation state
         * @return true if the state was modified successfully
         */
        bool setActive(const std::string &id, bool active);

        /**
         * @brief Set the position of a particle system
         * @param id System identifier
         * @param position New position
         * @return true if the position was modified successfully
         */
        bool setPosition(const std::string &id, const sf::Vector2f &position);

        /**
         * @brief Emit a specific number of particles
         * @param id System identifier
         * @param count Number of particles to emit
         * @return true if the particles were emitted successfully
         */
        bool emit(const std::string &id, int count);

        /**
         * @brief Stop a particle system (without removing it)
         * @param id System identifier
         * @return true if the system was stopped successfully
         */
        bool stop(const std::string &id);

        /**
         * @brief Remove a particle system
         * @param id System identifier
         * @return true if the system was removed successfully
         */
        bool removeSystem(const std::string &id);

        /**
         * @brief Update all particle systems
         * @param deltaTime Time elapsed since the last update in seconds
         */
        void update(float deltaTime);

        /**
         * @brief Draw all particle systems
         * @param target Render target
         * @param states Render states
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        /**
         * @brief Private constructor (singleton)
         */
        ParticleSystem();

        // Unique singleton instance
        static std::unique_ptr<ParticleSystem> s_instance;

        // Structure for particle system data
        struct ParticleData
        {
            thor::ParticleSystem system;
            thor::UniversalEmitter emitter;
            bool active;
            sf::Vector2f position;
        };

        std::unordered_map<std::string, std::unique_ptr<ParticleData>> m_particleSystems;
    };

} // namespace Orenji