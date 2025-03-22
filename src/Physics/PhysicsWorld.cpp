#include "../../include/Physics/PhysicsWorld.hpp"
#include <iostream>
#include <cassert>

// Callbacks pour la gestion des contacts Box2D
void BeginContactCallback(void *listener, void *contact)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->BeginContact(contact);
}

void EndContactCallback(void *listener, void *contact)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->EndContact(contact);
}

void PreSolveCallback(void *listener, void *contact, void *oldManifold)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->PreSolve(contact, oldManifold);
}

void PostSolveCallback(void *listener, void *contact, void *impulse)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->PostSolve(contact, impulse);
}

namespace Orenji
{
    // Initialisation des statiques
    std::unique_ptr<PhysicsWorld> PhysicsWorld::s_instance = nullptr;
    b2StackAllocator *PhysicsWorld::s_stackAllocator = nullptr;

    // Implémentation de ContactListener
    PhysicsWorld::ContactListener::ContactListener()
        : m_beginContactCallback(nullptr), m_endContactCallback(nullptr),
          m_preSolveCallback(nullptr), m_postSolveCallback(nullptr)
    {
    }

    void PhysicsWorld::ContactListener::BeginContact(b2ContactId contact)
    {
        if (m_beginContactCallback)
        {
            m_beginContactCallback(contact);
        }
    }

    void PhysicsWorld::ContactListener::EndContact(b2ContactId contact)
    {
        if (m_endContactCallback)
        {
            m_endContactCallback(contact);
        }
    }

    void PhysicsWorld::ContactListener::PreSolve(b2ContactId contact, const b2Manifold *oldManifold)
    {
        if (m_preSolveCallback)
        {
            m_preSolveCallback(contact, oldManifold);
        }
    }

    void PhysicsWorld::ContactListener::PostSolve(b2ContactId contact, const b2ContactImpulse *impulse)
    {
        if (m_postSolveCallback)
        {
            m_postSolveCallback(contact, impulse);
        }
    }

    void PhysicsWorld::ContactListener::SetBeginContactCallback(BeginContactCallback callback)
    {
        m_beginContactCallback = callback;
    }

    void PhysicsWorld::ContactListener::SetEndContactCallback(EndContactCallback callback)
    {
        m_endContactCallback = callback;
    }

    void PhysicsWorld::ContactListener::SetPreSolveCallback(PreSolveCallback callback)
    {
        m_preSolveCallback = callback;
    }

    void PhysicsWorld::ContactListener::SetPostSolveCallback(PostSolveCallback callback)
    {
        m_postSolveCallback = callback;
    }

    // Implémentation de PhysicsWorld
    PhysicsWorld &PhysicsWorld::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::make_unique<PhysicsWorld>();
        }
        return *s_instance;
    }

    PhysicsWorld::PhysicsWorld()
        : m_world(b2_nullWorldId),
          m_debugDrawEnabled(false),
          m_velocityIterations(8),
          m_positionIterations(3),
          m_subStepCount(1)
    {
        // Allouer l'allocateur de pile pour Box2D si nécessaire
        if (s_stackAllocator == nullptr)
        {
            s_stackAllocator = new b2StackAllocator();
        }

        // Initialiser le listener de contact
        m_contactListener = std::make_unique<ContactListener>();
    }

    PhysicsWorld::~PhysicsWorld()
    {
        // Détruire le monde physique s'il existe
        if (IsValid(m_world))
        {
            b2DestroyWorld(m_world);
            m_world = b2_nullWorldId;
        }

        // Libérer l'allocateur de pile si nous sommes la dernière instance
        if (s_instance.get() == this && s_stackAllocator != nullptr)
        {
            delete s_stackAllocator;
            s_stackAllocator = nullptr;
        }
    }

    void PhysicsWorld::initialize(const sf::Vector2f &gravity)
    {
        // Créer un nouveau monde s'il n'existe pas déjà
        if (!IsValid(m_world))
        {
            // Configurer les paramètres du monde
            b2WorldDef worldDef;
            worldDef.gravity = pixelsToMeters(gravity);

            // Créer le monde Box2D
            m_world = b2CreateWorld(&worldDef);

            // Configurer les événements de contact
            m_contactEvent.beginFcn = [](b2WorldId worldId, void *context, b2ContactId contactId)
            {
                auto *physicsWorld = static_cast<PhysicsWorld *>(context);
                if (physicsWorld->m_contactListener)
                {
                    physicsWorld->m_contactListener->BeginContact(contactId);
                }
            };

            m_contactEvent.endFcn = [](b2WorldId worldId, void *context, b2ContactId contactId)
            {
                auto *physicsWorld = static_cast<PhysicsWorld *>(context);
                if (physicsWorld->m_contactListener)
                {
                    physicsWorld->m_contactListener->EndContact(contactId);
                }
            };

            m_contactEvent.preSolveFcn = [](b2WorldId worldId, void *context, b2ContactId contactId, const b2Manifold *oldManifold)
            {
                auto *physicsWorld = static_cast<PhysicsWorld *>(context);
                if (physicsWorld->m_contactListener)
                {
                    physicsWorld->m_contactListener->PreSolve(contactId, oldManifold);
                }
            };

            m_contactEvent.postSolveFcn = [](b2WorldId worldId, void *context, b2ContactId contactId, const b2ContactImpulse *impulse)
            {
                auto *physicsWorld = static_cast<PhysicsWorld *>(context);
                if (physicsWorld->m_contactListener)
                {
                    physicsWorld->m_contactListener->PostSolve(contactId, impulse);
                }
            };

            m_contactEvent.context = this;

            // Enregistrer les événements de contact
            b2World_SetContactEvents(m_world, &m_contactEvent);
        }
        else
        {
            // Mettre à jour la gravité si le monde existe déjà
            setGravity(gravity);
        }
    }

    void PhysicsWorld::update(float timeStep)
    {
        if (!IsValid(m_world))
            return;

        float subStep = timeStep / static_cast<float>(m_subStepCount);
        for (int i = 0; i < m_subStepCount; ++i)
        {
            b2World_Step(m_world, subStep, m_velocityIterations, m_positionIterations);
        }
    }

    b2BodyId PhysicsWorld::createBody(const sf::Vector2f &position, b2BodyType type)
    {
        if (!IsValid(m_world))
            return b2_nullBodyId;

        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position = pixelsToMeters(position);

        return b2CreateBody(m_world, &bodyDef);
    }

    void PhysicsWorld::destroyBody(b2BodyId body)
    {
        if (!IsValid(m_world) || !IsValid(body))
            return;

        b2DestroyBody(body);
    }

    b2FixtureId PhysicsWorld::addBoxFixture(b2BodyId body, const sf::Vector2f &size,
                                            float density, float friction,
                                            uint16_t categoryBits, uint16_t maskBits,
                                            bool isSensor)
    {
        if (!IsValid(m_world) || !IsValid(body))
            return b2_nullFixtureId;

        // Créer une forme rectangulaire
        b2ShapeDef shapeDef;
        shapeDef.density = density;
        shapeDef.friction = friction;
        shapeDef.isSensor = isSensor;
        shapeDef.filter.categoryBits = categoryBits;
        shapeDef.filter.maskBits = maskBits;

        // Créer la forme de boîte
        b2BoxShape box;
        // La taille est la moitié de la largeur/hauteur dans Box2D
        box.SetHalfExtents(pixelsToMeters(size.x * 0.5f), pixelsToMeters(size.y * 0.5f));

        // Créer la forme dans le monde
        b2ShapeId shapeId = b2CreateShape(m_world, &box.m_def);

        // Attacher la forme au corps
        b2FixtureId fixtureId = b2CreateFixture(body, shapeId, &shapeDef);

        // Libérer la forme
        b2DestroyShape(shapeId);

        return fixtureId;
    }

    b2FixtureId PhysicsWorld::addCircleFixture(b2BodyId body, float radius,
                                               float density, float friction,
                                               uint16_t categoryBits, uint16_t maskBits,
                                               bool isSensor)
    {
        if (!IsValid(m_world) || !IsValid(body))
            return b2_nullFixtureId;

        // Créer une forme circulaire
        b2ShapeDef shapeDef;
        shapeDef.density = density;
        shapeDef.friction = friction;
        shapeDef.isSensor = isSensor;
        shapeDef.filter.categoryBits = categoryBits;
        shapeDef.filter.maskBits = maskBits;

        // Créer la forme de cercle
        b2CircleShape circle;
        circle.m_radius = pixelsToMeters(radius);

        // Créer la forme dans le monde
        b2ShapeId shapeId = b2CreateShape(m_world, &circle.m_def);

        // Attacher la forme au corps
        b2FixtureId fixtureId = b2CreateFixture(body, shapeId, &shapeDef);

        // Libérer la forme
        b2DestroyShape(shapeId);

        return fixtureId;
    }

    void PhysicsWorld::setBeginContactCallback(ContactListener::BeginContactCallback callback)
    {
        if (m_contactListener)
        {
            m_contactListener->SetBeginContactCallback(callback);
        }
    }

    void PhysicsWorld::setEndContactCallback(ContactListener::EndContactCallback callback)
    {
        if (m_contactListener)
        {
            m_contactListener->SetEndContactCallback(callback);
        }
    }

    void PhysicsWorld::setPreSolveCallback(ContactListener::PreSolveCallback callback)
    {
        if (m_contactListener)
        {
            m_contactListener->SetPreSolveCallback(callback);
        }
    }

    void PhysicsWorld::setPostSolveCallback(ContactListener::PostSolveCallback callback)
    {
        if (m_contactListener)
        {
            m_contactListener->SetPostSolveCallback(callback);
        }
    }

    float PhysicsWorld::metersToPixels(float meters)
    {
        return meters * PIXELS_PER_METER;
    }

    sf::Vector2f PhysicsWorld::metersToPixels(const b2Vec2 &metersVec)
    {
        return sf::Vector2f(metersVec.x * PIXELS_PER_METER, metersVec.y * PIXELS_PER_METER);
    }

    float PhysicsWorld::pixelsToMeters(float pixels)
    {
        return pixels * METERS_PER_PIXEL;
    }

    b2Vec2 PhysicsWorld::pixelsToMeters(const sf::Vector2f &pixelsVec)
    {
        return b2Vec2(pixelsVec.x * METERS_PER_PIXEL, pixelsVec.y * METERS_PER_PIXEL);
    }

    void PhysicsWorld::setGravity(const sf::Vector2f &gravity)
    {
        if (IsValid(m_world))
        {
            b2Vec2 b2Gravity = pixelsToMeters(gravity);
            b2World_SetGravity(m_world, b2Gravity);
        }
    }

    sf::Vector2f PhysicsWorld::getGravity() const
    {
        if (IsValid(m_world))
        {
            b2Vec2 b2Gravity = b2World_GetGravity(m_world);
            return metersToPixels(b2Gravity);
        }
        return sf::Vector2f(0, 0);
    }

    void PhysicsWorld::debugDraw(sf::RenderWindow &window)
    {
        if (!IsValid(m_world) || !m_debugDrawEnabled)
            return;

        // Parcourir tous les corps du monde
        b2BodyId bodyId = b2World_GetFirstBody(m_world);
        while (IsValid(bodyId))
        {
            b2Transform transform = b2Body_GetTransform(bodyId);
            sf::Vector2f worldPos = metersToPixels(transform.p);

            // Couleur différente selon le type de corps
            sf::Color color;
            switch (b2Body_GetType(bodyId))
            {
            case b2_staticBody:
                color = sf::Color(128, 128, 128); // Gris
                break;
            case b2_kinematicBody:
                color = sf::Color(0, 128, 255); // Bleu clair
                break;
            case b2_dynamicBody:
                color = sf::Color(0, 255, 0); // Vert
                break;
            default:
                color = sf::Color::White;
                break;
            }

            // Parcourir toutes les fixtures du corps
            b2FixtureId fixtureId = GetBodyFixtureList(bodyId);
            while (IsValid(fixtureId))
            {
                // Sélectionner une couleur différente si c'est un capteur
                sf::Color fixtureColor = b2Fixture_IsSensor(fixtureId) ? sf::Color(255, 255, 0) : color;

                // Récupérer la forme de la fixture
                b2ShapeId shapeId = b2Fixture_GetShape(fixtureId);
                b2ShapeType shapeType = b2Shape_GetType(shapeId);

                // Dessiner selon le type de forme
                switch (shapeType)
                {
                case b2ShapeType::e_circle:
                {
                    float radius = metersToPixels(b2CircleShape_GetRadius(shapeId));
                    sf::CircleShape circle(radius);
                    circle.setOrigin(radius, radius);
                    circle.setPosition(worldPos);
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineColor(fixtureColor);
                    circle.setOutlineThickness(1.0f);
                    window.draw(circle);

                    // Dessiner une ligne du centre vers le bord pour indiquer la rotation
                    float angle = GetBodyAngle(bodyId);
                    sf::Vertex line[2] = {
                        sf::Vertex(worldPos, fixtureColor),
                        sf::Vertex(worldPos + sf::Vector2f(std::cos(angle) * radius, std::sin(angle) * radius), fixtureColor)};
                    window.draw(line, 2, sf::Lines);
                    break;
                }
                case b2ShapeType::e_polygon:
                {
                    int vertexCount = b2PolygonShape_GetVertexCount(shapeId);
                    sf::ConvexShape polygon(vertexCount);
                    polygon.setFillColor(sf::Color::Transparent);
                    polygon.setOutlineColor(fixtureColor);
                    polygon.setOutlineThickness(1.0f);

                    for (int i = 0; i < vertexCount; ++i)
                    {
                        // Récupérer le sommet et le transformer
                        b2Vec2 vertex = b2PolygonShape_GetVertex(shapeId, i);

                        // Appliquer la rotation
                        b2Rot rotation = transform.q;
                        b2Vec2 rotatedVertex;
                        b2RotMulVec2(&rotation, &vertex, &rotatedVertex);

                        // Convertir en position mondiale et définir le point
                        sf::Vector2f worldVertex = worldPos + metersToPixels(rotatedVertex);
                        polygon.setPoint(i, worldVertex);
                    }

                    window.draw(polygon);
                    break;
                }
                default:
                    // Autres types de formes non gérées pour le moment
                    break;
                }

                fixtureId = GetNextFixture(fixtureId);
            }

            bodyId = b2Body_GetNext(bodyId);
        }
    }
} // namespace Orenji