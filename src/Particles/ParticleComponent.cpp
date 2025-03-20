#include "../../include/Particles/ParticleComponent.hpp"
#include "../../include/Particles/ParticleSystem.hpp"
#include "../../include/Core/Entity.hpp"
#include <iostream>

namespace Orenji
{
    ParticleComponent::ParticleComponent()
        : m_particleSystem(thor::Particles::System<thor::EmptyAdder>::create()), m_triggerType(ParticleTriggerType::Continuous), m_emissionRate(30.f), m_burstSize(10), m_enabled(false), m_lastPosition(0.f, 0.f), m_distanceTrigger(10.f), m_distanceAccumulator(0.f)
    {
        // Configurer l'émetteur par défaut
        m_emitter.setEmissionRate(m_emissionRate);
        m_emitter.setParticleLifetime(thor::Distributions::uniform(1.f, 3.f));
        m_emitter.setParticlePosition(thor::Distributions::circle(sf::Vector2f(0.f, 0.f), 2.f));
        m_emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(0.f, -30.f), 10.f));
        m_emitter.setParticleRotation(thor::Distributions::uniform(0.f, 360.f));
    }

    ParticleComponent::~ParticleComponent()
    {
        // Déconnecter l'émetteur si connecté
        if (m_connection.isConnected())
        {
            m_connection.disconnect();
        }
    }

    void ParticleComponent::initialize()
    {
        if (m_triggerType == ParticleTriggerType::Continuous && m_enabled)
        {
            // Connecter l'émetteur pour une émission continue
            m_connection = m_particleSystem.addEmitter(m_emitter);
        }

        if (Entity *entity = getEntity())
        {
            m_lastPosition = entity->getPosition();
        }
    }

    void ParticleComponent::update(float deltaTime)
    {
        if (!m_enabled || !getEntity())
            return;

        // Mettre à jour la position de l'émetteur pour qu'elle corresponde à l'entité
        sf::Vector2f position = getEntity()->getPosition();
        m_emitter.setParticlePosition(thor::Distributions::circle(position, 2.f));

        // Gérer les différents types de déclencheurs
        switch (m_triggerType)
        {
        case ParticleTriggerType::Continuous:
            // Déjà géré par la connexion
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

        // Mettre à jour le système de particules
        m_particleSystem.update(sf::seconds(deltaTime));
    }

    bool ParticleComponent::loadFromFile(const std::string &filename)
    {
        // Pour l'instant, on va supposer que le fichier contient simplement le nom d'un template prédéfini
        try
        {
            auto &particleSystem = ParticleSystem::getInstance();

            // Essayer de configurer avec un template
            return particleSystem.setupParticleSystem(m_particleSystem, m_emitter, filename);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erreur lors du chargement de la configuration de particules: " << e.what() << std::endl;
            return false;
        }
    }

    void ParticleComponent::setTexture(const std::string &texturePath)
    {
        if (m_texture.loadFromFile(texturePath))
        {
            m_particleSystem.setTexture(m_texture);
        }
        else
        {
            std::cerr << "Erreur: Impossible de charger la texture: " << texturePath << std::endl;
        }
    }

    void ParticleComponent::setEmissionRate(float rate)
    {
        m_emissionRate = rate;
        m_emitter.setEmissionRate(rate);
    }

    void ParticleComponent::setBurstSize(unsigned int count)
    {
        m_burstSize = count;
    }

    void ParticleComponent::setTriggerType(ParticleTriggerType type)
    {
        // Si on passe d'un type continu à un autre type, déconnecter l'émetteur
        if (m_triggerType == ParticleTriggerType::Continuous && type != ParticleTriggerType::Continuous)
        {
            if (m_connection.isConnected())
            {
                m_connection.disconnect();
            }
        }

        m_triggerType = type;

        // Si on passe à un type continu et que le système est activé, connecter l'émetteur
        if (m_triggerType == ParticleTriggerType::Continuous && m_enabled)
        {
            m_connection = m_particleSystem.addEmitter(m_emitter);
        }

        // Réinitialiser les variables pour le déclencheur basé sur la distance
        if (m_triggerType == ParticleTriggerType::OnDistance && getEntity())
        {
            m_lastPosition = getEntity()->getPosition();
            m_distanceAccumulator = 0.f;
        }
    }

    void ParticleComponent::emit(unsigned int count)
    {
        if (count == 0)
            count = m_burstSize;

        m_particleSystem.emitParticles(count, m_emitter);
    }

    void ParticleComponent::setEnabled(bool enabled)
    {
        // Si l'état change
        if (m_enabled != enabled)
        {
            m_enabled = enabled;

            // Pour le type continu, connecter ou déconnecter l'émetteur
            if (m_triggerType == ParticleTriggerType::Continuous)
            {
                if (m_enabled)
                {
                    m_connection = m_particleSystem.addEmitter(m_emitter);
                }
                else if (m_connection.isConnected())
                {
                    m_connection.disconnect();
                }
            }

            // Pour le type one-shot, émettre des particules si activé
            if (m_triggerType == ParticleTriggerType::OneShot && m_enabled)
            {
                emit(m_burstSize);
            }

            // Pour le type distance, réinitialiser si activé
            if (m_triggerType == ParticleTriggerType::OnDistance && m_enabled && getEntity())
            {
                m_lastPosition = getEntity()->getPosition();
                m_distanceAccumulator = 0.f;
            }
        }
    }

    bool ParticleComponent::isEnabled() const
    {
        return m_enabled;
    }

    unsigned int ParticleComponent::getParticleCount() const
    {
        return m_particleSystem.getParticleCount();
    }

    void ParticleComponent::clearParticles()
    {
        m_particleSystem.clearParticles();
    }

    void ParticleComponent::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Dessiner le système de particules
        target.draw(m_particleSystem, states);
    }

} // namespace Orenji