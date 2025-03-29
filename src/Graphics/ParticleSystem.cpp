#include "Graphics/ParticleSystem.hpp"
#include <cmath>
#include <random>
#include <algorithm>

namespace Graphics
{
    ParticleSystem::ParticleSystem(unsigned int maxParticles)
        : m_vertices(sf::Quads), m_maxParticles(maxParticles), m_emitterPosition(0.f, 0.f), m_emissionRate(10.f), m_emissionTimer(0.f), m_minVelocity(-50.f, -50.f), m_maxVelocity(50.f, 50.f), m_minLifetime(1.f), m_maxLifetime(3.f), m_minSize(5.f), m_maxSize(10.f), m_minRotation(-180.f), m_maxRotation(180.f), m_startColor(sf::Color::White), m_endColor(sf::Color(255, 255, 255, 0)), m_enabled(true)
    {
        // Réserver de l'espace pour le nombre maximum de particules
        m_particles.reserve(m_maxParticles);
        m_vertices.resize(m_maxParticles * 4);

        // Définir un comportement par défaut (aucune modification)
        m_particleBehavior = [](Particle &particle, float deltaTime) {};
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::setTexture(std::shared_ptr<sf::Texture> texture)
    {
        m_texture = texture;
    }

    void ParticleSystem::setEmissionRate(float particlesPerSecond)
    {
        m_emissionRate = particlesPerSecond;
    }

    void ParticleSystem::setEmitterPosition(const sf::Vector2f &position)
    {
        m_emitterPosition = position;
    }

    void ParticleSystem::setParticleLifetime(float minLifetime, float maxLifetime)
    {
        m_minLifetime = minLifetime;
        m_maxLifetime = maxLifetime;
    }

    void ParticleSystem::setParticleVelocity(const sf::Vector2f &minVelocity, const sf::Vector2f &maxVelocity)
    {
        m_minVelocity = minVelocity;
        m_maxVelocity = maxVelocity;
    }

    void ParticleSystem::setParticleSize(float minSize, float maxSize)
    {
        m_minSize = minSize;
        m_maxSize = maxSize;
    }

    void ParticleSystem::setParticleColors(const sf::Color &startColor, const sf::Color &endColor)
    {
        m_startColor = startColor;
        m_endColor = endColor;
    }

    void ParticleSystem::setRotationSpeed(float minRotation, float maxRotation)
    {
        m_minRotation = minRotation;
        m_maxRotation = maxRotation;
    }

    void ParticleSystem::setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

    bool ParticleSystem::isEnabled() const
    {
        return m_enabled;
    }

    void ParticleSystem::clear()
    {
        m_particles.clear();
    }

    void ParticleSystem::emitBurst(unsigned int count)
    {
        for (unsigned int i = 0; i < count && m_particles.size() < m_maxParticles; ++i)
        {
            emitParticle();
        }
    }

    void ParticleSystem::update(float deltaTime)
    {
        // Émettre de nouvelles particules selon le taux d'émission
        if (m_enabled)
        {
            m_emissionTimer += deltaTime;

            float particlesThisFrame = m_emissionRate * deltaTime;
            int wholeParticles = static_cast<int>(particlesThisFrame);
            float fractionalPart = particlesThisFrame - wholeParticles;

            for (int i = 0; i < wholeParticles && m_particles.size() < m_maxParticles; ++i)
            {
                emitParticle();
            }

            // Gestion de la partie fractionnaire (émission probabiliste)
            if (randomFloat(0.f, 1.f) < fractionalPart && m_particles.size() < m_maxParticles)
            {
                emitParticle();
            }
        }

        // Mettre à jour toutes les particules
        for (auto it = m_particles.begin(); it != m_particles.end();)
        {
            Particle &particle = *it;

            // Réduire la durée de vie
            particle.lifetime -= deltaTime;

            if (particle.lifetime <= 0)
            {
                // Supprimer les particules mortes
                it = m_particles.erase(it);
            }
            else
            {
                // Mettre à jour la position
                particle.position += particle.velocity * deltaTime;

                // Appliquer le comportement personnalisé
                m_particleBehavior(particle, deltaTime);

                // Mise à jour de la rotation
                particle.rotation += particle.rotationSpeed * deltaTime;

                // Calculer le ratio de vie (0 = mort, 1 = nouveau)
                float lifeRatio = particle.lifetime / particle.initialLifetime;

                // Interpoler la couleur en fonction du ratio de vie
                particle.color.r = static_cast<sf::Uint8>(m_endColor.r + (m_startColor.r - m_endColor.r) * lifeRatio);
                particle.color.g = static_cast<sf::Uint8>(m_endColor.g + (m_startColor.g - m_endColor.g) * lifeRatio);
                particle.color.b = static_cast<sf::Uint8>(m_endColor.b + (m_startColor.b - m_endColor.b) * lifeRatio);
                particle.color.a = static_cast<sf::Uint8>(m_endColor.a + (m_startColor.a - m_endColor.a) * lifeRatio);

                ++it;
            }
        }

        // Mettre à jour le VertexArray
        updateVertices();
    }

    void ParticleSystem::setParticleBehavior(const std::function<void(Particle &, float)> &function)
    {
        m_particleBehavior = function;
    }

    void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Appliquer la texture
        if (m_texture)
        {
            states.texture = m_texture.get();
        }

        // Dessiner seulement s'il y a des particules
        if (!m_particles.empty())
        {
            target.draw(m_vertices, 0, m_particles.size() * 4, states);
        }
    }

    void ParticleSystem::emitParticle()
    {
        Particle particle;
        particle.position = m_emitterPosition;
        particle.velocity = randomVector(m_minVelocity, m_maxVelocity);
        particle.lifetime = randomFloat(m_minLifetime, m_maxLifetime);
        particle.initialLifetime = particle.lifetime;
        particle.size = randomFloat(m_minSize, m_maxSize);
        particle.color = m_startColor;
        particle.rotation = randomFloat(0.f, 360.f);
        particle.rotationSpeed = randomFloat(m_minRotation, m_maxRotation);

        m_particles.push_back(particle);
    }

    float ParticleSystem::randomFloat(float min, float max) const
    {
        static std::mt19937 engine(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(engine);
    }

    sf::Vector2f ParticleSystem::randomVector(const sf::Vector2f &min, const sf::Vector2f &max) const
    {
        return {randomFloat(min.x, max.x), randomFloat(min.y, max.y)};
    }

    sf::Color ParticleSystem::randomColor(const sf::Color &min, const sf::Color &max) const
    {
        return sf::Color(
            static_cast<sf::Uint8>(randomFloat(min.r, max.r)),
            static_cast<sf::Uint8>(randomFloat(min.g, max.g)),
            static_cast<sf::Uint8>(randomFloat(min.b, max.b)),
            static_cast<sf::Uint8>(randomFloat(min.a, max.a)));
    }

    void ParticleSystem::updateVertices()
    {
        for (size_t i = 0; i < m_particles.size(); ++i)
        {
            const Particle &particle = m_particles[i];

            // Calculer les coordonnées des quatre sommets du quad
            float halfSize = particle.size / 2.f;

            // Position de base des quatre coins (centrés sur l'origine)
            sf::Vector2f topLeft(-halfSize, -halfSize);
            sf::Vector2f topRight(halfSize, -halfSize);
            sf::Vector2f bottomRight(halfSize, halfSize);
            sf::Vector2f bottomLeft(-halfSize, halfSize);

            // Appliquer la rotation
            float angle = particle.rotation * 3.14159f / 180.f;
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);

            auto rotatePoint = [cosA, sinA](sf::Vector2f point) -> sf::Vector2f
            {
                return {
                    point.x * cosA - point.y * sinA,
                    point.x * sinA + point.y * cosA};
            };

            topLeft = rotatePoint(topLeft);
            topRight = rotatePoint(topRight);
            bottomRight = rotatePoint(bottomRight);
            bottomLeft = rotatePoint(bottomLeft);

            // Déplacer à la position de la particule
            topLeft += particle.position;
            topRight += particle.position;
            bottomRight += particle.position;
            bottomLeft += particle.position;

            // Mettre à jour les vertices
            size_t vertexIndex = i * 4;

            // Coordonnées de texture (quad complet)
            sf::Vector2f texCoords[4] = {
                {0.f, 0.f},
                {1.f, 0.f},
                {1.f, 1.f},
                {0.f, 1.f}};

            m_vertices[vertexIndex].position = topLeft;
            m_vertices[vertexIndex].color = particle.color;
            m_vertices[vertexIndex].texCoords = texCoords[0];

            m_vertices[vertexIndex + 1].position = topRight;
            m_vertices[vertexIndex + 1].color = particle.color;
            m_vertices[vertexIndex + 1].texCoords = texCoords[1];

            m_vertices[vertexIndex + 2].position = bottomRight;
            m_vertices[vertexIndex + 2].color = particle.color;
            m_vertices[vertexIndex + 2].texCoords = texCoords[2];

            m_vertices[vertexIndex + 3].position = bottomLeft;
            m_vertices[vertexIndex + 3].color = particle.color;
            m_vertices[vertexIndex + 3].texCoords = texCoords[3];
        }
    }

    // Effets prédéfinis
    void ParticleSystem::fireEffect(Particle &particle, float deltaTime)
    {
        // Effet de feu : monte + réduction de taille avec le temps
        particle.velocity.y -= 50.f * deltaTime; // Accélération vers le haut
        float lifeRatio = particle.lifetime / particle.initialLifetime;
        particle.size = particle.size * lifeRatio;
    }

    void ParticleSystem::smokeEffect(Particle &particle, float deltaTime)
    {
        // Effet de fumée : monte lentement + dérive latérale aléatoire
        particle.velocity.y -= 10.f * deltaTime;
        particle.velocity.x += (std::sin(particle.lifetime * 3.f) * 5.f) * deltaTime;
    }

    void ParticleSystem::sparkEffect(Particle &particle, float deltaTime)
    {
        // Effet d'étincelle : gravité + haute vitesse
        particle.velocity.y += 98.f * deltaTime; // Gravité
    }

    void ParticleSystem::explosionEffect(Particle &particle, float deltaTime)
    {
        // Effet d'explosion : ralentit avec le temps
        float lifeRatio = particle.lifetime / particle.initialLifetime;
        particle.velocity *= (0.95f * lifeRatio);
    }

    void ParticleSystem::rainEffect(Particle &particle, float deltaTime)
    {
        // Effet de pluie : chute rapide + petites déviations
        particle.velocity.y += 200.f * deltaTime;
        particle.velocity.x += (std::sin(particle.lifetime * 10.f) * 2.f) * deltaTime;
    }

    void ParticleSystem::snowEffect(Particle &particle, float deltaTime)
    {
        // Effet de neige : chute lente + oscillation latérale
        particle.velocity.y += 20.f * deltaTime;
        particle.velocity.x = 15.f * std::sin(particle.lifetime * 2.f);
        particle.rotation += deltaTime * 20.f; // Rotation lente
    }
} // namespace Graphics