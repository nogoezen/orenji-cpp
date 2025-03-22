#include "Particles/ParticleSystem.hpp"
#include "Particles/ParticleComponent.hpp"
#include <iostream>
#include <random>
#include <fstream>

namespace Orenji
{
    // Initialize static member
    std::unique_ptr<ParticleSystem> ParticleSystem::s_instance;

    ParticleSystem &ParticleSystem::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::make_unique<ParticleSystem>();
        }
        return *s_instance;
    }

    ParticleSystem::ParticleSystem()
    {
        // Initialize default templates
        EmissionParameters fire;
        fire.emissionRate = 50.0f;
        fire.minLifetime = 0.5f;
        fire.maxLifetime = 2.0f;
        fire.minSize = 3.0f;
        fire.maxSize = 10.0f;
        fire.minSpeed = 10.0f;
        fire.maxSpeed = 30.0f;
        fire.startColor = sf::Color(255, 120, 0, 200);
        fire.endColor = sf::Color(255, 0, 0, 0);
        fire.positionRadius = 5.0f;
        fire.baseVelocity = sf::Vector2f(0.0f, -1.0f);
        fire.velocitySpread = 45.0f * 3.14159f / 180.0f; // Convertir en radians
        fire.minRotationSpeed = -90.0f;
        fire.maxRotationSpeed = 90.0f;
        m_templates["fire"] = fire;

        // Smoke effect
        EmissionParameters smoke;
        smoke.emissionRate = 10.0f;
        smoke.minLifetime = 2.0f;
        smoke.maxLifetime = 5.0f;
        smoke.minSize = 10.0f;
        smoke.maxSize = 20.0f;
        smoke.minSpeed = 5.0f;
        smoke.maxSpeed = 15.0f;
        smoke.startColor = sf::Color(100, 100, 100, 150);
        smoke.endColor = sf::Color(200, 200, 200, 0);
        smoke.positionRadius = 2.0f;
        smoke.baseVelocity = sf::Vector2f(0.0f, -1.0f);
        smoke.velocitySpread = 30.0f * 3.14159f / 180.0f; // Convertir en radians
        smoke.minRotationSpeed = -20.0f;
        smoke.maxRotationSpeed = 20.0f;
        m_templates["smoke"] = smoke;

        // Spark effect
        EmissionParameters spark;
        spark.emissionRate = 100.0f;
        spark.minLifetime = 0.2f;
        spark.maxLifetime = 0.7f;
        spark.minSize = 1.0f;
        spark.maxSize = 2.0f;
        spark.minSpeed = 50.0f;
        spark.maxSpeed = 150.0f;
        spark.startColor = sf::Color(255, 230, 100, 255);
        spark.endColor = sf::Color(255, 180, 0, 0);
        spark.positionRadius = 0.5f;
        spark.velocitySpread = 180.0f * 3.14159f / 180.0f; // Convertir en radians
        spark.minRotationSpeed = 0.0f;
        spark.maxRotationSpeed = 0.0f;
        m_templates["spark"] = spark;

        // Explosion effect
        EmissionParameters explosion;
        explosion.emissionRate = 0.0f; // One-shot
        explosion.minLifetime = 0.5f;
        explosion.maxLifetime = 1.0f;
        explosion.minSize = 2.0f;
        explosion.maxSize = 8.0f;
        explosion.minSpeed = 100.0f;
        explosion.maxSpeed = 200.0f;
        explosion.startColor = sf::Color(255, 200, 0, 255);
        explosion.endColor = sf::Color(255, 0, 0, 0);
        explosion.positionRadius = 1.0f;
        explosion.velocitySpread = 180.0f * 3.14159f / 180.0f; // Convertir en radians
        explosion.minRotationSpeed = -180.0f;
        explosion.maxRotationSpeed = 180.0f;
        m_templates["explosion"] = explosion;
    }

    ParticleSystem::~ParticleSystem()
    {
        // Clean up particle components
        m_particleComponents.clear();
    }

    bool ParticleSystem::setupParticleSystem(ParticleComponent &component, const std::string &templateName)
    {
        // Check if template exists
        auto it = m_templates.find(templateName);
        if (it == m_templates.end())
        {
            std::cerr << "Particle template not found: " << templateName << std::endl;
            return false;
        }

        // Apply template to component
        component.setEmissionParameters(it->second);

        // Determine trigger type based on emission rate
        if (it->second.emissionRate <= 0.0f)
        {
            // If emission rate is 0, use OneShot
            component.setTriggerType(ParticleTriggerType::OneShot);
            component.setBurstSize(50); // Default burst size for one-shot effects
        }
        else
        {
            // Otherwise use continuous emission
            component.setTriggerType(ParticleTriggerType::Continuous);
            component.setEmissionRate(it->second.emissionRate);
        }

        return true;
    }

    ParticleComponent *ParticleSystem::createParticleSystem(const std::string &name, const std::string &texturePath)
    {
        // Create a new component
        auto component = std::make_unique<ParticleComponent>(name);

        // Set texture if provided
        if (!texturePath.empty())
        {
            // Charger la texture depuis le gestionnaire de ressources
            sf::Texture *texture = m_textureCache.loadResource(texturePath);
            if (texture)
            {
                component->setTexture(texture);
            }
        }

        // Store and return the component
        ParticleComponent *result = component.get();
        m_particleComponents[name] = std::move(component);
        return result;
    }

    bool ParticleSystem::setTexture(const std::string &name, const std::string &texturePath)
    {
        auto it = m_particleComponents.find(name);
        if (it != m_particleComponents.end())
        {
            sf::Texture *texture = m_textureCache.loadResource(texturePath);
            if (texture)
            {
                it->second->setTexture(texture);
                return true;
            }
        }
        return false;
    }

    bool ParticleSystem::createSystem(const std::string &id, ParticleType type, const sf::Vector2f &position, const sf::Color &color)
    {
        // Create a new particle system based on type
        auto component = std::make_unique<ParticleComponent>(id);

        std::string templateName;

        // Map ParticleType to template name
        switch (type)
        {
        case ParticleType::EXPLOSION:
            templateName = "explosion";
            break;
        case ParticleType::FIRE:
            templateName = "fire";
            break;
        case ParticleType::SMOKE:
            templateName = "smoke";
            break;
        case ParticleType::SPARK:
            templateName = "spark";
            break;
        default:
            templateName = "fire"; // Default
            break;
        }

        // Configure the component
        if (!setupParticleSystem(*component, templateName))
        {
            return false;
        }

        // Store the component
        m_particleSystems[id] = std::make_unique<ParticleData>();
        m_particleSystems[id]->active = true;
        m_particleSystems[id]->position = position;

        // Modifier la couleur si nécessaire
        if (color != sf::Color::White)
        {
            EmissionParameters params = m_templates[templateName];
            params.startColor = color;
            params.endColor = sf::Color(color.r, color.g, color.b, 0);
            component->setEmissionParameters(params);
        }

        // Set position
        component->setPosition(position);

        // Add to component map as well
        m_particleComponents[id] = std::move(component);

        return true;
    }

    bool ParticleSystem::createCustomSystem(const std::string &id, const sf::Vector2f &position, const EmissionParameters &params)
    {
        // Create a new particle system with custom parameters
        auto component = std::make_unique<ParticleComponent>(id);

        // Apply custom parameters
        component->setEmissionParameters(params);
        component->setTriggerType(params.emissionRate > 0 ? ParticleTriggerType::Continuous : ParticleTriggerType::OneShot);
        component->setPosition(position);

        // Store the system
        m_particleSystems[id] = std::make_unique<ParticleData>();
        m_particleSystems[id]->active = true;
        m_particleSystems[id]->position = position;
        m_particleSystems[id]->parameters = params;

        // Add to component map
        m_particleComponents[id] = std::move(component);

        return true;
    }

    void ParticleSystem::update(float deltaTime)
    {
        // Update all particle components
        for (auto &pair : m_particleComponents)
        {
            if (pair.second && pair.second->isEnabled())
            {
                pair.second->update(deltaTime);
            }
        }
    }

    void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Draw all particle components
        for (const auto &pair : m_particleComponents)
        {
            if (pair.second && pair.second->isEnabled())
            {
                pair.second->draw(target);
            }
        }
    }

    ParticleComponent *ParticleSystem::getParticleComponent(const std::string &name)
    {
        auto it = m_particleComponents.find(name);
        if (it != m_particleComponents.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void ParticleSystem::removeParticleSystem(const std::string &name)
    {
        m_particleComponents.erase(name);
    }

    bool ParticleSystem::hasParticleSystem(const std::string &name) const
    {
        return m_particleComponents.find(name) != m_particleComponents.end();
    }

    bool ParticleSystem::emitParticles(const std::string &id, unsigned int count)
    {
        auto component = getParticleComponent(id);
        if (component)
        {
            component->emit(count);
            return true;
        }
        return false;
    }

    bool ParticleSystem::setPosition(const std::string &id, const sf::Vector2f &position)
    {
        auto component = getParticleComponent(id);
        if (component)
        {
            component->setPosition(position);
            return true;
        }
        return false;
    }

    bool ParticleSystem::setActive(const std::string &id, bool active)
    {
        auto component = getParticleComponent(id);
        if (component)
        {
            component->setEnabled(active);
            return true;
        }
        return false;
    }

    bool ParticleSystem::removeSystem(const std::string &id)
    {
        m_particleSystems.erase(id);
        m_particleComponents.erase(id);
        return true;
    }

    bool ParticleSystem::addAffector(const std::string &id, AffectorType type, float strength)
    {
        auto component = getParticleComponent(id);
        if (component)
        {
#ifndef DISABLE_THOR
            // Thor version doesn't support this directly through the component
            return false;
#else
            component->addAffector(type, strength);
            return true;
#endif
        }
        return false;
    }

    bool ParticleSystem::setTexture(const std::string &name, const sf::Texture &texture, const sf::IntRect &textureRect)
    {
        auto component = getParticleComponent(name);
        if (component)
        {
            component->setTexture(&texture, textureRect);
            return true;
        }
        return false;
    }
} // namespace Orenji