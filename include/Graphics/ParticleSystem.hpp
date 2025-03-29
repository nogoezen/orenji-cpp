#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>

namespace Orenji
{
    namespace Graphics
    {

        /**
         * @brief Structure representing a single particle
         */
        struct Particle
        {
            sf::Vector2f position;     // Current position
            sf::Vector2f prevPosition; // Previous position for physics calculations
            sf::Vector2f velocity;     // Current velocity
            sf::Vector2f acceleration; // Current acceleration
            sf::Color color;           // Current color
            sf::Color startColor;      // Initial color
            sf::Color endColor;        // Final color before dying
            float lifetime;            // Remaining lifetime in seconds
            float initialLifetime;     // Total lifetime
            float size;                // Current size
            float initialSize;         // Initial size
            float endSize;             // Final size before dying
            float rotation;            // Current rotation in degrees
            float rotationSpeed;       // Rotation speed in degrees per second
            float drag;                // Air resistance factor (0-1)
            float alpha;               // Transparency (0-1)
            bool active = false;       // Whether the particle is currently active
        };

        /**
         * @brief Enum for predefined particle effects
         */
        enum class ParticleEffect
        {
            NONE,
            FIRE,
            SMOKE,
            EXPLOSION,
            SPARK,
            RAIN,
            SNOW,
            DUST,
            WATERFALL,
            MAGIC,
            LEAF,
            BUBBLE
        };

        /**
         * @brief Class for managing particle effects with high performance
         */
        class ParticleSystem : public sf::Drawable, public sf::Transformable
        {
        public:
            using ParticleBehavior = std::function<void(Particle &, float)>;

            /**
             * @brief Constructor
             * @param maxParticles Maximum number of particles in the system
             */
            ParticleSystem(unsigned int maxParticles = 5000);

            /**
             * @brief Destructor
             */
            virtual ~ParticleSystem();

            /**
             * @brief Set the texture
             * @param texturePath Path to the texture to apply to particles
             */
            void setTexture(const std::string &texturePath);

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
             * @brief Set the emitter radius (circular)
             * @param radius Radius of the circular emission area
             */
            void setEmitterRadius(float radius);

            /**
             * @brief Set the emitter area (rectangular)
             * @param topLeft Top left corner of the emission area
             * @param size Size of the emission area
             */
            void setRectangularEmitter(const sf::Vector2f &topLeft, const sf::Vector2f &size);

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
             * @brief Set the particle acceleration
             * @param acceleration Acceleration vector to apply to particles
             */
            void setAcceleration(const sf::Vector2f &acceleration);

            /**
             * @brief Set the particle size
             * @param minSize Minimum size
             * @param maxSize Maximum size
             */
            void setParticleSize(float minSize, float maxSize);

            /**
             * @brief Set the particle final size (for size fading)
             * @param minEndSize Minimum end size
             * @param maxEndSize Maximum end size
             */
            void setParticleEndSize(float minEndSize, float maxEndSize);

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
            void setParticleRotationSpeed(float minRotation, float maxRotation);

            /**
             * @brief Set the air resistance (drag) factor
             * @param drag Drag factor between 0 (no drag) and 1 (maximum drag)
             */
            void setDrag(float drag);

            /**
             * @brief Apply a force (like gravity) to all particles
             * @param force Force vector to apply
             */
            void applyForce(const sf::Vector2f &force);

            /**
             * @brief Apply a radial force from a point (attraction or repulsion)
             * @param center Center point of the force
             * @param strength Strength of the force (negative for attraction, positive for repulsion)
             * @param radius Radius of effect (0 for infinite)
             */
            void applyRadialForce(const sf::Vector2f &center, float strength, float radius = 0.0f);

            /**
             * @brief Enable/disable the emitter
             * @param enabled Whether the emitter is active
             */
            void enableEmitter(bool enable);

            /**
             * @brief Check if emitter is enabled
             * @return True if enabled
             */
            bool isEmitterEnabled() const;

            /**
             * @brief Get the current active particle count
             * @return Number of active particles
             */
            size_t getParticleCount() const;

            /**
             * @brief Get the maximum particle count
             * @return Maximum number of particles
             */
            size_t getMaxParticleCount() const;

            /**
             * @brief Emit a burst of particles
             * @param count Number of particles to emit
             */
            void emit(unsigned int count);

            /**
             * @brief Update all particles
             * @param deltaTime Time since last frame in seconds
             */
            void update(float deltaTime);

            /**
             * @brief Set a custom behavior function
             * @param behavior Function that modifies particle behavior
             */
            void setParticleBehavior(ParticleBehavior behavior);

            /**
             * @brief Set a predefined particle effect
             * @param effect Type of effect from ParticleEffect enum
             */
            void setEffect(ParticleEffect effect);

            /**
             * @brief Load a particle effect configuration from a file
             * @param filepath Path to the file containing effect parameters
             * @return True if loading succeeded
             */
            bool loadFromFile(const std::string &filepath);

            /**
             * @brief Save the current particle effect configuration to a file
             * @param filename Name of the file to save to
             * @return True if saving succeeded
             */
            bool saveToFile(const std::string &filename);

            /**
             * @brief Clear all particles
             */
            void clear();

            /**
             * @brief Set particle blend mode
             * @param mode SFML blend mode to use
             */
            void setBlendMode(sf::BlendMode mode);

            /**
             * @brief Set whether to use circular emitter
             * @param circular True to use circular emitter, false for point/rectangular
             */
            void setCircularEmitter(bool circular);

            // Predefined particle behaviors
            static void fireEffect(Particle &particle, float deltaTime);
            static void smokeEffect(Particle &particle, float deltaTime);
            static void explosionEffect(Particle &particle, float deltaTime);
            static void sparkEffect(Particle &particle, float deltaTime);
            static void rainEffect(Particle &particle, float deltaTime);
            static void snowEffect(Particle &particle, float deltaTime);
            static void dustEffect(Particle &particle, float deltaTime);
            static void waterfallEffect(Particle &particle, float deltaTime);
            static void magicEffect(Particle &particle, float deltaTime);
            static void leafEffect(Particle &particle, float deltaTime);
            static void bubbleEffect(Particle &particle, float deltaTime);

        private:
            /**
             * @brief Draw the particle system to a render target
             * @param target Render target to draw to
             * @param states Current render states
             */
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

            /**
             * @brief Emit a new particle
             * @return Reference to the emitted particle
             */
            Particle &emitParticle();

            /**
             * @brief Find an inactive particle to reuse
             * @return Index of an inactive particle, or -1 if none available
             */
            int findInactiveParticle() const;

            /**
             * @brief Update the vertex array with current particle data
             */
            void updateVertices();

            /**
             * @brief Apply blending mode based on effect type
             * @param states Render states to modify
             */
            void applyBlendMode(sf::RenderStates &states) const;

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

            /**
             * @brief Get a random position within the emitter area
             */
            sf::Vector2f getRandomEmissionPosition() const;

            // Particle storage
            std::vector<Particle> m_particles;
            sf::VertexArray m_vertices;
            std::shared_ptr<sf::Texture> m_texture;
            ParticleBehavior m_particleBehavior;

            // Random number generation
            mutable std::mt19937 m_randomEngine;
            mutable std::uniform_real_distribution<float> m_uniformDist;

            // Emitter properties
            unsigned int m_maxParticles;
            sf::Vector2f m_emitterPosition;
            sf::Vector2f m_emitterAreaTopLeft;
            sf::Vector2f m_emitterAreaSize;
            float m_emitterRadius;
            bool m_useCircularEmitter;
            float m_emissionRate;
            float m_emissionAccumulator;

            // Forces
            sf::Vector2f m_globalForce;
            sf::Vector2f m_acceleration;
            float m_drag;

            // Particle properties
            sf::Vector2f m_minVelocity;
            sf::Vector2f m_maxVelocity;
            float m_minLifetime;
            float m_maxLifetime;
            float m_minSize;
            float m_maxSize;
            float m_minEndSize;
            float m_maxEndSize;
            float m_minRotation;
            float m_maxRotation;
            sf::Color m_startColor;
            sf::Color m_endColor;

            // Blending mode
            sf::BlendMode m_blendMode;

            // System state
            bool m_emitterEnabled;
            size_t m_activeParticleCount;
        };

    } // namespace Graphics
} // namespace Orenji