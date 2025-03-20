#include "../../include/Physics/PhysicsWorld.hpp"
#include <iostream>

namespace Orenji
{

    std::unique_ptr<PhysicsWorld> PhysicsWorld::s_instance = nullptr;

    // Implémentation de ContactListener
    void ContactListener::BeginContact(b2Contact *contact)
    {
        if (m_beginContactCallback)
        {
            m_beginContactCallback(contact);
        }
    }

    void ContactListener::EndContact(b2Contact *contact)
    {
        if (m_endContactCallback)
        {
            m_endContactCallback(contact);
        }
    }

    void ContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
    {
        if (m_preSolveCallback)
        {
            m_preSolveCallback(contact, oldManifold);
        }
    }

    void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
    {
        if (m_postSolveCallback)
        {
            m_postSolveCallback(contact, impulse);
        }
    }

    void ContactListener::SetBeginContactCallback(BeginContactCallback callback)
    {
        m_beginContactCallback = callback;
    }

    void ContactListener::SetEndContactCallback(EndContactCallback callback)
    {
        m_endContactCallback = callback;
    }

    void ContactListener::SetPreSolveCallback(PreSolveCallback callback)
    {
        m_preSolveCallback = callback;
    }

    void ContactListener::SetPostSolveCallback(PostSolveCallback callback)
    {
        m_postSolveCallback = callback;
    }

    // Implémentation de PhysicsWorld
    PhysicsWorld::PhysicsWorld(const sf::Vector2f &gravity)
        : m_debugDrawEnabled(false), m_debugLines(sf::Lines)
    {

        // Créer le monde Box2D avec la gravité définie
        m_world = std::make_unique<b2World>(b2Vec2(gravity.x, gravity.y));

        // Définir le gestionnaire de contacts
        m_world->SetContactListener(&m_contactListener);
    }

    PhysicsWorld::~PhysicsWorld()
    {
        // Box2D nettoie automatiquement
    }

    PhysicsWorld &PhysicsWorld::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::make_unique<PhysicsWorld>();
        }
        return *s_instance;
    }

    sf::Vector2f PhysicsWorld::metersToPixels(const b2Vec2 &meterPos)
    {
        return sf::Vector2f(meterPos.x * PIXELS_PER_METER, meterPos.y * PIXELS_PER_METER);
    }

    b2Vec2 PhysicsWorld::pixelsToMeters(const sf::Vector2f &pixelPos)
    {
        return b2Vec2(pixelPos.x * METERS_PER_PIXEL, pixelPos.y * METERS_PER_PIXEL);
    }

    float PhysicsWorld::metersToPixels(float meterValue)
    {
        return meterValue * PIXELS_PER_METER;
    }

    float PhysicsWorld::pixelsToMeters(float pixelValue)
    {
        return pixelValue * METERS_PER_PIXEL;
    }

    void PhysicsWorld::update(float deltaTime)
    {
        // Limiter le pas de temps à 1/60 secondes (pour éviter des comportements étranges)
        const float timeStep = std::min(deltaTime, 1.0f / 60.0f);

        // Mettre à jour le monde physique
        m_world->Step(timeStep, m_velocityIterations, m_positionIterations);
    }

    b2Body *PhysicsWorld::createBody(const sf::Vector2f &position, b2BodyType type)
    {
        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position = pixelsToMeters(position);
        bodyDef.fixedRotation = false;

        return m_world->CreateBody(&bodyDef);
    }

    void PhysicsWorld::destroyBody(b2Body *body)
    {
        if (body)
        {
            m_world->DestroyBody(body);
        }
    }

    b2Fixture *PhysicsWorld::addBoxFixture(b2Body *body, const sf::Vector2f &size,
                                           float density, float friction, float restitution,
                                           uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!body)
            return nullptr;

        // Créer une forme de boîte
        b2PolygonShape shape;
        shape.SetAsBox(pixelsToMeters(size.x / 2.0f), pixelsToMeters(size.y / 2.0f));

        // Définir les propriétés de la fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.filter.categoryBits = categoryBits;
        fixtureDef.filter.maskBits = maskBits;
        fixtureDef.isSensor = isSensor;

        return body->CreateFixture(&fixtureDef);
    }

    b2Fixture *PhysicsWorld::addCircleFixture(b2Body *body, float radius,
                                              float density, float friction, float restitution,
                                              uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!body)
            return nullptr;

        // Créer une forme circulaire
        b2CircleShape shape;
        shape.m_radius = pixelsToMeters(radius);

        // Définir les propriétés de la fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.filter.categoryBits = categoryBits;
        fixtureDef.filter.maskBits = maskBits;
        fixtureDef.isSensor = isSensor;

        return body->CreateFixture(&fixtureDef);
    }

    ContactListener &PhysicsWorld::getContactListener()
    {
        return m_contactListener;
    }

    void PhysicsWorld::debugDraw(sf::RenderWindow &window)
    {
        if (!m_debugDrawEnabled)
            return;

        m_debugLines.clear();

        // Parcourir tous les corps et dessiner leurs fixtures
        for (b2Body *body = m_world->GetBodyList(); body; body = body->GetNext())
        {
            const b2Transform &transform = body->GetTransform();

            for (b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                b2Shape *shape = fixture->GetShape();

                switch (shape->GetType())
                {
                case b2Shape::e_circle:
                {
                    // Dessiner un cercle
                    b2CircleShape *circle = static_cast<b2CircleShape *>(shape);

                    const int segments = 20;
                    const float radius = metersToPixels(circle->m_radius);
                    const sf::Vector2f center = metersToPixels(b2Mul(transform, circle->m_p));

                    for (int i = 0; i < segments; i++)
                    {
                        float angle1 = i * 2.0f * b2_pi / segments;
                        float angle2 = (i + 1) * 2.0f * b2_pi / segments;

                        sf::Vector2f p1(center.x + radius * std::cos(angle1),
                                        center.y + radius * std::sin(angle1));
                        sf::Vector2f p2(center.x + radius * std::cos(angle2),
                                        center.y + radius * std::sin(angle2));

                        m_debugLines.append(sf::Vertex(p1, sf::Color::Green));
                        m_debugLines.append(sf::Vertex(p2, sf::Color::Green));
                    }
                    break;
                }

                case b2Shape::e_polygon:
                {
                    // Dessiner un polygone
                    b2PolygonShape *poly = static_cast<b2PolygonShape *>(shape);
                    const int vertexCount = poly->m_count;

                    for (int i = 0; i < vertexCount; i++)
                    {
                        int i2 = (i + 1) % vertexCount;

                        sf::Vector2f p1 = metersToPixels(b2Mul(transform, poly->m_vertices[i]));
                        sf::Vector2f p2 = metersToPixels(b2Mul(transform, poly->m_vertices[i2]));

                        m_debugLines.append(sf::Vertex(p1, sf::Color::Green));
                        m_debugLines.append(sf::Vertex(p2, sf::Color::Green));
                    }
                    break;
                }

                default:
                    // Types de formes non pris en charge
                    break;
                }
            }
        }

        window.draw(m_debugLines);
    }

} // namespace Orenji