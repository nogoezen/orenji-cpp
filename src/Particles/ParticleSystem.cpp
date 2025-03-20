#include "../include/Particles/ParticleSystem.hpp"
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Animations/FadeAnimation.hpp>
#include <random>

namespace Orenji
{
    // Initialize the singleton instance
    std::unique_ptr<ParticleSystem> ParticleSystem::s_instance = nullptr;

    ParticleSystem::ParticleSystem()
    {
        // Private constructor for singleton
    }

    ParticleSystem::~ParticleSystem()
    {
        // Cleanup
        m_particleSystems.clear();
    }

    ParticleSystem &ParticleSystem::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<ParticleSystem>(new ParticleSystem());
        }
        return *s_instance;
    }

    bool ParticleSystem::createSystem(const std::string &id,
                                      ParticleType type,
                                      const sf::Vector2f &position,
                                      const sf::Color &color)
    {
        // Check if system with this ID already exists
        if (m_particleSystems.find(id) != m_particleSystems.end())
        {
            return false;
        }

        // Create new particle system data
        auto data = std::make_unique<ParticleData>();
        data->active = true;
        data->position = position;

        // Configure the particle system based on type
        switch (type)
        {
        case ParticleType::EXPLOSION:
        {
            // Explosion effect - particles spread out quickly and fade
            data->emitter.setEmissionRate(200.f);
            data->emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.5f), sf::seconds(1.5f)));

            // Initial particle velocity - outward from center
            data->emitter.setParticleVelocity(thor::Distributions::deflect(
                thor::PolarVector2f(150.f, 0.f), 360.f));

            // Color
            data->emitter.setParticleColor(color);

            // Add affectors to fade out particles based on remaining lifetime
            data->system.addAffector([color](thor::Particle &particle, sf::Time)
                                     {
                // Use getElapsedRatio to safely access the particle's age
                float ratio = thor::getElapsedRatio(particle);
                particle.color.a = static_cast<sf::Uint8>((1.f - ratio) * 255); });

            // Add affector to slow down particles over time
            data->system.addAffector([](thor::Particle &particle, sf::Time dt)
                                     { particle.velocity *= 0.97f; });
        }
        break;

        case ParticleType::FIRE:
        {
            // Fire effect - continuous upward flow with color change
            data->emitter.setEmissionRate(30.f);
            data->emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.5f), sf::seconds(1.0f)));

            // Initial velocity - mostly upward with variation
            data->emitter.setParticleVelocity(thor::Distributions::deflect(
                thor::PolarVector2f(50.f, 270.f), 45.f));

            // Start with the fire color
            sf::Color fireColor = color.r > 200 ? color : sf::Color(255, 100, 20);
            data->emitter.setParticleColor(fireColor);

            // Add color transition from fire color to smoke as particles age
            data->system.addAffector([fireColor](thor::Particle &particle, sf::Time)
                                     {
                float ratio = thor::getElapsedRatio(particle);
                
                // Fade to dark smoke color
                sf::Color smokeColor(50, 50, 50);
                
                // Blend the colors manually (Thor 2.0 doesn't have interpolate)
                particle.color.r = static_cast<sf::Uint8>(fireColor.r * (1.f - ratio) + smokeColor.r * ratio);
                particle.color.g = static_cast<sf::Uint8>(fireColor.g * (1.f - ratio) + smokeColor.g * ratio);
                particle.color.b = static_cast<sf::Uint8>(fireColor.b * (1.f - ratio) + smokeColor.b * ratio);
                
                // Fade out alpha at end of life
                if (ratio > 0.7f) {
                    particle.color.a = static_cast<sf::Uint8>((1.f - ratio) / 0.3f * 255);
                } });

            // Add size change - gets smaller as the particle ages
            data->system.addAffector([](thor::Particle &particle, sf::Time)
                                     {
                float ratio = thor::getElapsedRatio(particle);
                particle.scale = sf::Vector2f(1.f - 0.5f * ratio, 1.f - 0.5f * ratio); });
        }
        break;

        case ParticleType::SMOKE:
        {
            // Smoke effect - slow upward movement with alpha fade
            data->emitter.setEmissionRate(15.f);
            data->emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(1.5f), sf::seconds(3.0f)));

            // Slow upward movement with variation
            data->emitter.setParticleVelocity(thor::Distributions::deflect(
                thor::PolarVector2f(20.f, 270.f), 20.f));

            // Dark smoke color
            sf::Color smokeColor = color.r < 100 ? color : sf::Color(80, 80, 80);
            data->emitter.setParticleColor(smokeColor);

            // Fade out with age
            data->system.addAffector([](thor::Particle &particle, sf::Time)
                                     {
                float ratio = thor::getElapsedRatio(particle);
                particle.color.a = static_cast<sf::Uint8>((1.f - ratio) * 200); });

            // Gradually increase size
            data->system.addAffector([](thor::Particle &particle, sf::Time)
                                     {
                float ratio = thor::getElapsedRatio(particle);
                particle.scale = sf::Vector2f(0.5f + ratio * 0.5f, 0.5f + ratio * 0.5f); });
        }
        break;

        case ParticleType::SPARKLE:
        {
            // Sparkle effect - quick, bright particles that fade rapidly
            data->emitter.setEmissionRate(100.f);
            data->emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1f), sf::seconds(0.5f)));

            // Random direction, medium speed
            data->emitter.setParticleVelocity(thor::Distributions::deflect(
                thor::PolarVector2f(50.f, 0.f), 360.f));

            // Bright color
            sf::Color sparkleColor = color.r > 200 && color.g > 200 ? color : sf::Color(255, 255, 200);
            data->emitter.setParticleColor(sparkleColor);

            // Quick fade out
            data->system.addAffector([](thor::Particle &particle, sf::Time)
                                     {
                float ratio = thor::getElapsedRatio(particle);
                particle.color.a = static_cast<sf::Uint8>((1.f - ratio * ratio) * 255); });

            // Random rotation
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> rotDist(-10.0f, 10.0f);

            data->system.addAffector([rotDist, gen](thor::Particle &particle, sf::Time dt) mutable
                                     { particle.rotation += rotDist(gen) * dt.asSeconds(); });
        }
        break;

        case ParticleType::CUSTOM:
        default:
        {
            // Default custom system, can be configured later
            data->emitter.setEmissionRate(30.f);
            data->emitter.setParticleLifetime(sf::seconds(1.f));
            data->emitter.setParticleColor(color);
        }
        break;
        }

        // Set the emitter position
        data->emitter.setParticlePosition(position);

        // Store the particle system
        m_particleSystems[id] = std::move(data);
        return true;
    }

    bool ParticleSystem::createCustomSystem(const std::string &id,
                                            const sf::Vector2f &position,
                                            float emissionRate,
                                            sf::Time particleLifetime)
    {
        // Check if system already exists
        if (m_particleSystems.find(id) != m_particleSystems.end())
        {
            return false;
        }

        // Create a basic system with custom parameters
        auto data = std::make_unique<ParticleData>();
        data->active = true;
        data->position = position;

        data->emitter.setEmissionRate(emissionRate);
        data->emitter.setParticleLifetime(particleLifetime);
        data->emitter.setParticlePosition(position);

        m_particleSystems[id] = std::move(data);
        return true;
    }

    bool ParticleSystem::addAffector(const std::string &id, std::function<void(thor::Particle &, sf::Time)> affector)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        it->second->system.addAffector(affector);
        return true;
    }

    bool ParticleSystem::setEmitter(const std::string &id, std::function<void(thor::EmissionInterface &, sf::Time)> emitter)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        // In Thor 2.0, we need to add the emitter each update, not set it once
        it->second->system.addEmitter(emitter);
        return true;
    }

    bool ParticleSystem::setTexture(const std::string &id, const sf::Texture &texture, const sf::IntRect &textureRect)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        // In Thor 2.0, setTexture only takes one argument
        it->second->system.setTexture(texture);

        return true;
    }

    bool ParticleSystem::setActive(const std::string &id, bool active)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        it->second->active = active;
        return true;
    }

    bool ParticleSystem::setPosition(const std::string &id, const sf::Vector2f &position)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        it->second->position = position;
        it->second->emitter.setParticlePosition(position);
        return true;
    }

    bool ParticleSystem::emit(const std::string &id, int count)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        // Get the emitter
        thor::UniversalEmitter &emitter = it->second->emitter;
        sf::Vector2f position = it->second->position;

        // Use a temporary connection to emit particles
        it->second->system.addEmitter([count, position](thor::EmissionInterface &interface, sf::Time)
                                      {
            for (int i = 0; i < count; ++i) {
                // Create a particle with reasonable defaults
                thor::Particle particle(sf::seconds(1.f));
                
                // Set properties
                particle.position = position;
                particle.velocity = thor::PolarVector2f(50.f, thor::random(0.f, 360.f));
                particle.rotation = 0.0f;
                particle.rotationSpeed = 0.0f;
                particle.scale = sf::Vector2f(1.f, 1.f);
                particle.color = sf::Color::White;
                
                // Emit the particle
                interface.emitParticle(particle);
            } }, sf::seconds(0.f));

        return true;
    }

    bool ParticleSystem::stop(const std::string &id)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        it->second->emitter.setEmissionRate(0.f);
        it->second->active = false;
        return true;
    }

    bool ParticleSystem::removeSystem(const std::string &id)
    {
        auto it = m_particleSystems.find(id);
        if (it == m_particleSystems.end())
        {
            return false;
        }

        m_particleSystems.erase(it);
        return true;
    }

    void ParticleSystem::update(float deltaTime)
    {
        sf::Time dt = sf::seconds(deltaTime);

        for (auto &pair : m_particleSystems)
        {
            auto &data = pair.second;

            if (data->active)
            {
                // Update the emitter and system
                data->system.addEmitter(data->emitter, dt);
                data->system.update(dt);
            }
        }
    }

    void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        for (const auto &pair : m_particleSystems)
        {
            if (pair.second->active)
            {
                target.draw(pair.second->system, states);
            }
        }
    }
} // namespace Orenji