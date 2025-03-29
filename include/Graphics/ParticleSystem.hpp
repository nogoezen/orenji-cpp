#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>

namespace Graphics
{
    /**
     * @brief Structure representing a single particle
     */
    struct Particle
    {
        sf::Vector2f position; // Current position
        sf::Vector2f velocity; // Current velocity
        sf::Color color;       // Current color
        float lifetime;        // Remaining lifetime in seconds
        float initialLifetime; // Total lifetime
        float size;            // Current size
        float rotation;        // Current rotation in degrees
        float rotationSpeed;   // Rotation speed in degrees per second
    };

    /**
     * @brief Class for managing particle effects
     */
    class ParticleSystem : public sf::Drawable
    {
    public:
        /**
         * @brief Constructor
         * @param maxParticles Maximum number of particles in the system
         */
        ParticleSystem(unsigned int maxParticles = 1000);

        /**
         * @brief Destructor
         */
        virtual ~ParticleSystem();

        /**
         * @brief Set the texture
         * @param texture Texture to apply to particles
         */
        void setTexture(std::shared_ptr<sf::Texture> texture);

        /**
         * @brief Set the particle emission rate
         * @param particlesPerSecond Number of particles emitted per second
         */
        void setEmissionRate(float particlesPerSecond);

        /**
         * @brief Set the emitter position
         * @param position Position of the emitter
         */
        void setEmitterPosition(const sf::Vector2f &position);

        /**
         * @brief Set the particle lifetime
         * @param minLifetime Minimum lifetime in seconds
         * @param maxLifetime Maximum lifetime in seconds
         */
        void setParticleLifetime(float minLifetime, float maxLifetime);

        /**
         * @brief Set the particle initial velocity
         * @param minVelocity Minimum velocity vector
         * @param maxVelocity Maximum velocity vector
         */
        void setParticleVelocity(const sf::Vector2f &minVelocity, const sf::Vector2f &maxVelocity);

        /**
         * @brief Set the particle size
         * @param minSize Minimum size
         * @param maxSize Maximum size
         */
        void setParticleSize(float minSize, float maxSize);

        /**
         * @brief Set the particle colors
         * @param startColor Color at start of lifetime
         * @param endColor Color at end of lifetime
         */
        void setParticleColors(const sf::Color &startColor, const sf::Color &endColor);

        /**
         * @brief Set rotation speed range
         * @param minRotation Minimum rotation speed in degrees per second
         * @param maxRotation Maximum rotation speed in degrees per second
         */
        void setRotationSpeed(float minRotation, float maxRotation);

        /**
         * @brief Enable/disable the emitter
         * @param enabled Whether the emitter is active
         */
        void setEnabled(bool enabled);

        /**
         * @brief Check if emitter is enabled
         * @return True if enabled
         */
        bool isEnabled() const;

        /**
         * @brief Emit a burst of particles
         * @param count Number of particles to emit
         */
        void emitBurst(unsigned int count);

        /**
         * @brief Update all particles
         * @param deltaTime Time since last frame in seconds
         */
        void update(float deltaTime);

        /**
         * @brief Set a custom behavior function
         * @param function Function that modifies particle behavior
         */
        void setParticleBehavior(const std::function<void(Particle &, float)> &function);

        /**
         * @brief Clear all particles
         */
        void clear();

        // Predefined particle behaviors
        static void fireEffect(Particle &particle, float deltaTime);
        static void smokeEffect(Particle &particle, float deltaTime);
        static void sparkEffect(Particle &particle, float deltaTime);
        static void explosionEffect(Particle &particle, float deltaTime);
        static void rainEffect(Particle &particle, float deltaTime);
        static void snowEffect(Particle &particle, float deltaTime);

    private:
        /**
         * @brief Draw the particle system to a render target
         * @param target Render target to draw to
         * @param states Current render states
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Emit a new particle
         */
        void emitParticle();

        /**
         * @brief Get a random value between min and max
         */
        float randomFloat(float min, float max) const;

        /**
         * @brief Get a random vector between min and max
         */
        sf::Vector2f randomVector(const sf::Vector2f &min, const sf::Vector2f &max) const;

        /**
         * @brief Get a random color between two colors
         */
        sf::Color randomColor(const sf::Color &min, const sf::Color &max) const;

        std::vector<Particle> m_particles;
        sf::VertexArray m_vertices;
        std::shared_ptr<sf::Texture> m_texture;
        std::function<void(Particle &, float)> m_particleBehavior;

        unsigned int m_maxParticles;
        sf::Vector2f m_emitterPosition;
        float m_emissionRate;
        float m_emissionTimer;

        sf::Vector2f m_minVelocity;
        sf::Vector2f m_maxVelocity;
        float m_minLifetime;
        float m_maxLifetime;
        float m_minSize;
        float m_maxSize;
        float m_minRotation;
        float m_maxRotation;
        sf::Color m_startColor;
        sf::Color m_endColor;

        bool m_enabled;
    };
} // namespace Graphics