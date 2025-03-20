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
    // Initialisation du singleton
    PhysicsWorld *PhysicsWorld::s_instance = nullptr;

    // Implémentation de ContactListener
    PhysicsWorld::ContactListener::ContactListener()
        : m_beginContactCallback(nullptr), m_endContactCallback(nullptr),
          m_preSolveCallback(nullptr), m_postSolveCallback(nullptr)
    {
    }

    void PhysicsWorld::ContactListener::BeginContact(void *contact)
    {
        if (m_beginContactCallback)
        {
            m_beginContactCallback(contact);
        }
    }

    void PhysicsWorld::ContactListener::EndContact(void *contact)
    {
        if (m_endContactCallback)
        {
            m_endContactCallback(contact);
        }
    }

    void PhysicsWorld::ContactListener::PreSolve(void *contact, const void *oldManifold)
    {
        if (m_preSolveCallback)
        {
            m_preSolveCallback(contact, oldManifold);
        }
    }

    void PhysicsWorld::ContactListener::PostSolve(void *contact, const void *impulse)
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
    PhysicsWorld::PhysicsWorld()
        : m_world{b2_nullWorldId},
          m_velocityIterations(8),
          m_positionIterations(3),
          m_debugDrawEnabled(false)
    {
    }

    void PhysicsWorld::initialize(const sf::Vector2f &gravity)
    {
        // Créer une définition de monde Box2D
        b2WorldDef worldDef;
        worldDef.gravity.x = gravity.x * METERS_PER_PIXEL;
        worldDef.gravity.y = gravity.y * METERS_PER_PIXEL;

        // Créer le monde Box2D
        m_world = b2CreateWorld(&worldDef);

        if (!IsValid(m_world))
        {
            assert(false && "Failed to create Box2D world");
            return;
        }
    }

    PhysicsWorld::~PhysicsWorld()
    {
        if (IsValid(m_world))
        {
            b2DestroyWorld(m_world);
            m_world = b2_nullWorldId;
        }
    }

    PhysicsWorld &PhysicsWorld::getInstance()
    {
        if (!s_instance)
        {
            s_instance = new PhysicsWorld();
        }
        return *s_instance;
    }

    // Conversion entre pixels et mètres
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
        b2Vec2 vec;
        vec.x = pixelsVec.x * METERS_PER_PIXEL;
        vec.y = pixelsVec.y * METERS_PER_PIXEL;
        return vec;
    }

    // Mise à jour du monde physique
    void PhysicsWorld::update(float timeStep)
    {
        if (!IsValid(m_world))
            return;

        b2World_Step(m_world, timeStep, m_velocityIterations, m_positionIterations);
    }

    // Création d'un corps physique
    b2BodyId PhysicsWorld::createBody(const sf::Vector2f &position, b2BodyType type)
    {
        if (!IsValid(m_world))
            return b2_nullBodyId;

        // Créer une définition de corps Box2D
        b2BodyDef bodyDef;
        bodyDef.type = type;

        // Convertir la position de pixels à mètres
        b2Vec2 pos = pixelsToMeters(position);
        bodyDef.position = pos;

        // Créer le corps
        return b2CreateBody(m_world, &bodyDef);
    }

    // Destruction d'un corps physique
    void PhysicsWorld::destroyBody(b2BodyId body)
    {
        if (!IsValid(m_world) || !IsValid(body))
            return;

        b2DestroyBody(body);
    }

    // Ajout d'une fixture rectangulaire
    b2FixtureId PhysicsWorld::addBoxFixture(b2BodyId body, const sf::Vector2f &size,
                                            float density, float friction,
                                            uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!IsValid(m_world) || !IsValid(body))
            return b2_nullFixtureId;

        // Créer une définition de forme de boîte Box2D
        b2ShapeDef shapeDef;
        shapeDef.friction = friction;
        shapeDef.density = density;
        shapeDef.isSensor = isSensor;
        shapeDef.filter.categoryBits = categoryBits;
        shapeDef.filter.maskBits = maskBits;

        // Créer la forme de boîte
        b2BoxDef boxDef;
        boxDef.shapeDef = shapeDef;
        boxDef.halfWidth = pixelsToMeters(size.x / 2.0f);
        boxDef.halfHeight = pixelsToMeters(size.y / 2.0f);

        // Créer la shape et la fixture
        b2ShapeId shape = b2CreateBoxShape(&boxDef);
        b2FixtureId fixture = b2Body_CreateFixture(body, shape);

        // Nettoyer la shape (Box2D 2.4.x ne le fait pas automatiquement)
        b2DestroyShape(shape);

        return fixture;
    }

    // Ajout d'une fixture circulaire
    b2FixtureId PhysicsWorld::addCircleFixture(b2BodyId body, float radius,
                                               float density, float friction,
                                               uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!IsValid(m_world) || !IsValid(body))
            return b2_nullFixtureId;

        // Créer une définition de forme de cercle Box2D
        b2ShapeDef shapeDef;
        shapeDef.friction = friction;
        shapeDef.density = density;
        shapeDef.isSensor = isSensor;
        shapeDef.filter.categoryBits = categoryBits;
        shapeDef.filter.maskBits = maskBits;

        // Créer la forme de cercle
        b2CircleDef circleDef;
        circleDef.shapeDef = shapeDef;
        circleDef.radius = pixelsToMeters(radius);

        // Créer la shape et la fixture
        b2ShapeId shape = b2CreateCircleShape(&circleDef);
        b2FixtureId fixture = b2Body_CreateFixture(body, shape);

        // Nettoyer la shape (Box2D 2.4.x ne le fait pas automatiquement)
        b2DestroyShape(shape);

        return fixture;
    }

    // Callbacks pour la gestion des collisions
    void PhysicsWorld::setBeginContactCallback(ContactListener::BeginContactCallback callback)
    {
        m_contactListener.SetBeginContactCallback(callback);
    }

    void PhysicsWorld::setEndContactCallback(ContactListener::EndContactCallback callback)
    {
        m_contactListener.SetEndContactCallback(callback);
    }

    void PhysicsWorld::setPreSolveCallback(ContactListener::PreSolveCallback callback)
    {
        m_contactListener.SetPreSolveCallback(callback);
    }

    void PhysicsWorld::setPostSolveCallback(ContactListener::PostSolveCallback callback)
    {
        m_contactListener.SetPostSolveCallback(callback);
    }

    // Debug draw
    void PhysicsWorld::setDebugDraw(bool enabled)
    {
        m_debugDrawEnabled = enabled;
    }

    bool PhysicsWorld::isDebugDrawEnabled() const
    {
        return m_debugDrawEnabled;
    }

    void PhysicsWorld::debugDraw(sf::RenderWindow &window)
    {
        if (!IsValid(m_world) || !m_debugDrawEnabled)
            return;

        // Énumérer tous les corps du monde
        b2BodyId bodyId = b2_nullBodyId;
        uint32_t bodyIndex = 0;

        while ((bodyId = b2World_GetBodyAtIndex(m_world, bodyIndex++)).index != b2_nullBodyId.index)
        {
            // Obtenir la position et la rotation du corps
            b2Vec2 position = b2Body_GetPosition(bodyId);
            float angle = b2Body_GetAngle(bodyId);

            // Convertir en pixels
            sf::Vector2f worldPos = metersToPixels(position);

            // Énumérer toutes les fixtures du corps
            b2FixtureId fixtureId = b2_nullFixtureId;
            uint32_t fixtureIndex = 0;

            while ((fixtureId = b2Body_GetFixtureAtIndex(bodyId, fixtureIndex++)).index != b2_nullFixtureId.index)
            {
                // Obtenir la forme de la fixture
                b2ShapeId shapeId = b2Fixture_GetShape(fixtureId);
                b2ShapeType shapeType = b2Shape_GetType(shapeId);

                // Couleur selon le type de corps
                sf::Color color;
                switch (b2Body_GetType(bodyId))
                {
                case b2_staticBody:
                    color = sf::Color(128, 128, 128); // Gris
                    break;
                case b2_kinematicBody:
                    color = sf::Color(128, 128, 255); // Bleu clair
                    break;
                case b2_dynamicBody:
                    color = sf::Color(255, 255, 255); // Blanc
                    break;
                default:
                    color = sf::Color::Red; // Erreur
                    break;
                }

                // Modifier la couleur si c'est un capteur
                if (b2Fixture_IsSensor(fixtureId))
                {
                    color.a = 128; // Semi-transparent
                }

                // Dessiner selon le type de forme
                if (shapeType == b2_circleShape)
                {
                    // Cercle
                    float radius = metersToPixels(b2CircleShape_GetRadius(shapeId));
                    sf::CircleShape circle(radius);
                    circle.setOrigin(radius, radius);
                    circle.setPosition(worldPos);
                    circle.setRotation(angle * 180.0f / b2_pi);
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineColor(color);
                    circle.setOutlineThickness(1.0f);
                    window.draw(circle);

                    // Dessiner une ligne pour montrer la rotation
                    sf::Vertex line[2] = {
                        sf::Vertex(worldPos, color),
                        sf::Vertex(worldPos + sf::Vector2f(std::cos(angle) * radius, std::sin(angle) * radius), color)};
                    window.draw(line, 2, sf::Lines);
                }
                else if (shapeType == b2_polygonShape)
                {
                    // Polygone (boîte)
                    int vertexCount = b2PolygonShape_GetVertexCount(shapeId);
                    sf::ConvexShape polygon(vertexCount);

                    for (int i = 0; i < vertexCount; ++i)
                    {
                        // Obtenir le sommet
                        b2Vec2 vertex = b2PolygonShape_GetVertex(shapeId, i);

                        // Appliquer la rotation du corps
                        b2Rot rotation;
                        b2Rot_Set(&rotation, angle);
                        b2Vec2 rotatedVertex;
                        b2RotMulVec2(&rotation, &vertex, &rotatedVertex);

                        // Convertir en pixels et ajouter à la position du corps
                        sf::Vector2f worldVertex = worldPos + metersToPixels(rotatedVertex);
                        polygon.setPoint(i, worldVertex);
                    }

                    polygon.setFillColor(sf::Color::Transparent);
                    polygon.setOutlineColor(color);
                    polygon.setOutlineThickness(1.0f);
                    window.draw(polygon);
                }
            }
        }
    }

} // namespace Orenji