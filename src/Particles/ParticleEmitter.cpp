#include "Particles/ParticleEmitter.hpp"
#include "Utils/MathUtils.hpp"

namespace Orenji
{

    ParticleEmitter::ParticleEmitter(unsigned int maxParticles)
        : m_position(0.f, 0.f), m_size(10.f, 10.f), m_shape(EmitterShape::Point),
          m_minLife(1.f), m_maxLife(3.f),
          m_minStartSize(2.f), m_maxStartSize(5.f),
          m_minEndSize(0.1f), m_maxEndSize(0.5f),
          m_minVelocity(10.f), m_maxVelocity(50.f),
          m_minAngle(0.f), m_maxAngle(360.f),
          m_minRotationSpeed(-90.f), m_maxRotationSpeed(90.f),
          m_startColor(sf::Color::White), m_endColor(sf::Color::Transparent)
    {
        // Réserve de l'espace pour les particules
        m_particles.resize(maxParticles);
    }

    ParticleEmitter::~ParticleEmitter()
    {
        m_affectors.clear();
    }

    void ParticleEmitter::emit(unsigned int count)
    {
        unsigned int emitted = 0;
        for (auto &particle : m_particles)
        {
            if (!particle.active)
            {
                initializeParticle(particle);
                emitted++;

                if (emitted >= count)
                {
                    break;
                }
            }
        }
    }

    void ParticleEmitter::update(float deltaTime)
    {
        for (auto &particle : m_particles)
        {
            if (particle.active)
            {
                // Met à jour la particule
                if (particle.update(deltaTime))
                {
                    // Applique les affecteurs
                    for (const auto &affector : m_affectors)
                    {
                        affector(particle, deltaTime);
                    }
                }
            }
        }
    }

    void ParticleEmitter::addAffector(const ParticleAffector &affector)
    {
        m_affectors.push_back(affector);
    }

    void ParticleEmitter::clearAffectors()
    {
        m_affectors.clear();
    }

    void ParticleEmitter::setPosition(const sf::Vector2f &position)
    {
        m_position = position;
    }

    const sf::Vector2f &ParticleEmitter::getPosition() const
    {
        return m_position;
    }

    void ParticleEmitter::setShape(EmitterShape shape)
    {
        m_shape = shape;
    }

    EmitterShape ParticleEmitter::getShape() const
    {
        return m_shape;
    }

    void ParticleEmitter::setSize(const sf::Vector2f &size)
    {
        m_size = size;
    }

    const sf::Vector2f &ParticleEmitter::getSize() const
    {
        return m_size;
    }

    void ParticleEmitter::setMinLife(float minLife)
    {
        m_minLife = minLife;
    }

    float ParticleEmitter::getMinLife() const
    {
        return m_minLife;
    }

    void ParticleEmitter::setMaxLife(float maxLife)
    {
        m_maxLife = maxLife;
    }

    float ParticleEmitter::getMaxLife() const
    {
        return m_maxLife;
    }

    void ParticleEmitter::setMinStartSize(float minStartSize)
    {
        m_minStartSize = minStartSize;
    }

    float ParticleEmitter::getMinStartSize() const
    {
        return m_minStartSize;
    }

    void ParticleEmitter::setMaxStartSize(float maxStartSize)
    {
        m_maxStartSize = maxStartSize;
    }

    float ParticleEmitter::getMaxStartSize() const
    {
        return m_maxStartSize;
    }

    void ParticleEmitter::setMinEndSize(float minEndSize)
    {
        m_minEndSize = minEndSize;
    }

    float ParticleEmitter::getMinEndSize() const
    {
        return m_minEndSize;
    }

    void ParticleEmitter::setMaxEndSize(float maxEndSize)
    {
        m_maxEndSize = maxEndSize;
    }

    float ParticleEmitter::getMaxEndSize() const
    {
        return m_maxEndSize;
    }

    void ParticleEmitter::setMinVelocity(float minVelocity)
    {
        m_minVelocity = minVelocity;
    }

    float ParticleEmitter::getMinVelocity() const
    {
        return m_minVelocity;
    }

    void ParticleEmitter::setMaxVelocity(float maxVelocity)
    {
        m_maxVelocity = maxVelocity;
    }

    float ParticleEmitter::getMaxVelocity() const
    {
        return m_maxVelocity;
    }

    void ParticleEmitter::setMinAngle(float minAngle)
    {
        m_minAngle = minAngle;
    }

    float ParticleEmitter::getMinAngle() const
    {
        return m_minAngle;
    }

    void ParticleEmitter::setMaxAngle(float maxAngle)
    {
        m_maxAngle = maxAngle;
    }

    float ParticleEmitter::getMaxAngle() const
    {
        return m_maxAngle;
    }

    void ParticleEmitter::setMinRotationSpeed(float minRotation)
    {
        m_minRotationSpeed = minRotation;
    }

    float ParticleEmitter::getMinRotationSpeed() const
    {
        return m_minRotationSpeed;
    }

    void ParticleEmitter::setMaxRotationSpeed(float maxRotation)
    {
        m_maxRotationSpeed = maxRotation;
    }

    float ParticleEmitter::getMaxRotationSpeed() const
    {
        return m_maxRotationSpeed;
    }

    void ParticleEmitter::setStartColor(const sf::Color &color)
    {
        m_startColor = color;
    }

    const sf::Color &ParticleEmitter::getStartColor() const
    {
        return m_startColor;
    }

    void ParticleEmitter::setEndColor(const sf::Color &color)
    {
        m_endColor = color;
    }

    const sf::Color &ParticleEmitter::getEndColor() const
    {
        return m_endColor;
    }

    const std::vector<Particle> &ParticleEmitter::getParticles() const
    {
        return m_particles;
    }

    unsigned int ParticleEmitter::getActiveParticleCount() const
    {
        unsigned int count = 0;
        for (const auto &particle : m_particles)
        {
            if (particle.active)
            {
                count++;
            }
        }
        return count;
    }

    void ParticleEmitter::initializeParticle(Particle &particle)
    {
        // Position d'émission selon la forme
        particle.position = getEmissionPosition();

        // Durée de vie
        particle.totalLife = Utils::MathUtils::randFloat(m_minLife, m_maxLife);
        particle.life = particle.totalLife;

        // Taille
        particle.startSize = Utils::MathUtils::randFloat(m_minStartSize, m_maxStartSize);
        particle.endSize = Utils::MathUtils::randFloat(m_minEndSize, m_maxEndSize);
        particle.size = particle.startSize;

        // Couleur
        particle.startColor = m_startColor;
        particle.endColor = m_endColor;
        particle.color = particle.startColor;

        // Direction et vitesse
        float angle = Utils::MathUtils::randFloat(m_minAngle, m_maxAngle);
        float velocity = Utils::MathUtils::randFloat(m_minVelocity, m_maxVelocity);
        particle.velocity = Utils::MathUtils::vectorFromAngle(angle, velocity);

        // Rotation
        particle.rotation = Utils::MathUtils::randFloat(0.f, 360.f);
        particle.rotationSpeed = Utils::MathUtils::randFloat(m_minRotationSpeed, m_maxRotationSpeed);

        // Activation
        particle.active = true;
    }

    sf::Vector2f ParticleEmitter::getEmissionPosition() const
    {
        sf::Vector2f position = m_position;

        switch (m_shape)
        {
        case EmitterShape::Point:
            // Position fixe
            break;

        case EmitterShape::Line:
            // Position aléatoire sur une ligne
            position.x += Utils::MathUtils::randFloat(-m_size.x / 2.f, m_size.x / 2.f);
            break;

        case EmitterShape::Rectangle:
            // Position aléatoire dans un rectangle
            position.x += Utils::MathUtils::randFloat(-m_size.x / 2.f, m_size.x / 2.f);
            position.y += Utils::MathUtils::randFloat(-m_size.y / 2.f, m_size.y / 2.f);
            break;

        case EmitterShape::Circle:
            // Position aléatoire dans un cercle
            {
                float radius = Utils::MathUtils::randFloat(0.f, std::min(m_size.x, m_size.y) / 2.f);
                float angle = Utils::MathUtils::randFloat(0.f, 360.f);
                position += Utils::MathUtils::vectorFromAngle(angle, radius);
            }
            break;

        case EmitterShape::Ring:
            // Position aléatoire sur un anneau
            {
                float radius = std::min(m_size.x, m_size.y) / 2.f;
                float angle = Utils::MathUtils::randFloat(0.f, 360.f);
                position += Utils::MathUtils::vectorFromAngle(angle, radius);
            }
            break;
        }

        return position;
    }

} // namespace Orenji