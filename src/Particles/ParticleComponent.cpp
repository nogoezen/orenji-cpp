#include "Particles/ParticleComponent.hpp"
#include "Core/Entity.hpp"
#include <iostream>
#include <sstream>

namespace Orenji
{

    // Initialisation de l'ID du prochain émetteur
    unsigned int ParticleComponent::s_nextEmitterId = 0;

    ParticleComponent::ParticleComponent(const std::string &name)
        : Component(name), m_emitter(nullptr), m_triggerType(ParticleTriggerType::Continuous),
          m_emissionRate(10.f), m_burstSize(10), m_emissionAccumulator(0.f),
          m_emitting(false), m_texture(nullptr)
    {
        // Génère un nom unique pour l'émetteur
        std::ostringstream oss;
        oss << "Emitter_" << s_nextEmitterId++;
        m_emitterName = oss.str();

        // Crée l'émetteur dans le système de particules
        ParticleSystem::getInstance().createEmitter(m_emitterName);
        m_emitter = ParticleSystem::getInstance().getEmitter(m_emitterName);
    }

    ParticleComponent::~ParticleComponent()
    {
        // Supprime l'émetteur du système de particules
        ParticleSystem::getInstance().removeEmitter(m_emitterName);
    }

    bool ParticleComponent::initialize()
    {
        return Component::initialize();
    }

    void ParticleComponent::update(float deltaTime)
    {
        if (!isActive() || !m_emitter || !m_emitting)
        {
            return;
        }

        // Met à jour la position de l'émetteur avec celle de l'entité
        if (getOwner())
        {
            m_emitter->setPosition(getOwner()->getPosition());
        }

        // Gère l'émission selon le type de déclenchement
        switch (m_triggerType)
        {
        case ParticleTriggerType::Continuous:
            // Émission continue basée sur le taux d'émission
            m_emissionAccumulator += deltaTime * m_emissionRate;
            if (m_emissionAccumulator >= 1.f)
            {
                unsigned int particlesToEmit = static_cast<unsigned int>(m_emissionAccumulator);
                m_emitter->emit(particlesToEmit);
                m_emissionAccumulator -= particlesToEmit;
            }
            break;

        case ParticleTriggerType::Burst:
            // Émission en rafale (une seule fois au démarrage)
            if (m_emissionAccumulator == 0.f)
            {
                m_emitter->emit(m_burstSize);
                m_emissionAccumulator = 1.f; // Marque que l'émission a été faite
            }
            break;

        case ParticleTriggerType::OnEvent:
            // L'émission est déclenchée explicitement via la méthode trigger()
            break;
        }
    }

    void ParticleComponent::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Le rendu est géré par le ParticleSystem
        // Nous n'avons rien à faire ici car tous les émetteurs sont dessinés ensemble
    }

    bool ParticleComponent::loadFromFile(const std::string &filename)
    {
        // Cette méthode pourrait être implémentée pour charger une configuration de particules à partir d'un fichier JSON
        // Pour l'instant, on retourne simplement false
        std::cerr << "ParticleComponent::loadFromFile: Non implémenté" << std::endl;
        return false;
    }

    void ParticleComponent::setTexture(sf::Texture *texture)
    {
        m_texture = texture;
        ParticleSystem::getInstance().setTexture(texture);
    }

    void ParticleComponent::setEmissionRate(float particlesPerSecond)
    {
        m_emissionRate = particlesPerSecond;
    }

    float ParticleComponent::getEmissionRate() const
    {
        return m_emissionRate;
    }

    void ParticleComponent::setBurstSize(unsigned int burstSize)
    {
        m_burstSize = burstSize;
    }

    unsigned int ParticleComponent::getBurstSize() const
    {
        return m_burstSize;
    }

    void ParticleComponent::setTriggerType(ParticleTriggerType triggerType)
    {
        m_triggerType = triggerType;
        m_emissionAccumulator = 0.f; // Réinitialise l'accumulateur lors du changement de type
    }

    ParticleTriggerType ParticleComponent::getTriggerType() const
    {
        return m_triggerType;
    }

    void ParticleComponent::trigger()
    {
        if (m_emitter && m_triggerType == ParticleTriggerType::OnEvent)
        {
            m_emitter->emit(m_burstSize);
        }
    }

    bool ParticleComponent::isEmitting() const
    {
        return m_emitting;
    }

    void ParticleComponent::start()
    {
        m_emitting = true;
        m_emissionAccumulator = 0.f;
    }

    void ParticleComponent::stop()
    {
        m_emitting = false;
    }

    ParticleEmitter *ParticleComponent::getEmitter()
    {
        return m_emitter;
    }

    void ParticleComponent::addAffector(const ParticleAffector &affector)
    {
        if (m_emitter)
        {
            m_emitter->addAffector(affector);
        }
    }

} // namespace Orenji