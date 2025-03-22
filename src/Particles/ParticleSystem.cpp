#include "Particles/ParticleSystem.hpp"
#include <iostream>

namespace Orenji
{

    // Initialisation du pointeur singleton
    ParticleSystem *ParticleSystem::s_instance = nullptr;

    ParticleSystem::ParticleSystem()
        : m_texture(nullptr), m_needVertexUpdate(true)
    {
        // Initialise le tableau de vertices avec le type de primitive Quads
        // SFML 3 utilise une énumération scoped
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    }

    ParticleSystem::~ParticleSystem()
    {
        clear();
    }

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

    bool ParticleSystem::createEmitter(const std::string &name, unsigned int maxParticles)
    {
        if (hasEmitter(name))
        {
            std::cerr << "ParticleSystem::createEmitter: Un émetteur avec ce nom existe déjà: " << name << std::endl;
            return false;
        }

        m_emitters[name] = std::make_unique<ParticleEmitter>(maxParticles);
        m_needVertexUpdate = true;
        return true;
    }

    bool ParticleSystem::removeEmitter(const std::string &name)
    {
        if (!hasEmitter(name))
        {
            std::cerr << "ParticleSystem::removeEmitter: Émetteur non trouvé: " << name << std::endl;
            return false;
        }

        m_emitters.erase(name);
        m_needVertexUpdate = true;
        return true;
    }

    ParticleEmitter *ParticleSystem::getEmitter(const std::string &name)
    {
        if (!hasEmitter(name))
        {
            return nullptr;
        }
        return m_emitters[name].get();
    }

    bool ParticleSystem::hasEmitter(const std::string &name) const
    {
        return m_emitters.find(name) != m_emitters.end();
    }

    bool ParticleSystem::addAffector(const std::string &emitterName, const ParticleAffector &affector)
    {
        auto emitter = getEmitter(emitterName);
        if (!emitter)
        {
            std::cerr << "ParticleSystem::addAffector: Émetteur non trouvé: " << emitterName << std::endl;
            return false;
        }

        emitter->addAffector(affector);
        return true;
    }

    bool ParticleSystem::emit(const std::string &emitterName, unsigned int count)
    {
        auto emitter = getEmitter(emitterName);
        if (!emitter)
        {
            std::cerr << "ParticleSystem::emit: Émetteur non trouvé: " << emitterName << std::endl;
            return false;
        }

        emitter->emit(count);
        m_needVertexUpdate = true;
        return true;
    }

    void ParticleSystem::update(float deltaTime)
    {
        for (auto &pair : m_emitters)
        {
            pair.second->update(deltaTime);
        }
        m_needVertexUpdate = true;
    }

    void ParticleSystem::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (m_needVertexUpdate)
        {
            updateVertices();
        }

        if (m_vertices.getVertexCount() > 0)
        {
            states.texture = m_texture;
            target.draw(m_vertices, states);
        }
    }

    void ParticleSystem::clear()
    {
        m_emitters.clear();
        m_needVertexUpdate = true;
    }

    bool ParticleSystem::setEmitterPosition(const std::string &emitterName, const sf::Vector2f &position)
    {
        auto emitter = getEmitter(emitterName);
        if (!emitter)
        {
            std::cerr << "ParticleSystem::setEmitterPosition: Émetteur non trouvé: " << emitterName << std::endl;
            return false;
        }

        emitter->setPosition(position);
        return true;
    }

    void ParticleSystem::setTexture(sf::Texture *texture)
    {
        m_texture = texture;
    }

    unsigned int ParticleSystem::getActiveParticleCount() const
    {
        unsigned int count = 0;
        for (const auto &pair : m_emitters)
        {
            count += pair.second->getActiveParticleCount();
        }
        return count;
    }

    void ParticleSystem::updateVertices() const
    {
        // Calcule le nombre total de particules actives
        unsigned int totalActiveParticles = getActiveParticleCount();

        // Redimensionne le tableau de vertices (6 vertices par particule - 2 triangles)
        m_vertices.resize(totalActiveParticles * 6);

        // Si pas de particules actives, on s'arrête là
        if (totalActiveParticles == 0)
        {
            m_needVertexUpdate = false;
            return;
        }

        // Met à jour les vertices pour chaque particule
        unsigned int vertexIndex = 0;

        for (const auto &pair : m_emitters)
        {
            const auto &emitter = pair.second;
            const auto &particles = emitter->getParticles();

            for (const auto &particle : particles)
            {
                if (particle.active)
                {
                    // Calcul des coordonnées du quad
                    float halfSize = particle.size / 2.f;
                    float left = -halfSize;
                    float right = halfSize;
                    float top = -halfSize;
                    float bottom = halfSize;

                    // Rotation de la particule
                    sf::Transform transform;
                    transform.rotate(sf::degrees(particle.rotation));

                    // Coins du quad
                    sf::Vector2f topLeft = particle.position + transform.transformPoint(sf::Vector2f(left, top));
                    sf::Vector2f topRight = particle.position + transform.transformPoint(sf::Vector2f(right, top));
                    sf::Vector2f bottomLeft = particle.position + transform.transformPoint(sf::Vector2f(left, bottom));
                    sf::Vector2f bottomRight = particle.position + transform.transformPoint(sf::Vector2f(right, bottom));

                    // Coordonnées de texture (par défaut, utilise tout le sprite)
                    sf::Vector2f texTopLeft(0.f, 0.f);
                    sf::Vector2f texTopRight(1.f, 0.f);
                    sf::Vector2f texBottomLeft(0.f, 1.f);
                    sf::Vector2f texBottomRight(1.f, 1.f);

                    // Couleur de la particule
                    sf::Color color = particle.color;

                    // Mise à jour des vertices pour le premier triangle (Top-Left, Top-Right, Bottom-Right)
                    sf::Vertex vertex;

                    // Triangle 1
                    vertex.position = topLeft;
                    vertex.color = color;
                    vertex.texCoords = texTopLeft;
                    m_vertices[vertexIndex + 0] = vertex;

                    vertex.position = topRight;
                    vertex.color = color;
                    vertex.texCoords = texTopRight;
                    m_vertices[vertexIndex + 1] = vertex;

                    vertex.position = bottomRight;
                    vertex.color = color;
                    vertex.texCoords = texBottomRight;
                    m_vertices[vertexIndex + 2] = vertex;

                    // Triangle 2
                    vertex.position = topLeft;
                    vertex.color = color;
                    vertex.texCoords = texTopLeft;
                    m_vertices[vertexIndex + 3] = vertex;

                    vertex.position = bottomRight;
                    vertex.color = color;
                    vertex.texCoords = texBottomRight;
                    m_vertices[vertexIndex + 4] = vertex;

                    vertex.position = bottomLeft;
                    vertex.color = color;
                    vertex.texCoords = texBottomLeft;
                    m_vertices[vertexIndex + 5] = vertex;

                    vertexIndex += 6;
                }
            }
        }

        m_needVertexUpdate = false;
    }

} // namespace Orenji