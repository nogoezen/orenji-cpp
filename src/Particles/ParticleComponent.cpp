#include "../../include/Particles/ParticleComponent.hpp"
#include "../../include/Particles/ParticleSystem.hpp"
#include "../../include/Core/Entity.hpp"
#include <iostream>
#include <random>

namespace Orenji
{
    // Générateur de nombres aléatoires
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float random(float min, float max)
    {
        return min + dist(rng) * (max - min);
    }

    ParticleComponent::ParticleComponent()
        : m_triggerType(ParticleTriggerType::Continuous),
          m_emissionRate(30.f),
          m_burstSize(10),
          m_enabled(false),
          m_emissionAccumulator(0.f),
          m_lastPosition(0.f, 0.f),
          m_distanceTrigger(10.f),
          m_distanceAccumulator(0.f),
          m_parameters(nullptr),
          m_vertices(sf::PrimitiveType::Triangles),
          m_startColor(sf::Color(255, 255, 255, 255)),
          m_endColor(sf::Color(255, 255, 255, 0)),
          m_minSize(2.f),
          m_maxSize(5.f),
          m_minLifetime(1.f),
          m_maxLifetime(3.f)
    {
        // Configuration par défaut - peut être remplacée par loadFromFile ou setEmissionParameters
    }

    void ParticleComponent::initialize()
    {
        if (Entity *entity = getOwner())
        {
            m_lastPosition = entity->getTransform().getPosition();
        }
    }

    void ParticleComponent::update(float deltaTime)
    {
        if (!m_enabled || !getOwner())
            return;

        // Mettre à jour la position du système pour qu'elle corresponde à l'entité
        sf::Vector2f position = getOwner()->getTransform().getPosition();

        // Gérer les différents types de déclencheurs
        switch (m_triggerType)
        {
        case ParticleTriggerType::Continuous:
            // Calculer combien de particules à émettre ce frame
            m_emissionAccumulator += deltaTime * m_emissionRate;
            if (m_emissionAccumulator >= 1.0f)
            {
                int particlesToEmit = static_cast<int>(m_emissionAccumulator);
                emit(particlesToEmit);
                m_emissionAccumulator -= particlesToEmit;
            }
            break;

        case ParticleTriggerType::OneShot:
            // Si activé pour la première fois, émettre des particules une seule fois
            if (m_enabled)
            {
                emit(m_burstSize);
                m_enabled = false; // Désactiver après le one-shot
            }
            break;

        case ParticleTriggerType::OnCommand:
            // Rien à faire ici, l'émission est gérée manuellement
            break;

        case ParticleTriggerType::OnDistance:
            // Vérifier la distance parcourue
            {
                sf::Vector2f diff = position - m_lastPosition;
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                m_distanceAccumulator += distance;

                // Si la distance accumulée dépasse le seuil, émettre des particules
                if (m_distanceAccumulator >= m_distanceTrigger)
                {
                    int emitCount = static_cast<int>(m_distanceAccumulator / m_distanceTrigger);
                    emit(emitCount);
                    m_distanceAccumulator = std::fmod(m_distanceAccumulator, m_distanceTrigger);
                }

                m_lastPosition = position;
            }
            break;
        }

        // Mettre à jour toutes les particules
        for (size_t i = 0; i < m_particles.size();)
        {
            SimpleParticle &p = m_particles[i];
            p.elapsed += deltaTime;

            if (p.elapsed >= p.lifetime)
            {
                // Particule expirée, supprimer en échangeant avec la dernière
                m_particles[i] = m_particles.back();
                m_particles.pop_back();
            }
            else
            {
                // Appliquer le mouvement à la particule
                p.position += p.velocity * deltaTime;
                p.rotation += p.rotationSpeed * deltaTime;

                // Appliquer les affecteurs
                applyAffectors(p, deltaTime);

                // Passer à la particule suivante
                ++i;
            }
        }

        // Mettre à jour le vertex array pour le rendu
        m_vertices.resize(m_particles.size() * 6); // 6 vertices par particule (2 triangles)

        for (size_t i = 0; i < m_particles.size(); ++i)
        {
            const SimpleParticle &p = m_particles[i];

            // Calculer la taille en fonction du temps écoulé (peut décroître)
            float ratio = p.elapsed / p.lifetime;
            float size = p.size * (1.0f - ratio * 0.5f);

            // Calculer la couleur en fonction du temps écoulé
            sf::Color color = m_startColor;
            if (ratio > 0.0f)
            {
                color.r = static_cast<uint8_t>(m_startColor.r + (m_endColor.r - m_startColor.r) * ratio);
                color.g = static_cast<uint8_t>(m_startColor.g + (m_endColor.g - m_startColor.g) * ratio);
                color.b = static_cast<uint8_t>(m_startColor.b + (m_endColor.b - m_startColor.b) * ratio);
                color.a = static_cast<uint8_t>(m_startColor.a + (m_endColor.a - m_startColor.a) * ratio);
            }

            // Position des 4 sommets du quad
            float angle = p.rotation * 3.14159f / 180.f;
            float cos_a = std::cos(angle);
            float sin_a = std::sin(angle);

            // Vecteurs de taille pour chaque coin du quad
            sf::Vector2f v1(-size, -size);
            sf::Vector2f v2(size, -size);
            sf::Vector2f v3(size, size);
            sf::Vector2f v4(-size, size);

            // Appliquer la rotation à chaque vecteur
            sf::Vector2f r1(v1.x * cos_a - v1.y * sin_a, v1.x * sin_a + v1.y * cos_a);
            sf::Vector2f r2(v2.x * cos_a - v2.y * sin_a, v2.x * sin_a + v2.y * cos_a);
            sf::Vector2f r3(v3.x * cos_a - v3.y * sin_a, v3.x * sin_a + v3.y * cos_a);
            sf::Vector2f r4(v4.x * cos_a - v4.y * sin_a, v4.x * sin_a + v4.y * cos_a);

            // Indice de base dans le vertex array
            size_t idx = i * 6;

            // Premier triangle (v1, v2, v3)
            m_vertices[idx].position = p.position + r1;
            m_vertices[idx].color = color;
            m_vertices[idx].texCoords = sf::Vector2f(0, 0);

            m_vertices[idx + 1].position = p.position + r2;
            m_vertices[idx + 1].color = color;
            m_vertices[idx + 1].texCoords = sf::Vector2f(m_texture.getSize().x, 0);

            m_vertices[idx + 2].position = p.position + r3;
            m_vertices[idx + 2].color = color;
            m_vertices[idx + 2].texCoords = sf::Vector2f(m_texture.getSize().x, m_texture.getSize().y);

            // Deuxième triangle (v1, v3, v4)
            m_vertices[idx + 3].position = p.position + r1;
            m_vertices[idx + 3].color = color;
            m_vertices[idx + 3].texCoords = sf::Vector2f(0, 0);

            m_vertices[idx + 4].position = p.position + r3;
            m_vertices[idx + 4].color = color;
            m_vertices[idx + 4].texCoords = sf::Vector2f(m_texture.getSize().x, m_texture.getSize().y);

            m_vertices[idx + 5].position = p.position + r4;
            m_vertices[idx + 5].color = color;
            m_vertices[idx + 5].texCoords = sf::Vector2f(0, m_texture.getSize().y);
        }
    }

    bool ParticleComponent::loadFromFile(const std::string &templateName)
    {
        // Essayer de charger depuis le gestionnaire de particules
        auto &system = ParticleSystem::getInstance();

        // Configurera this avec le template spécifié
        return system.setupParticleSystem(*this, templateName);
    }

    void ParticleComponent::setTexture(const std::string &texturePath)
    {
        if (!m_texture.loadFromFile(texturePath))
        {
            std::cerr << "Erreur: Impossible de charger la texture de particule: " << texturePath << std::endl;
        }
    }

    void ParticleComponent::setEmissionRate(float rate)
    {
        m_emissionRate = rate;
    }

    void ParticleComponent::setBurstSize(unsigned int count)
    {
        m_burstSize = count;
    }

    void ParticleComponent::setTriggerType(ParticleTriggerType type)
    {
        m_triggerType = type;

        // Réinitialiser les variables si nécessaire
        if (m_triggerType == ParticleTriggerType::OnDistance && getOwner())
        {
            m_lastPosition = getOwner()->getTransform().getPosition();
            m_distanceAccumulator = 0.f;
        }
        else if (m_triggerType == ParticleTriggerType::Continuous)
        {
            m_emissionAccumulator = 0.f;
        }
    }

    void ParticleComponent::emit(unsigned int count)
    {
        if (count == 0)
            count = m_burstSize;

        // Générer les nouvelles particules
        for (unsigned int i = 0; i < count; ++i)
        {
            m_particles.push_back(generateParticle());
        }
    }

    void ParticleComponent::setEnabled(bool enabled)
    {
        // Si l'état change
        if (m_enabled != enabled)
        {
            m_enabled = enabled;

            // Réinitialiser les accumulateurs selon le type
            if (m_enabled)
            {
                if (m_triggerType == ParticleTriggerType::Continuous)
                {
                    m_emissionAccumulator = 0.f;
                }
                else if (m_triggerType == ParticleTriggerType::OneShot)
                {
                    emit(m_burstSize);
                    m_enabled = false; // Désactiver après le one-shot
                }
                else if (m_triggerType == ParticleTriggerType::OnDistance && getOwner())
                {
                    m_lastPosition = getOwner()->getTransform().getPosition();
                    m_distanceAccumulator = 0.f;
                }
            }
        }
    }

    bool ParticleComponent::isEnabled() const
    {
        return m_enabled;
    }

    unsigned int ParticleComponent::getParticleCount() const
    {
        return static_cast<unsigned int>(m_particles.size());
    }

    void ParticleComponent::clearParticles()
    {
        m_particles.clear();
    }

    void ParticleComponent::setEmissionParameters(const EmissionParameters &params)
    {
        m_parameters = const_cast<EmissionParameters *>(&params);
        m_emissionRate = params.emissionRate;
        m_minLifetime = params.minLifetime;
        m_maxLifetime = params.maxLifetime;
        m_minSize = params.minSize;
        m_maxSize = params.maxSize;
        m_startColor = params.startColor;
        m_endColor = params.endColor;
    }

    void ParticleComponent::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (m_particles.empty())
            return;

        // Applique la texture si disponible
        if (m_texture.getSize().x > 0)
        {
            states.texture = &m_texture;
        }

        // Dessiner toutes les particules en une fois
        target.draw(m_vertices, states);
    }

    SimpleParticle ParticleComponent::generateParticle() const
    {
        // Position d'émission basée sur l'entité
        sf::Vector2f position = getOwner() ? getOwner()->getTransform().getPosition() : sf::Vector2f(0.f, 0.f);

        // Si nous avons des paramètres d'émission personnalisés, les utiliser
        if (m_parameters)
        {
            // Calculer la position avec un décalage aléatoire
            float offsetAngle = random(0.f, 360.f) * 3.14159f / 180.f;
            float offsetDistance = random(0.f, m_parameters->positionRadius);
            sf::Vector2f offset(
                std::cos(offsetAngle) * offsetDistance + m_parameters->positionOffset.x,
                std::sin(offsetAngle) * offsetDistance + m_parameters->positionOffset.y);

            // Calculer la vitesse avec une variation aléatoire
            float speed = random(m_parameters->minSpeed, m_parameters->maxSpeed);
            float angle = random(0.f, 360.f) * 3.14159f / 180.f;

            // Permettre une déviation autour d'une direction de base
            if (m_parameters->baseVelocity != sf::Vector2f(0.f, 0.f))
            {
                float baseAngle = std::atan2(m_parameters->baseVelocity.y, m_parameters->baseVelocity.x);
                float spread = m_parameters->velocitySpread * 3.14159f / 180.f; // Convertir degrés en radians
                angle = baseAngle + random(-spread, spread);
            }

            SimpleParticle p;
            p.position = position + offset;
            p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
            p.lifetime = random(m_parameters->minLifetime, m_parameters->maxLifetime);
            p.elapsed = 0.f;
            p.size = random(m_parameters->minSize, m_parameters->maxSize);
            p.rotation = random(0.f, 360.f);
            p.rotationSpeed = random(m_parameters->minRotationSpeed, m_parameters->maxRotationSpeed);
            p.color = m_parameters->startColor;

            return p;
        }
        else
        {
            // Utiliser des valeurs par défaut
            SimpleParticle p;
            p.position = position + sf::Vector2f(random(-2.f, 2.f), random(-2.f, 2.f));

            float angle = random(0.f, 360.f) * 3.14159f / 180.f;
            float speed = random(20.f, 50.f);
            p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

            p.lifetime = random(m_minLifetime, m_maxLifetime);
            p.elapsed = 0.f;
            p.size = random(m_minSize, m_maxSize);
            p.rotation = random(0.f, 360.f);
            p.rotationSpeed = random(-180.f, 180.f);
            p.color = m_startColor;

            return p;
        }
    }

    void ParticleComponent::applyAffectors(SimpleParticle &particle, float deltaTime)
    {
        // Simulation de gravité simple
        particle.velocity.y += 30.f * deltaTime; // Force de gravité

        // Résistance de l'air (ralentit les particules)
        particle.velocity *= 0.99f;

        // Ajuster l'alpha en fonction du temps de vie (effet de fondu)
        float lifetimeRatio = particle.elapsed / particle.lifetime;
        if (m_parameters)
        {
            particle.color.r = static_cast<uint8_t>(m_parameters->startColor.r + (m_parameters->endColor.r - m_parameters->startColor.r) * lifetimeRatio);
            particle.color.g = static_cast<uint8_t>(m_parameters->startColor.g + (m_parameters->endColor.g - m_parameters->startColor.g) * lifetimeRatio);
            particle.color.b = static_cast<uint8_t>(m_parameters->startColor.b + (m_parameters->endColor.b - m_parameters->startColor.b) * lifetimeRatio);
            particle.color.a = static_cast<uint8_t>(m_parameters->startColor.a + (m_parameters->endColor.a - m_parameters->startColor.a) * lifetimeRatio);
        }
        else
        {
            particle.color.a = static_cast<uint8_t>(m_startColor.a * (1.f - lifetimeRatio));
        }
    }

} // namespace Orenji