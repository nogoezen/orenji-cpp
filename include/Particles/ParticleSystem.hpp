#pragma once

#include "../Core/ResourceCache.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <map>

namespace Orenji
{
    class ParticleComponent;

    /**
     * @brief Structure représentant une particule simple
     */
    struct SimpleParticle
    {
        sf::Vector2f position; // Position
        sf::Vector2f velocity; // Vitesse
        sf::Color color;       // Couleur
        float lifetime;        // Durée de vie maximale
        float elapsed;         // Temps écoulé
        float size;            // Taille
        float rotation;        // Rotation
        float rotationSpeed;   // Vitesse de rotation
    };

    /**
     * @brief Structure représentant les paramètres d'émission de particules
     */
    struct EmissionParameters
    {
        float emissionRate = 30.0f;
        float minLifetime = 1.0f;
        float maxLifetime = 3.0f;
        float minSize = 2.0f;
        float maxSize = 5.0f;
        float minSpeed = 20.0f;
        float maxSpeed = 50.0f;
        float minRotationSpeed = -180.0f;
        float maxRotationSpeed = 180.0f;
        sf::Color startColor = sf::Color::White;
        sf::Color endColor = sf::Color(255, 255, 255, 0);
        sf::Vector2f positionOffset = sf::Vector2f(0.0f, 0.0f);
        float positionRadius = 2.0f;
        sf::Vector2f baseVelocity = sf::Vector2f(0.0f, -30.0f);
        float velocitySpread = 10.0f;
    };

    /**
     * @brief Type d'affecteur pour les particules
     */
    enum class AffectorType
    {
        GRAVITY,  ///< Applique une gravité
        FADE,     ///< Fait disparaître progressivement
        SCALE,    ///< Change la taille
        COLOR,    ///< Change la couleur
        ROTATION, ///< Fait tourner la particule
        DRAG,     ///< Applique une résistance de l'air
        VORTEX    ///< Crée un effet de tourbillon
    };

    /**
     * @brief Particle system management class
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
         * @brief Create a simple particle system
         * @param id System identifier
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
         * @param id System identifier
         * @param position Initial position of the system
         * @param params Emission parameters
         * @return true if the system was created successfully
         */
        bool createCustomSystem(const std::string &id,
                                const sf::Vector2f &position,
                                const EmissionParameters &params = EmissionParameters());

        /**
         * @brief Add an affector to a particle system
         * @param id System identifier
         * @param type Affector type
         * @param strength Strength of the effect
         * @return true if the affector was added successfully
         */
        bool addAffector(const std::string &id, AffectorType type, float strength = 1.0f);

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
         * @brief Configure a particle component with predefined settings
         * @param component The component to configure
         * @param templateName The name of the template
         * @return true if successful
         */
        bool setupParticleSystem(ParticleComponent &component, const std::string &templateName);

        /**
         * @brief Create a particle component
         * @param name Component name
         * @param texturePath Optional texture path
         * @return Pointer to created component
         */
        ParticleComponent *createParticleSystem(const std::string &name, const std::string &texturePath = "");

        /**
         * @brief Set texture for a component
         * @param name Component name
         * @param texturePath Texture path
         * @return true if success
         */
        bool setTexture(const std::string &name, const std::string &texturePath);

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
         * @brief Remove a particle component
         * @param name Component name
         */
        void removeParticleSystem(const std::string &name);

        /**
         * @brief Get a particle component
         * @param name Component name
         * @return Pointer to component or nullptr
         */
        ParticleComponent *getParticleComponent(const std::string &name);

        /**
         * @brief Check if a particle component exists
         * @param name Component name
         * @return true if exists
         */
        bool hasParticleSystem(const std::string &name) const;

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

        // Singleton pattern
        ParticleSystem(const ParticleSystem &) = delete;
        ParticleSystem &operator=(const ParticleSystem &) = delete;

    private:
        /**
         * @brief Constructor (private for singleton)
         */
        ParticleSystem();

        static std::unique_ptr<ParticleSystem> s_instance;

        /**
         * @brief Structure containing data for a particle system
         */
        struct ParticleData
        {
            std::vector<SimpleParticle> particles;
            EmissionParameters parameters;
            sf::VertexArray vertices;
            sf::Texture texture;
            sf::IntRect textureRect;
            bool active;
            sf::Vector2f position;
            float emissionAccumulator;
            std::vector<std::pair<AffectorType, float>> affectors;
        };

        // Map of particle systems by ID
        std::unordered_map<std::string, std::unique_ptr<ParticleData>> m_particleSystems;

        // Map of particle components by name
        std::map<std::string, std::unique_ptr<ParticleComponent>> m_particleComponents;

        // Map of templates by name
        std::map<std::string, EmissionParameters> m_templates;

        /**
         * @brief Generate a new particle based on the given parameters
         * @param params Emission parameters
         * @param position Base position
         * @return A new particle
         */
        SimpleParticle generateParticle(const EmissionParameters &params, const sf::Vector2f &position);

        /**
         * @brief Apply affectors to a particle
         * @param particle The particle to affect
         * @param affectors List of affectors and their strengths
         * @param deltaTime Time elapsed
         */
        void applyAffectors(SimpleParticle &particle,
                            const std::vector<std::pair<AffectorType, float>> &affectors,
                            float deltaTime);
    };

} // namespace Orenji