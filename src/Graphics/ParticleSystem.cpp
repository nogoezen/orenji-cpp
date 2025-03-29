#include "Graphics/ParticleSystem.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint> // Pour std::uint8_t

namespace Orenji
{
    namespace Graphics
    {

        ParticleSystem::ParticleSystem(unsigned int maxParticles)
            : m_vertices(sf::PrimitiveType::Triangles),
              m_maxParticles(maxParticles),
              m_emitterPosition(0.f, 0.f),
              m_emitterAreaTopLeft(0.f, 0.f),
              m_emitterAreaSize(0.f, 0.f),
              m_emitterRadius(0.f),
              m_useCircularEmitter(false),
              m_emissionRate(10.f),
              m_emissionAccumulator(0.f),
              m_globalForce(0.f, 0.f),
              m_acceleration(0.f, 0.f),
              m_drag(0.f),
              m_minVelocity(-50.f, -50.f),
              m_maxVelocity(50.f, 50.f),
              m_minLifetime(1.f),
              m_maxLifetime(3.f),
              m_minSize(5.f),
              m_maxSize(10.f),
              m_minEndSize(0.f),
              m_maxEndSize(0.f),
              m_minRotation(-180.f),
              m_maxRotation(180.f),
              m_startColor(sf::Color::White),
              m_endColor(sf::Color(255, 255, 255, 0)),
              m_blendMode(sf::BlendAlpha),
              m_emitterEnabled(true),
              m_activeParticleCount(0)
        {
            // Seed the random number generator
            std::random_device rd;
            m_randomEngine = std::mt19937(rd());
            m_uniformDist = std::uniform_real_distribution<float>(0.f, 1.f);

            // Réserver de l'espace pour le nombre maximum de particules
            m_particles.resize(m_maxParticles);
            m_vertices.resize(m_maxParticles * 4);

            // Initialiser toutes les particules comme inactives
            for (auto &particle : m_particles)
            {
                particle.active = false;
            }

            // Définir un comportement par défaut (aucune modification)
            m_particleBehavior = [](Particle &particle, float deltaTime) {};
        }

        ParticleSystem::~ParticleSystem()
        {
        }

        void ParticleSystem::setTexture(const std::string &texturePath)
        {
            m_texture = std::make_shared<sf::Texture>();
            if (!m_texture->loadFromFile(texturePath))
            {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
                m_texture.reset();
            }
        }

        void ParticleSystem::setEmissionRate(float particlesPerSecond)
        {
            m_emissionRate = particlesPerSecond;
        }

        void ParticleSystem::setEmitterPosition(const sf::Vector2f &position)
        {
            m_emitterPosition = position;
            m_useCircularEmitter = false;
            m_emitterAreaSize = sf::Vector2f(0.f, 0.f);
        }

        void ParticleSystem::setRectangularEmitter(const sf::Vector2f &topLeft, const sf::Vector2f &size)
        {
            m_emitterAreaTopLeft = topLeft;
            m_emitterAreaSize = size;
            m_useCircularEmitter = false;
        }

        void ParticleSystem::setEmitterRadius(float radius)
        {
            m_emitterRadius = radius;
            m_useCircularEmitter = true;
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

        void ParticleSystem::setAcceleration(const sf::Vector2f &acceleration)
        {
            m_acceleration = acceleration;
        }

        void ParticleSystem::setParticleSize(float minSize, float maxSize)
        {
            m_minSize = minSize;
            m_maxSize = maxSize;

            // Par défaut, la taille finale est identique à la taille initiale
            if (m_minEndSize == 0.f && m_maxEndSize == 0.f)
            {
                m_minEndSize = minSize;
                m_maxEndSize = maxSize;
            }
        }

        void ParticleSystem::setParticleEndSize(float minEndSize, float maxEndSize)
        {
            m_minEndSize = minEndSize;
            m_maxEndSize = maxEndSize;
        }

        void ParticleSystem::setParticleColors(const sf::Color &startColor, const sf::Color &endColor)
        {
            m_startColor = startColor;
            m_endColor = endColor;
        }

        void ParticleSystem::setParticleRotationSpeed(float minRotation, float maxRotation)
        {
            m_minRotation = minRotation;
            m_maxRotation = maxRotation;
        }

        void ParticleSystem::setDrag(float drag)
        {
            m_drag = std::clamp(drag, 0.f, 1.f);
        }

        void ParticleSystem::applyForce(const sf::Vector2f &force)
        {
            m_globalForce = force;
        }

        void ParticleSystem::applyRadialForce(const sf::Vector2f &center, float strength, float radius)
        {
            // Cette force sera appliquée individuellement aux particules dans la méthode update
            for (auto &particle : m_particles)
            {
                if (!particle.active)
                    continue;

                sf::Vector2f direction = particle.position - center;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                // Si la distance est dans le rayon d'effet ou si le rayon est 0 (infini)
                if (radius <= 0.f || distance <= radius)
                {
                    if (distance > 0.f)
                    {
                        // Normaliser la direction
                        direction /= distance;

                        // Calculer la force en fonction de la distance (diminue avec la distance)
                        float force = strength;
                        if (radius > 0.f)
                        {
                            force *= (1.f - distance / radius);
                        }

                        // Appliquer la force comme une accélération
                        particle.acceleration += direction * force;
                    }
                }
            }
        }

        void ParticleSystem::enableEmitter(bool enable)
        {
            m_emitterEnabled = enable;
        }

        bool ParticleSystem::isEmitterEnabled() const
        {
            return m_emitterEnabled;
        }

        size_t ParticleSystem::getParticleCount() const
        {
            return m_activeParticleCount;
        }

        size_t ParticleSystem::getMaxParticleCount() const
        {
            return m_maxParticles;
        }

        void ParticleSystem::clear()
        {
            for (auto &particle : m_particles)
            {
                particle.active = false;
            }
            m_activeParticleCount = 0;
        }

        void ParticleSystem::emit(unsigned int count)
        {
            for (unsigned int i = 0; i < count; ++i)
            {
                emitParticle();
            }
        }

        void ParticleSystem::setEffect(ParticleEffect effect)
        {
            // Configurer les paramètres en fonction de l'effet choisi
            switch (effect)
            {
            case ParticleEffect::FIRE:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(0.5f, 1.5f);
                setParticleVelocity(sf::Vector2f(-10.f, -80.f), sf::Vector2f(10.f, -50.f));
                setParticleSize(10.f, 30.f);
                setParticleEndSize(5.f, 15.f);
                setParticleColors(sf::Color(255, 160, 20, 200), sf::Color(130, 60, 0, 0));
                setEmissionRate(100.f);
                setDrag(0.02f);
                setAcceleration(sf::Vector2f(0.f, -20.f));
                setParticleBehavior(fireEffect);
                break;

            case ParticleEffect::SMOKE:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(1.5f, 3.f);
                setParticleVelocity(sf::Vector2f(-5.f, -40.f), sf::Vector2f(5.f, -20.f));
                setParticleSize(20.f, 40.f);
                setParticleEndSize(40.f, 80.f);
                setParticleColors(sf::Color(50, 50, 50, 150), sf::Color(150, 150, 150, 0));
                setEmissionRate(20.f);
                setDrag(0.05f);
                setAcceleration(sf::Vector2f(0.f, -5.f));
                setParticleBehavior(smokeEffect);
                break;

            case ParticleEffect::SPARK:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(0.3f, 0.8f);
                setParticleVelocity(sf::Vector2f(-150.f, -150.f), sf::Vector2f(150.f, 150.f));
                setParticleSize(2.f, 4.f);
                setParticleEndSize(0.5f, 1.f);
                setParticleColors(sf::Color(255, 230, 100, 255), sf::Color(255, 160, 20, 0));
                setEmissionRate(200.f);
                setDrag(0.01f);
                setAcceleration(sf::Vector2f(0.f, 100.f)); // Légère gravité
                setParticleBehavior(sparkEffect);
                break;

            case ParticleEffect::EXPLOSION:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(0.5f, 1.f);
                setParticleVelocity(sf::Vector2f(-300.f, -300.f), sf::Vector2f(300.f, 300.f));
                setParticleSize(10.f, 20.f);
                setParticleEndSize(1.f, 5.f);
                setParticleColors(sf::Color(255, 100, 20, 255), sf::Color(150, 20, 0, 0));
                setEmissionRate(0.f); // Pas d'émission continue
                setDrag(0.1f);
                setAcceleration(sf::Vector2f(0.f, 0.f));
                setParticleBehavior(explosionEffect);
                // Émettre un groupe de particules immédiatement
                emit(100);
                break;

            case ParticleEffect::RAIN:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(0.8f, 1.2f);
                setParticleVelocity(sf::Vector2f(-20.f, 200.f), sf::Vector2f(20.f, 300.f));
                setParticleSize(1.f, 3.f);
                setParticleEndSize(1.f, 3.f);
                setParticleColors(sf::Color(100, 100, 240, 150), sf::Color(200, 200, 255, 100));
                setEmissionRate(200.f);
                setDrag(0.001f);
                setAcceleration(sf::Vector2f(0.f, 200.f));
                setParticleBehavior(rainEffect);
                break;

            case ParticleEffect::SNOW:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(3.f, 6.f);
                setParticleVelocity(sf::Vector2f(-15.f, 15.f), sf::Vector2f(15.f, 30.f));
                setParticleSize(2.f, 5.f);
                setParticleEndSize(2.f, 5.f);
                setParticleColors(sf::Color(250, 250, 255, 200), sf::Color(250, 250, 255, 50));
                setParticleRotationSpeed(10.f, 30.f);
                setEmissionRate(50.f);
                setDrag(0.05f);
                setAcceleration(sf::Vector2f(0.f, 5.f));
                setParticleBehavior(snowEffect);
                break;

            case ParticleEffect::DUST:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(1.f, 3.f);
                setParticleVelocity(sf::Vector2f(-20.f, -5.f), sf::Vector2f(20.f, 5.f));
                setParticleSize(2.f, 8.f);
                setParticleEndSize(6.f, 12.f);
                setParticleColors(sf::Color(180, 160, 120, 100), sf::Color(180, 160, 120, 0));
                setParticleRotationSpeed(-10.f, 10.f);
                setEmissionRate(10.f);
                setDrag(0.1f);
                setAcceleration(sf::Vector2f(0.f, -1.f));
                setParticleBehavior(dustEffect);
                break;

            case ParticleEffect::WATERFALL:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(1.f, 2.f);
                setParticleVelocity(sf::Vector2f(-10.f, 50.f), sf::Vector2f(10.f, 150.f));
                setParticleSize(3.f, 8.f);
                setParticleEndSize(1.f, 3.f);
                setParticleColors(sf::Color(100, 150, 255, 150), sf::Color(200, 230, 255, 0));
                setEmissionRate(100.f);
                setDrag(0.02f);
                setAcceleration(sf::Vector2f(0.f, 100.f));
                setParticleBehavior(waterfallEffect);
                break;

            case ParticleEffect::MAGIC:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(0.5f, 1.5f);
                setParticleVelocity(sf::Vector2f(-20.f, -20.f), sf::Vector2f(20.f, 20.f));
                setParticleSize(5.f, 10.f);
                setParticleEndSize(1.f, 3.f);
                setParticleColors(sf::Color(100, 20, 255, 200), sf::Color(200, 150, 255, 0));
                setParticleRotationSpeed(-180.f, 180.f);
                setEmissionRate(30.f);
                setDrag(0.05f);
                setAcceleration(sf::Vector2f(0.f, -10.f));
                setParticleBehavior(magicEffect);
                break;

            case ParticleEffect::LEAF:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(3.f, 8.f);
                setParticleVelocity(sf::Vector2f(-20.f, 5.f), sf::Vector2f(20.f, 20.f));
                setParticleSize(5.f, 15.f);
                setParticleEndSize(5.f, 15.f);
                setParticleColors(sf::Color(50, 150, 30, 200), sf::Color(150, 180, 30, 100));
                setParticleRotationSpeed(-90.f, 90.f);
                setEmissionRate(5.f);
                setDrag(0.01f);
                setAcceleration(sf::Vector2f(0.f, 5.f));
                setParticleBehavior(leafEffect);
                break;

            case ParticleEffect::BUBBLE:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(2.f, 6.f);
                setParticleVelocity(sf::Vector2f(-10.f, -30.f), sf::Vector2f(10.f, -10.f));
                setParticleSize(3.f, 12.f);
                setParticleEndSize(3.f, 12.f);
                setParticleColors(sf::Color(200, 250, 255, 100), sf::Color(200, 250, 255, 0));
                setParticleRotationSpeed(0.f, 0.f);
                setEmissionRate(10.f);
                setDrag(0.05f);
                setAcceleration(sf::Vector2f(0.f, -5.f));
                setParticleBehavior(bubbleEffect);
                break;

            case ParticleEffect::NONE:
            default:
                m_blendMode = sf::BlendAlpha;
                setParticleLifetime(1.f, 3.f);
                setParticleVelocity(sf::Vector2f(-50.f, -50.f), sf::Vector2f(50.f, 50.f));
                setParticleSize(5.f, 10.f);
                setParticleEndSize(5.f, 10.f);
                setParticleColors(sf::Color::White, sf::Color(255, 255, 255, 0));
                setParticleRotationSpeed(0.f, 0.f);
                setEmissionRate(10.f);
                setDrag(0.f);
                setAcceleration(sf::Vector2f(0.f, 0.f));
                setParticleBehavior([](Particle &particle, float deltaTime) {});
                break;
            }
        }

        bool ParticleSystem::loadFromFile(const std::string &filepath)
        {
            std::ifstream file(filepath);
            if (!file.is_open())
            {
                std::cerr << "Failed to open particle effect file: " << filepath << std::endl;
                return false;
            }

            std::string line;
            std::string paramName;

            while (std::getline(file, line))
            {
                // Ignorer les lignes vides et les commentaires
                if (line.empty() || line[0] == '#')
                    continue;

                std::istringstream iss(line);
                iss >> paramName;

                if (paramName == "Effect")
                {
                    std::string effectName;
                    iss >> effectName;

                    if (effectName == "Fire")
                        setEffect(ParticleEffect::FIRE);
                    else if (effectName == "Smoke")
                        setEffect(ParticleEffect::SMOKE);
                    else if (effectName == "Explosion")
                        setEffect(ParticleEffect::EXPLOSION);
                    else if (effectName == "Spark")
                        setEffect(ParticleEffect::SPARK);
                    else if (effectName == "Rain")
                        setEffect(ParticleEffect::RAIN);
                    else if (effectName == "Snow")
                        setEffect(ParticleEffect::SNOW);
                    else if (effectName == "Dust")
                        setEffect(ParticleEffect::DUST);
                    else if (effectName == "Waterfall")
                        setEffect(ParticleEffect::WATERFALL);
                    else if (effectName == "Magic")
                        setEffect(ParticleEffect::MAGIC);
                    else if (effectName == "Leaf")
                        setEffect(ParticleEffect::LEAF);
                    else if (effectName == "Bubble")
                        setEffect(ParticleEffect::BUBBLE);
                }
                else if (paramName == "EmissionRate")
                {
                    float rate;
                    iss >> rate;
                    setEmissionRate(rate);
                }
                else if (paramName == "ParticleLifetime")
                {
                    float minLife, maxLife;
                    iss >> minLife >> maxLife;
                    setParticleLifetime(minLife, maxLife);
                }
                else if (paramName == "ParticleVelocity")
                {
                    float minX, minY, maxX, maxY;
                    iss >> minX >> minY >> maxX >> maxY;
                    setParticleVelocity(sf::Vector2f(minX, minY), sf::Vector2f(maxX, maxY));
                }
                else if (paramName == "ParticleSize")
                {
                    float minSize, maxSize;
                    iss >> minSize >> maxSize;
                    setParticleSize(minSize, maxSize);
                }
                else if (paramName == "ParticleEndSize")
                {
                    float minEndSize, maxEndSize;
                    iss >> minEndSize >> maxEndSize;
                    setParticleEndSize(minEndSize, maxEndSize);
                }
                else if (paramName == "ParticleColors")
                {
                    int r1, g1, b1, a1, r2, g2, b2, a2;
                    iss >> r1 >> g1 >> b1 >> a1 >> r2 >> g2 >> b2 >> a2;
                    setParticleColors(
                        sf::Color(r1, g1, b1, a1),
                        sf::Color(r2, g2, b2, a2));
                }
                else if (paramName == "RotationSpeed")
                {
                    float minRot, maxRot;
                    iss >> minRot >> maxRot;
                    setParticleRotationSpeed(minRot, maxRot);
                }
                else if (paramName == "Drag")
                {
                    float drag;
                    iss >> drag;
                    setDrag(drag);
                }
                else if (paramName == "Acceleration")
                {
                    float accX, accY;
                    iss >> accX >> accY;
                    setAcceleration(sf::Vector2f(accX, accY));
                }
                else if (paramName == "BlendMode")
                {
                    std::string mode;
                    iss >> mode;

                    if (mode == "Alpha")
                        setBlendMode(sf::BlendAlpha);
                    else if (mode == "Add")
                        setBlendMode(sf::BlendAdd);
                    else if (mode == "Multiply")
                        setBlendMode(sf::BlendMultiply);
                }
                else if (paramName == "CircularEmitter")
                {
                    bool useCircular;
                    iss >> std::boolalpha >> useCircular;
                    setCircularEmitter(useCircular);
                }
                else if (paramName == "EmitterRadius")
                {
                    float radius;
                    iss >> radius;
                    setEmitterRadius(radius);
                }
                else if (paramName == "RectangularEmitter")
                {
                    float x, y, width, height;
                    iss >> x >> y >> width >> height;
                    setRectangularEmitter(sf::Vector2f(x, y), sf::Vector2f(width, height));
                }
            }

            file.close();
            return true;
        }

        bool ParticleSystem::saveToFile(const std::string &filename)
        {
            try
            {
                std::ofstream file(filename);
                if (!file.is_open())
                {
                    std::cerr << "Failed to open file for writing: " << filename << std::endl;
                    return false;
                }

                file << "# Orenji Particle System Effect Configuration\n\n";

                file << "EmissionRate " << m_emissionRate << "\n";
                file << "ParticleLifetime " << m_minLifetime << " " << m_maxLifetime << "\n";
                file << "ParticleVelocity " << m_minVelocity.x << " " << m_minVelocity.y << " "
                     << m_maxVelocity.x << " " << m_maxVelocity.y << "\n";
                file << "ParticleSize " << m_minSize << " " << m_maxSize << "\n";
                file << "ParticleEndSize " << m_minEndSize << " " << m_maxEndSize << "\n";
                file << "ParticleColors " << static_cast<int>(m_startColor.r) << " " << static_cast<int>(m_startColor.g) << " "
                     << static_cast<int>(m_startColor.b) << " " << static_cast<int>(m_startColor.a) << " "
                     << static_cast<int>(m_endColor.r) << " " << static_cast<int>(m_endColor.g) << " "
                     << static_cast<int>(m_endColor.b) << " " << static_cast<int>(m_endColor.a) << "\n";
                file << "RotationSpeed " << m_minRotation << " " << m_maxRotation << "\n";
                file << "Drag " << m_drag << "\n";
                file << "Acceleration " << m_acceleration.x << " " << m_acceleration.y << "\n";

                // Écrire le mode de fusion
                file << "BlendMode ";
                if (m_blendMode == sf::BlendAlpha)
                {
                    file << "Alpha\n";
                }
                else if (m_blendMode == sf::BlendAdd)
                {
                    file << "Add\n";
                }
                else if (m_blendMode == sf::BlendMultiply)
                {
                    file << "Multiply\n";
                }
                else
                {
                    file << "Custom\n";
                }

                file.close();
                return true;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error saving particle effect to file: " << e.what() << std::endl;
                return false;
            }
        }

        void ParticleSystem::update(float deltaTime)
        {
            // Émettre de nouvelles particules selon le taux d'émission
            if (m_emitterEnabled)
            {
                m_emissionAccumulator += deltaTime;

                float particlesThisFrame = m_emissionRate * deltaTime;
                int wholeParticles = static_cast<int>(particlesThisFrame);
                float fractionalPart = particlesThisFrame - wholeParticles;

                for (int i = 0; i < wholeParticles; ++i)
                {
                    emitParticle();
                }

                // Gestion de la partie fractionnaire (émission probabiliste)
                if (randomFloat(0.f, 1.f) < fractionalPart)
                {
                    emitParticle();
                }
            }

            // Mettre à jour toutes les particules
            size_t activeCount = 0;
            for (auto &particle : m_particles)
            {
                if (!particle.active)
                    continue;

                // Stocker la position actuelle
                particle.prevPosition = particle.position;

                // Diminuer la durée de vie
                particle.lifetime -= deltaTime;

                // Si la particule est morte, la marquer comme inactive
                if (particle.lifetime <= 0.f)
                {
                    particle.active = false;
                    continue;
                }

                // Calculer le ratio de vie (0 = mort, 1 = nouvellement créée)
                float lifeRatio = particle.lifetime / particle.initialLifetime;

                // Appliquer le comportement personnalisé
                m_particleBehavior(particle, deltaTime);

                // Appliquer les forces et l'accélération
                particle.velocity += (m_globalForce + particle.acceleration) * deltaTime;

                // Appliquer la trainée (résistance à l'air)
                if (m_drag > 0.f)
                {
                    particle.velocity *= (1.f - m_drag * deltaTime);
                }

                // Mettre à jour la position
                particle.position += particle.velocity * deltaTime;

                // Mettre à jour la rotation
                particle.rotation += particle.rotationSpeed * deltaTime;

                // Interpoler la taille
                if (particle.initialSize != particle.endSize)
                {
                    particle.size = particle.endSize + (particle.initialSize - particle.endSize) * lifeRatio;
                }

                // Interpoler la couleur
                if (particle.startColor != particle.endColor)
                {
                    float r = particle.endColor.r + (particle.startColor.r - particle.endColor.r) * lifeRatio;
                    float g = particle.endColor.g + (particle.startColor.g - particle.endColor.g) * lifeRatio;
                    float b = particle.endColor.b + (particle.startColor.b - particle.endColor.b) * lifeRatio;
                    float a = particle.endColor.a + (particle.startColor.a - particle.endColor.a) * lifeRatio;

                    particle.color = sf::Color(static_cast<std::uint8_t>(r),
                                               static_cast<std::uint8_t>(g),
                                               static_cast<std::uint8_t>(b),
                                               static_cast<std::uint8_t>(a));
                }

                activeCount++;
            }

            m_activeParticleCount = activeCount;

            // Mettre à jour le tableau de vertices pour l'affichage
            updateVertices();
        }

        void ParticleSystem::setParticleBehavior(ParticleBehavior behavior)
        {
            m_particleBehavior = behavior;
        }

        void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            // Appliquer la texture
            states.texture = m_texture.get();

            // Appliquer le mode de fusion (blending mode)
            states.blendMode = m_blendMode;

            // Dessiner seulement les particules actives
            if (m_activeParticleCount > 0)
            {
                // SFML 3 utilise désormais le vertex array complet
                target.draw(m_vertices, states);
            }
        }

        Particle &ParticleSystem::emitParticle()
        {
            // Trouver une particule inactive ou la plus ancienne
            int index = findInactiveParticle();
            if (index < 0)
            {
                // Si toutes les particules sont actives, on en réutilise une (la plus ancienne)
                float oldestLifeRatio = 1.0f;
                index = 0;

                for (size_t i = 0; i < m_particles.size(); ++i)
                {
                    float lifeRatio = m_particles[i].lifetime / m_particles[i].initialLifetime;
                    if (lifeRatio < oldestLifeRatio)
                    {
                        oldestLifeRatio = lifeRatio;
                        index = static_cast<int>(i);
                    }
                }
            }

            // Configurer la nouvelle particule
            Particle &particle = m_particles[index];

            // Position initiale
            particle.position = getRandomEmissionPosition();
            particle.prevPosition = particle.position;

            // Vélocité initiale
            particle.velocity = randomVector(m_minVelocity, m_maxVelocity);

            // Accélération initiale
            particle.acceleration = m_acceleration;

            // Durée de vie
            particle.lifetime = randomFloat(m_minLifetime, m_maxLifetime);
            particle.initialLifetime = particle.lifetime;

            // Taille
            particle.size = randomFloat(m_minSize, m_maxSize);
            particle.initialSize = particle.size;
            particle.endSize = randomFloat(m_minEndSize, m_maxEndSize);

            // Rotation
            particle.rotation = randomFloat(0.f, 360.f);
            particle.rotationSpeed = randomFloat(m_minRotation, m_maxRotation);

            // Couleur
            particle.startColor = randomColor(m_startColor, m_startColor);
            particle.endColor = randomColor(m_endColor, m_endColor);
            particle.color = particle.startColor;

            // Traînée
            particle.drag = m_drag;

            // Transparence
            particle.alpha = 1.0f;

            // Activer la particule
            particle.active = true;
            m_activeParticleCount++;

            return particle;
        }

        int ParticleSystem::findInactiveParticle() const
        {
            for (size_t i = 0; i < m_particles.size(); ++i)
            {
                if (!m_particles[i].active)
                {
                    return static_cast<int>(i);
                }
            }
            return -1; // Aucune particule inactive trouvée
        }

        void ParticleSystem::updateVertices()
        {
            for (size_t i = 0; i < m_particles.size(); ++i)
            {
                const Particle &particle = m_particles[i];

                if (!particle.active)
                    continue;

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

        void ParticleSystem::applyBlendMode(sf::RenderStates &states) const
        {
            states.blendMode = m_blendMode;
        }

        sf::Vector2f ParticleSystem::getRandomEmissionPosition() const
        {
            if (m_useCircularEmitter && m_emitterRadius > 0.f)
            {
                // Générer une position dans un cercle
                float angle = randomFloat(0.f, 2.f * 3.14159f);
                float distance = randomFloat(0.f, m_emitterRadius);
                return m_emitterPosition + sf::Vector2f(std::cos(angle) * distance, std::sin(angle) * distance);
            }
            else if (m_emitterAreaSize.x > 0.f && m_emitterAreaSize.y > 0.f)
            {
                // Générer une position dans un rectangle
                return m_emitterAreaTopLeft + sf::Vector2f(
                                                  randomFloat(0.f, m_emitterAreaSize.x),
                                                  randomFloat(0.f, m_emitterAreaSize.y));
            }
            else
            {
                // Position unique (émetteur ponctuel)
                return m_emitterPosition;
            }
        }

        float ParticleSystem::randomFloat(float min, float max) const
        {
            return min + m_uniformDist(m_randomEngine) * (max - min);
        }

        sf::Vector2f ParticleSystem::randomVector(const sf::Vector2f &min, const sf::Vector2f &max) const
        {
            return sf::Vector2f(
                randomFloat(min.x, max.x),
                randomFloat(min.y, max.y));
        }

        sf::Color ParticleSystem::randomColor(const sf::Color &min, const sf::Color &max) const
        {
            return sf::Color(
                static_cast<std::uint8_t>(randomFloat(min.r, max.r)),
                static_cast<std::uint8_t>(randomFloat(min.g, max.g)),
                static_cast<std::uint8_t>(randomFloat(min.b, max.b)),
                static_cast<std::uint8_t>(randomFloat(min.a, max.a)));
        }

        // Effets prédéfinis
        void ParticleSystem::fireEffect(Particle &particle, float deltaTime)
        {
            // Effet de feu : monte + réduction de taille avec le temps
            particle.velocity.y -= 50.f * deltaTime; // Accélération vers le haut

            // Fluctuation latérale pour donner un effet flamme
            particle.velocity.x += (std::sin(particle.lifetime * 5.f) * 10.f) * deltaTime;

            // Faire tourner légèrement la particule pour plus de naturel
            particle.rotation += deltaTime * 10.f;
        }

        void ParticleSystem::smokeEffect(Particle &particle, float deltaTime)
        {
            // Effet de fumée : monte lentement + dérive latérale aléatoire
            particle.velocity.y -= 10.f * deltaTime;
            particle.velocity.x += (std::sin(particle.lifetime * 3.f) * 5.f) * deltaTime;

            // Ralentir progressivement
            particle.velocity *= (0.99f - 0.05f * deltaTime);

            // Rotation lente
            particle.rotation += deltaTime * 5.f;
        }

        void ParticleSystem::sparkEffect(Particle &particle, float deltaTime)
        {
            // Effet d'étincelle : gravité + haute vitesse
            particle.velocity.y += 98.f * deltaTime; // Gravité

            // Dégradation de taille plus rapide vers la fin de vie
            float lifeRatio = particle.lifetime / particle.initialLifetime;
            if (lifeRatio < 0.3f)
            {
                particle.size *= (0.95f - 0.1f * deltaTime);
            }
        }

        void ParticleSystem::explosionEffect(Particle &particle, float deltaTime)
        {
            // Effet d'explosion : ralentit avec le temps
            float lifeRatio = particle.lifetime / particle.initialLifetime;
            particle.velocity *= (0.95f - 0.2f * deltaTime);

            // Ondulation de luminosité
            particle.color.r = static_cast<std::uint8_t>(std::min(255.f, particle.color.r * (1.f + 0.1f * std::sin(particle.lifetime * 20.f))));
        }

        void ParticleSystem::rainEffect(Particle &particle, float deltaTime)
        {
            // Effet de pluie : chute rapide + petites déviations
            particle.velocity.y += 200.f * deltaTime;
            particle.velocity.x += (std::sin(particle.lifetime * 10.f) * 2.f) * deltaTime;

            // Léger étirement vertical
            if (particle.size > 1.f)
            {
                particle.size *= 0.98f;
            }
        }

        void ParticleSystem::snowEffect(Particle &particle, float deltaTime)
        {
            // Effet de neige : chute lente + oscillation latérale
            particle.velocity.y += 20.f * deltaTime;
            particle.velocity.x = 15.f * std::sin(particle.lifetime * 2.f);
            particle.rotation += deltaTime * 20.f; // Rotation lente
        }

        void ParticleSystem::dustEffect(Particle &particle, float deltaTime)
        {
            // Effet de poussière : mouvement aléatoire lent
            particle.velocity.x += (std::sin(particle.lifetime * 4.f) * 3.f) * deltaTime;
            particle.velocity.y += (std::cos(particle.lifetime * 3.f) * 2.f) * deltaTime;

            // Ralentissement progressif
            particle.velocity *= (0.98f - 0.1f * deltaTime);

            // Rotation très lente
            particle.rotation += deltaTime * 2.f;
        }

        void ParticleSystem::waterfallEffect(Particle &particle, float deltaTime)
        {
            // Effet de cascade : chute rapide + éclaboussures en bas
            float lifeRatio = particle.lifetime / particle.initialLifetime;

            if (lifeRatio < 0.3f)
            {
                // À la fin de sa vie (proche du sol), ralentir et se déplacer latéralement
                particle.velocity.y *= 0.9f;
                particle.velocity.x = 30.f * std::sin(particle.lifetime * 10.f);
            }

            // Légère variation de couleur (plus claire/transparente vers la fin)
            if (lifeRatio < 0.5f)
            {
                particle.color.a = static_cast<std::uint8_t>(particle.color.a * 0.95f);
            }
        }

        void ParticleSystem::magicEffect(Particle &particle, float deltaTime)
        {
            // Effet magique : mouvement orbital + pulse de taille
            float angle = particle.lifetime * 3.f;
            float radius = 20.f * (1.f - particle.lifetime / particle.initialLifetime);

            // Mouvement orbital autour de la position d'origine
            particle.velocity.x = std::cos(angle) * radius - particle.position.x;
            particle.velocity.y = std::sin(angle) * radius - particle.position.y;

            // Pulse de taille
            particle.size = particle.initialSize * (0.8f + 0.4f * std::sin(particle.lifetime * 10.f));

            // Rotation rapide
            particle.rotation += deltaTime * 180.f;
        }

        void ParticleSystem::leafEffect(Particle &particle, float deltaTime)
        {
            // Effet de feuille : mouvement oscillant + rotation
            particle.velocity.x = 15.f * std::sin(particle.lifetime * 2.f);
            particle.velocity.y = 5.f + 5.f * std::cos(particle.lifetime * 1.5f);

            // Rotation variable
            particle.rotation += deltaTime * 30.f * std::sin(particle.lifetime);

            // Léger changement de taille
            particle.size = particle.initialSize * (0.9f + 0.1f * std::sin(particle.lifetime * 5.f));
        }

        void ParticleSystem::bubbleEffect(Particle &particle, float deltaTime)
        {
            // Effet de bulle : mouvement zigzag ascendant
            particle.velocity.x = 10.f * std::sin(particle.lifetime * 3.f);

            // Léger ralentissement de la montée
            particle.velocity.y *= 0.99f;

            // Pulse de taille pour simuler l'instabilité des bulles
            particle.size = particle.initialSize * (0.9f + 0.1f * std::sin(particle.lifetime * 8.f));

            // Légère variation de l'opacité
            particle.color.a = static_cast<std::uint8_t>(particle.color.a * (0.95f + 0.05f * std::sin(particle.lifetime * 10.f)));
        }

        void ParticleSystem::setBlendMode(sf::BlendMode mode)
        {
            m_blendMode = mode;
        }

        void ParticleSystem::setCircularEmitter(bool circular)
        {
            m_useCircularEmitter = circular;
        }
    } // namespace Graphics
} // namespace Orenji