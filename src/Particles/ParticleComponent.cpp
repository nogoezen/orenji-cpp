#include "Particles/ParticleComponent.hpp"
#include "Core/ResourceManager.hpp"
#include "utils/MathUtils.hpp"
#include <iostream>
#include <random>
#include <cmath>

namespace Orenji
{
#ifndef DISABLE_THOR
    ParticleComponent::ParticleComponent(const std::string &id)
        : Component(id), m_emissionRate(30.0f), m_emissionTime(0.0f), m_triggerType(ParticleTriggerType::Continuous), m_enabled(true), m_isActive(false), m_continuousEmitting(false), m_position(0.0f, 0.0f)
    {
        m_emitter = std::make_unique<thor::UniversalEmitter>();
        m_emitter->setEmissionRate(m_emissionRate);
        m_emitter->setParticleLifetime(thor::Distributions::uniform(sf::seconds(1.0f), sf::seconds(3.0f)));
        m_emitter->setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(0.0f, -30.0f), 10.0f));
        m_emitter->setParticlePosition(thor::Distributions::circle(sf::Vector2f(0.0f, 0.0f), 2.0f));
        m_emitter->setParticleRotation(thor::Distributions::uniform(0.0f, 360.0f));
        m_system.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.1f, 0.1f)));
    }

    ParticleComponent::~ParticleComponent()
    {
        if (m_connection.isConnected())
            m_connection.disconnect();
    }

    void ParticleComponent::initialize()
    {
        if (m_triggerType == ParticleTriggerType::Continuous && m_enabled)
        {
            m_continuousEmitting = true;
            m_connection = m_system.addEmitter(*m_emitter);
        }
    }

    void ParticleComponent::update(float dt)
    {
        m_system.update(sf::seconds(dt));

        if (m_system.getParticleCount() == 0 && !m_continuousEmitting)
            m_isActive = false;
        else
            m_isActive = true;
    }

    void ParticleComponent::draw(sf::RenderTarget &target)
    {
        sf::RenderStates states;
        states.transform.translate(m_position);
        target.draw(m_system, states);
    }

    bool ParticleComponent::loadFromFile(const std::string &filename)
    {
        // Not implemented for Thor version
        return false;
    }

    void ParticleComponent::setTexture(const sf::Texture *texture, const sf::IntRect &textureRect)
    {
        if (texture)
        {
            m_system.setTexture(texture);
            if (textureRect != sf::IntRect())
                m_system.setTextureRect(textureRect);
        }
    }

    void ParticleComponent::setEmissionRate(float rate)
    {
        m_emissionRate = rate;
        m_emitter->setEmissionRate(rate);
    }

    void ParticleComponent::setTriggerType(ParticleTriggerType triggerType)
    {
        m_triggerType = triggerType;

        if (m_connection.isConnected())
            m_connection.disconnect();

        m_continuousEmitting = false;

        if (m_triggerType == ParticleTriggerType::Continuous && m_enabled)
        {
            m_continuousEmitting = true;
            m_connection = m_system.addEmitter(*m_emitter);
        }
    }

    void ParticleComponent::emit(unsigned int count)
    {
        m_system.emitParticles(count);
        m_isActive = true;
    }

    bool ParticleComponent::isSystemActive() const
    {
        return m_isActive;
    }

    bool ParticleComponent::isContinuousEmitting() const
    {
        return m_continuousEmitting;
    }

    void ParticleComponent::setEnabled(bool enabled)
    {
        m_enabled = enabled;

        if (m_connection.isConnected())
            m_connection.disconnect();

        m_continuousEmitting = false;

        if (m_triggerType == ParticleTriggerType::Continuous && m_enabled)
        {
            m_continuousEmitting = true;
            m_connection = m_system.addEmitter(*m_emitter);
        }
    }

    bool ParticleComponent::isEnabled() const
    {
        return m_enabled;
    }

    void ParticleComponent::setPosition(const sf::Vector2f &position)
    {
        m_position = position;
    }
#else
    ParticleComponent::ParticleComponent(const std::string &id)
        : Component(id), m_emissionRate(30.0f), m_emissionTime(0.0f), m_triggerType(ParticleTriggerType::Continuous), m_enabled(true), m_isActive(false), m_continuousEmitting(false), m_position(0.0f, 0.0f), m_distanceTraveled(0.0f), m_texture(nullptr)
    {
        m_vertices.setPrimitiveType(sf::PrimitiveType::Quads);

        // Paramètres d'émission par défaut
        m_emissionParams.emissionRate = 30.0f;
        m_emissionParams.minLifetime = 1.0f;
        m_emissionParams.maxLifetime = 3.0f;
        m_emissionParams.minSize = 2.0f;
        m_emissionParams.maxSize = 5.0f;
        m_emissionParams.minSpeed = 20.0f;
        m_emissionParams.maxSpeed = 50.0f;
        m_emissionParams.startColor = sf::Color::White;
        m_emissionParams.endColor = sf::Color(255, 255, 255, 0);
    }

    ParticleComponent::~ParticleComponent()
    {
    }

    void ParticleComponent::initialize()
    {
        if (m_triggerType == ParticleTriggerType::Continuous && m_enabled)
        {
            m_continuousEmitting = true;
        }
    }

    void ParticleComponent::update(float dt)
    {
        // Mettre à jour le temps d'émission
        if (m_continuousEmitting)
        {
            m_emissionTime += dt;
            float particlesToEmit = m_emissionTime * m_emissionRate;
            if (particlesToEmit >= 1.0f)
            {
                int count = static_cast<int>(particlesToEmit);
                m_emissionTime -= count / m_emissionRate;
                emit(count);
            }
        }

        // Mettre à jour les particules existantes
        for (auto it = m_particles.begin(); it != m_particles.end();)
        {
            SimpleParticle &p = *it;
            p.elapsed += dt;

            if (p.elapsed >= p.lifetime)
            {
                // Supprimer les particules expirées
                it = m_particles.erase(it);
                continue;
            }

            // Mettre à jour position et rotation
            p.position += p.velocity * dt;
            p.rotation += p.rotationSpeed * dt;

            // Appliquer la gravité (simple affecteur)
            p.velocity.y += 9.81f * dt;

            ++it;
        }

        // Mettre à jour l'état actif
        m_isActive = (!m_particles.empty() || m_continuousEmitting);

        // Mettre à jour les vertex pour le rendu
        updateVertices();
    }

    void ParticleComponent::draw(sf::RenderTarget &target)
    {
        if (m_particles.empty())
            return;

        sf::RenderStates states;
        states.transform.translate(m_position);
        states.texture = m_texture;
        target.draw(m_vertices, states);
    }

    bool ParticleComponent::loadFromFile(const std::string &filename)
    {
        // À implémenter pour charger les paramètres depuis un fichier JSON/XML
        return false;
    }

    void ParticleComponent::setTexture(const sf::Texture *texture, const sf::IntRect &textureRect)
    {
        m_texture = texture;
        m_textureRect = textureRect.width > 0 ? textureRect : (texture ? sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y) : sf::IntRect());
    }

    void ParticleComponent::setEmissionRate(float rate)
    {
        m_emissionRate = rate;
        m_emissionParams.emissionRate = rate;
    }

    void ParticleComponent::setTriggerType(ParticleTriggerType triggerType)
    {
        m_triggerType = triggerType;
        m_continuousEmitting = (m_triggerType == ParticleTriggerType::Continuous && m_enabled);
    }

    void ParticleComponent::emit(unsigned int count)
    {
        for (unsigned int i = 0; i < count; ++i)
        {
            m_particles.push_back(createParticle());
        }

        // Mise à jour des vertices après l'ajout de particules
        updateVertices();

        m_isActive = true;
    }

    bool ParticleComponent::isSystemActive() const
    {
        return m_isActive;
    }

    bool ParticleComponent::isContinuousEmitting() const
    {
        return m_continuousEmitting;
    }

    void ParticleComponent::setEnabled(bool enabled)
    {
        m_enabled = enabled;
        m_continuousEmitting = (m_triggerType == ParticleTriggerType::Continuous && m_enabled);
    }

    bool ParticleComponent::isEnabled() const
    {
        return m_enabled;
    }

    void ParticleComponent::setPosition(const sf::Vector2f &position)
    {
        m_position = position;
    }

    void ParticleComponent::setEmissionParameters(const EmissionParameters &params)
    {
        m_emissionParams = params;
        m_emissionRate = params.emissionRate;
    }

    const EmissionParameters &ParticleComponent::getEmissionParameters() const
    {
        return m_emissionParams;
    }

    SimpleParticle ParticleComponent::createParticle()
    {
        SimpleParticle particle;

        // Position aléatoire dans un cercle autour de l'offset
        float angle = MathUtils::randomFloat(0.0f, 2.0f * 3.14159f);
        float distance = MathUtils::randomFloat(0.0f, m_emissionParams.positionRadius);
        particle.position = m_emissionParams.positionOffset + sf::Vector2f(std::cos(angle) * distance, std::sin(angle) * distance);

        // Vitesse aléatoire basée sur l'angle et la vitesse
        float speed = MathUtils::randomFloat(m_emissionParams.minSpeed, m_emissionParams.maxSpeed);
        float velocityAngle = MathUtils::randomFloat(-m_emissionParams.velocitySpread, m_emissionParams.velocitySpread);
        velocityAngle = (m_emissionParams.baseVelocity.y != 0 || m_emissionParams.baseVelocity.x != 0) ? std::atan2(m_emissionParams.baseVelocity.y, m_emissionParams.baseVelocity.x) + velocityAngle : velocityAngle;
        particle.velocity = sf::Vector2f(std::cos(velocityAngle) * speed, std::sin(velocityAngle) * speed);

        // Autres propriétés aléatoires
        particle.color = m_emissionParams.startColor;
        particle.lifetime = MathUtils::randomFloat(m_emissionParams.minLifetime, m_emissionParams.maxLifetime);
        particle.elapsed = 0.0f;
        particle.size = MathUtils::randomFloat(m_emissionParams.minSize, m_emissionParams.maxSize);
        particle.rotation = MathUtils::randomFloat(0.0f, 360.0f);
        particle.rotationSpeed = MathUtils::randomFloat(m_emissionParams.minRotationSpeed, m_emissionParams.maxRotationSpeed);

        return particle;
    }

    sf::Color ParticleComponent::lerpColor(const sf::Color &start, const sf::Color &end, float alpha)
    {
        sf::Color result;
        result.r = static_cast<uint8_t>(start.r + (end.r - start.r) * alpha);
        result.g = static_cast<uint8_t>(start.g + (end.g - start.g) * alpha);
        result.b = static_cast<uint8_t>(start.b + (end.b - start.b) * alpha);
        result.a = static_cast<uint8_t>(start.a + (end.a - start.a) * alpha);
        return result;
    }

    // Méthode privée pour mettre à jour les vertices
    void ParticleComponent::updateVertices()
    {
        m_vertices.clear();
        m_vertices.resize(m_particles.size() * 4);

        for (size_t i = 0; i < m_particles.size(); ++i)
        {
            const SimpleParticle &p = m_particles[i];
            float ratio = p.elapsed / p.lifetime;
            sf::Color color = lerpColor(m_emissionParams.startColor, m_emissionParams.endColor, ratio);

            // Calculer les coordonnées du quad
            float halfSize = p.size / 2.0f;

            // Appliquer la rotation
            float angle = p.rotation * 3.14159f / 180.0f;
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);

            sf::Vector2f points[4] = {
                sf::Vector2f(-halfSize, -halfSize),
                sf::Vector2f(halfSize, -halfSize),
                sf::Vector2f(halfSize, halfSize),
                sf::Vector2f(-halfSize, halfSize)};

            // Appliquer la rotation aux points
            for (int j = 0; j < 4; ++j)
            {
                sf::Vector2f rotated;
                rotated.x = points[j].x * cosA - points[j].y * sinA;
                rotated.y = points[j].x * sinA + points[j].y * cosA;
                points[j] = rotated + p.position;
            }

            // Définir les coordonnées de texture en fonction de m_textureRect
            sf::FloatRect texCoords;
            if (m_texture)
            {
                texCoords.left = static_cast<float>(m_textureRect.left);
                texCoords.top = static_cast<float>(m_textureRect.top);
                texCoords.width = static_cast<float>(m_textureRect.width);
                texCoords.height = static_cast<float>(m_textureRect.height);
            }

            // Configurer les vertices
            sf::Vertex *quad = &m_vertices[i * 4];

            quad[0].position = points[0];
            quad[1].position = points[1];
            quad[2].position = points[2];
            quad[3].position = points[3];

            quad[0].color = quad[1].color = quad[2].color = quad[3].color = color;

            if (m_texture)
            {
                quad[0].texCoords = sf::Vector2f(texCoords.left, texCoords.top);
                quad[1].texCoords = sf::Vector2f(texCoords.left + texCoords.width, texCoords.top);
                quad[2].texCoords = sf::Vector2f(texCoords.left + texCoords.width, texCoords.top + texCoords.height);
                quad[3].texCoords = sf::Vector2f(texCoords.left, texCoords.top + texCoords.height);
            }
        }
    }
#endif
}