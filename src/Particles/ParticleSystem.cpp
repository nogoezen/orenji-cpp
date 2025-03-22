#include "../../include/Particles/ParticleSystem.hpp"
#include "../../include/Particles/ParticleComponent.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <json/json.h>

namespace Orenji
{
    // Singleton instance
    ParticleSystem *ParticleSystem::s_instance = nullptr;

    ParticleSystem &ParticleSystem::getInstance()
    {
        if (!s_instance)
        {
            s_instance = new ParticleSystem();
        }
        return *s_instance;
    }

    void ParticleSystem::release()
    {
        if (s_instance)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }

    ParticleSystem::ParticleSystem()
    {
        // Initialize predefined systems
        initializePredefinedSystems();
    }

    ParticleSystem::~ParticleSystem()
    {
        // Clean up particle components
        m_particleComponents.clear();
    }

    void ParticleSystem::initializePredefinedSystems()
    {
        // Fire effect
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
        fire.velocitySpread = 45.0f;
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
        smoke.velocitySpread = 30.0f;
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
        spark.velocitySpread = 180.0f;
        spark.minRotationSpeed = 0.0f;
        spark.maxRotationSpeed = 0.0f;
        m_templates["spark"] = spark;

        // Water effect
        EmissionParameters water;
        water.emissionRate = 30.0f;
        water.minLifetime = 1.0f;
        water.maxLifetime = 3.0f;
        water.minSize = 2.0f;
        water.maxSize = 5.0f;
        water.minSpeed = 20.0f;
        water.maxSpeed = 40.0f;
        water.startColor = sf::Color(0, 100, 255, 150);
        water.endColor = sf::Color(0, 200, 255, 0);
        water.positionRadius = 1.0f;
        water.baseVelocity = sf::Vector2f(0.0f, 1.0f);
        water.velocitySpread = 30.0f;
        water.minRotationSpeed = 0.0f;
        water.maxRotationSpeed = 0.0f;
        m_templates["water"] = water;

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
        explosion.velocitySpread = 180.0f;
        explosion.minRotationSpeed = -180.0f;
        explosion.maxRotationSpeed = 180.0f;
        m_templates["explosion"] = explosion;
    }

    bool ParticleSystem::loadTemplatesFromFile(const std::string &filename)
    {
        try
        {
            // Open the file
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Failed to open particle templates file: " << filename << std::endl;
                return false;
            }

            // Parse JSON
            Json::Value root;
            file >> root;
            file.close();

            // Process each template
            for (const auto &templateName : root.getMemberNames())
            {
                const Json::Value &templateData = root[templateName];
                EmissionParameters params;

                // Load basic parameters
                params.emissionRate = templateData.get("emissionRate", 30.0f).asFloat();
                params.minLifetime = templateData.get("minLifetime", 1.0f).asFloat();
                params.maxLifetime = templateData.get("maxLifetime", 3.0f).asFloat();
                params.minSize = templateData.get("minSize", 2.0f).asFloat();
                params.maxSize = templateData.get("maxSize", 5.0f).asFloat();
                params.minSpeed = templateData.get("minSpeed", 20.0f).asFloat();
                params.maxSpeed = templateData.get("maxSpeed", 50.0f).asFloat();
                params.positionRadius = templateData.get("positionRadius", 2.0f).asFloat();
                params.velocitySpread = templateData.get("velocitySpread", 45.0f).asFloat();
                params.minRotationSpeed = templateData.get("minRotationSpeed", -180.0f).asFloat();
                params.maxRotationSpeed = templateData.get("maxRotationSpeed", 180.0f).asFloat();

                // Load colors
                if (templateData.isMember("startColor"))
                {
                    Json::Value startColor = templateData["startColor"];
                    params.startColor = sf::Color(
                        startColor.get("r", 255).asUInt(),
                        startColor.get("g", 255).asUInt(),
                        startColor.get("b", 255).asUInt(),
                        startColor.get("a", 255).asUInt());
                }

                if (templateData.isMember("endColor"))
                {
                    Json::Value endColor = templateData["endColor"];
                    params.endColor = sf::Color(
                        endColor.get("r", 255).asUInt(),
                        endColor.get("g", 255).asUInt(),
                        endColor.get("b", 255).asUInt(),
                        endColor.get("a", 0).asUInt());
                }

                // Load base velocity
                if (templateData.isMember("baseVelocity"))
                {
                    Json::Value baseVelocity = templateData["baseVelocity"];
                    params.baseVelocity = sf::Vector2f(
                        baseVelocity.get("x", 0.0f).asFloat(),
                        baseVelocity.get("y", 0.0f).asFloat());
                }

                // Load position offset
                if (templateData.isMember("positionOffset"))
                {
                    Json::Value posOffset = templateData["positionOffset"];
                    params.positionOffset = sf::Vector2f(
                        posOffset.get("x", 0.0f).asFloat(),
                        posOffset.get("y", 0.0f).asFloat());
                }

                // Add to templates
                m_templates[templateName] = params;
                std::cout << "Loaded particle template: " << templateName << std::endl;
            }

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error loading particle templates: " << e.what() << std::endl;
            return false;
        }
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

    ParticleComponent *ParticleSystem::createSystem(const std::string &id, ParticleType type, const sf::Vector2f &position, const sf::Color &color)
    {
        // Create a new component
        auto component = std::make_shared<ParticleComponent>();

        std::string templateName;

        // Map ParticleType to template name
        switch (type)
        {
        case ParticleType::Fire:
            templateName = "fire";
            break;
        case ParticleType::Smoke:
            templateName = "smoke";
            break;
        case ParticleType::Spark:
            templateName = "spark";
            break;
        case ParticleType::Water:
            templateName = "water";
            break;
        case ParticleType::Explosion:
            templateName = "explosion";
            break;
        default:
            templateName = "fire"; // Default to fire
            break;
        }

        // Configure the component using the template
        if (!setupParticleSystem(*component, templateName))
        {
            return nullptr;
        }

        // Store and return the component
        m_particleComponents[id] = component;

        // Set the component as enabled
        component->setEnabled(true);

        return component.get();
    }

    ParticleComponent *ParticleSystem::createCustomSystem(const std::string &id, const sf::Vector2f &position,
                                                          float emissionRate, sf::Time particleLifetime)
    {
        // Create a new component
        auto component = std::make_shared<ParticleComponent>();

        // Create custom emission parameters
        EmissionParameters params;
        params.emissionRate = emissionRate;
        params.minLifetime = particleLifetime.asSeconds();
        params.maxLifetime = particleLifetime.asSeconds() * 1.5f;

        // Apply parameters to component
        component->setEmissionParameters(params);
        component->setTriggerType(emissionRate > 0 ? ParticleTriggerType::Continuous : ParticleTriggerType::OneShot);

        // Store and return the component
        m_particleComponents[id] = component;

        // Set the component as enabled
        component->setEnabled(true);

        return component.get();
    }

    void ParticleSystem::update(float deltaTime)
    {
        // Update all particle components
        for (auto &pair : m_particleComponents)
        {
            if (pair.second)
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
            if (pair.second)
            {
                target.draw(*pair.second, states);
            }
        }
    }

    ParticleComponent *ParticleSystem::getComponent(const std::string &id)
    {
        auto it = m_particleComponents.find(id);
        if (it != m_particleComponents.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void ParticleSystem::removeComponent(const std::string &id)
    {
        m_particleComponents.erase(id);
    }

    bool ParticleSystem::addAffector(const std::string &particleId, AffectorType type, float strength)
    {
        // Find the particle component
        auto component = getComponent(particleId);
        if (!component)
        {
            return false;
        }

        // Implementation of custom affectors would go here
        // Since our system is simplified, we would need to implement actual affector logic
        // in the ParticleComponent::applyAffectors method

        return true;
    }

    void ParticleSystem::clearAllParticles()
    {
        for (auto &pair : m_particleComponents)
        {
            if (pair.second)
            {
                pair.second->clearParticles();
            }
        }
    }

} // namespace Orenji