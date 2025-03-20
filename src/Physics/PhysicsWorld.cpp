#include "../../include/Physics/PhysicsWorld.hpp"
#include <iostream>

// Callbacks pour la gestion des contacts Box2D
void BeginContactCallback(void *listener, void *contact)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->BeginContact(static_cast<b2ContactId>(contact));
}

void EndContactCallback(void *listener, void *contact)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->EndContact(static_cast<b2ContactId>(contact));
}

void PreSolveCallback(void *listener, void *contact, void *oldManifold)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->PreSolve(static_cast<b2ContactId>(contact),
                              static_cast<const b2Manifold *>(oldManifold));
}

void PostSolveCallback(void *listener, void *contact, void *impulse)
{
    auto *contactListener = static_cast<PhysicsWorld::ContactListener *>(listener);
    contactListener->PostSolve(static_cast<b2ContactId>(contact),
                               static_cast<const b2ContactImpulse *>(impulse));
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
    PhysicsWorld::PhysicsWorld(const sf::Vector2f &gravity)
        : m_debugLines(sf::Lines), m_debugDrawEnabled(false),
          m_velocityIterations(8), m_positionIterations(3)
    {
        initialize(gravity);
    }

    void PhysicsWorld::initialize(const sf::Vector2f &gravity)
    {
        // Conversion de gravité de pixels à mètres
        b2Vec2 gravityVec = pixelsToMeters(gravity);

        // Création du monde Box2D
        m_world = b2CreateWorld(gravityVec);

        // Configuration des callbacks de contact
        b2ContactCallbacks callbacks;
        callbacks.beginFcn = BeginContactCallback;
        callbacks.endFcn = EndContactCallback;
        callbacks.preSolveFcn = PreSolveCallback;
        callbacks.postSolveFcn = PostSolveCallback;

        b2SetContactCallbacks(m_world, &m_contactListener, callbacks);
    }

    PhysicsWorld::~PhysicsWorld()
    {
        if (bool(m_world))
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
    sf::Vector2f PhysicsWorld::metersToPixels(const b2Vec2 &metersVec)
    {
        return sf::Vector2f(metersVec.x * PIXELS_PER_METER, metersVec.y * PIXELS_PER_METER);
    }

    sf::Vector2f PhysicsWorld::metersToPixels(float x, float y)
    {
        return sf::Vector2f(x * PIXELS_PER_METER, y * PIXELS_PER_METER);
    }

    float PhysicsWorld::metersToPixels(float meters)
    {
        return meters * PIXELS_PER_METER;
    }

    b2Vec2 PhysicsWorld::pixelsToMeters(const sf::Vector2f &pixelsVec)
    {
        return b2Vec2(pixelsVec.x * METERS_PER_PIXEL, pixelsVec.y * METERS_PER_PIXEL);
    }

    b2Vec2 PhysicsWorld::pixelsToMeters(float x, float y)
    {
        return b2Vec2(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);
    }

    float PhysicsWorld::pixelsToMeters(float pixels)
    {
        return pixels * METERS_PER_PIXEL;
    }

    // Mise à jour du monde physique
    void PhysicsWorld::update(float timeStep)
    {
        if (!bool(m_world))
        {
            std::cerr << "Erreur: monde Box2D non initialisé" << std::endl;
            return;
        }

        // Mise à jour de la simulation Box2D
        b2World_Step(m_world, timeStep, m_velocityIterations);
    }

    // Création d'un corps physique
    b2BodyId PhysicsWorld::createBody(const sf::Vector2f &position, b2BodyType type)
    {
        if (!bool(m_world))
        {
            return b2_nullBodyId;
        }

        // Définition du corps
        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position = pixelsToMeters(position);

        // Création du corps
        return b2CreateBody(m_world, &bodyDef);
    }

    // Destruction d'un corps physique
    void PhysicsWorld::destroyBody(b2BodyId body)
    {
        if (!bool(m_world) || !bool(body))
        {
            return;
        }

        b2DestroyBody(body);
    }

    // Ajout d'une fixture rectangulaire
    b2FixtureId PhysicsWorld::addBoxFixture(b2BodyId body, const sf::Vector2f &size,
                                            float density, float friction, float restitution,
                                            uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!bool(m_world) || !bool(body))
        {
            return b2_nullFixtureId;
        }

        // Conversion de taille de pixels à mètres
        sf::Vector2f halfSize(size.x * 0.5f, size.y * 0.5f);
        b2Vec2 b2HalfSize = pixelsToMeters(halfSize);

        // Création de la forme
        b2ShapeDef shapeDef;
        b2Polygon_SetAsBox(shapeDef.shape, b2HalfSize.x, b2HalfSize.y);

        // Configuration de la fixture
        shapeDef.density = density;
        shapeDef.friction = friction;
        shapeDef.restitution = restitution;
        shapeDef.filter.categoryBits = categoryBits;
        shapeDef.filter.maskBits = maskBits;
        shapeDef.isSensor = isSensor;

        // Ajout de la fixture au corps
        return b2Body_CreateFixture(body, &shapeDef);
    }

    // Ajout d'une fixture circulaire
    b2FixtureId PhysicsWorld::addCircleFixture(b2BodyId body, float radius,
                                               float density, float friction, float restitution,
                                               uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!bool(m_world) || !bool(body))
        {
            return b2_nullFixtureId;
        }

        // Conversion de rayon de pixels à mètres
        float b2Radius = pixelsToMeters(radius);

        // Création de la forme
        b2ShapeDef shapeDef;
        b2Circle_Set(shapeDef.shape, {0.0f, 0.0f}, b2Radius);

        // Configuration de la fixture
        shapeDef.density = density;
        shapeDef.friction = friction;
        shapeDef.restitution = restitution;
        shapeDef.filter.categoryBits = categoryBits;
        shapeDef.filter.maskBits = maskBits;
        shapeDef.isSensor = isSensor;

        // Ajout de la fixture au corps
        return b2Body_CreateFixture(body, &shapeDef);
    }

    // Callbacks pour la gestion des collisions
    void PhysicsWorld::setBeginContactCallback(BeginContactCallback callback)
    {
        m_contactListener.SetBeginContactCallback(callback);
    }

    void PhysicsWorld::setEndContactCallback(EndContactCallback callback)
    {
        m_contactListener.SetEndContactCallback(callback);
    }

    void PhysicsWorld::setPreSolveCallback(PreSolveCallback callback)
    {
        m_contactListener.SetPreSolveCallback(callback);
    }

    void PhysicsWorld::setPostSolveCallback(PostSolveCallback callback)
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
        if (!bool(m_world) || !m_debugDrawEnabled)
        {
            return;
        }

        // Pour chaque corps dans le monde
        b2BodyId bodyId = b2World_GetFirstBody(m_world);
        while (bool(bodyId))
        {
            // Position et rotation du corps
            b2Vec2 position = b2Body_GetPosition(bodyId);
            float angle = b2Body_GetAngle(bodyId);

            sf::Vector2f sfPosition = metersToPixels(position);

            // Pour chaque fixture du corps
            b2FixtureId fixtureId = b2Body_GetFirstFixture(bodyId);
            while (bool(fixtureId))
            {
                // Récupération de la forme
                b2ShapeId shapeId = b2Fixture_GetShape(fixtureId);
                b2ShapeType shapeType = b2Shape_GetType(shapeId);

                // Couleur de la forme selon le type de corps et si c'est un capteur
                sf::Color shapeColor;
                b2BodyType bodyType = b2Body_GetType(bodyId);

                if (bodyType == b2_staticBody)
                {
                    shapeColor = sf::Color(128, 128, 128, 128); // Gris
                }
                else if (bodyType == b2_kinematicBody)
                {
                    shapeColor = sf::Color(128, 128, 255, 128); // Bleu clair
                }
                else // if (bodyType == b2_dynamicBody)
                {
                    shapeColor = sf::Color(255, 128, 128, 128); // Rouge clair
                }

                // Si c'est un capteur, utiliser une couleur semi-transparente
                if (b2Fixture_IsSensor(fixtureId))
                {
                    shapeColor.a = 64;
                }

                // Dessin selon le type de forme
                switch (shapeType)
                {
                case b2ShapeType::b2_circleShape:
                {
                    // Cercle
                    float radius = metersToPixels(b2Circle_GetRadius(shapeId));

                    sf::CircleShape circle(radius);
                    circle.setOrigin(radius, radius);
                    circle.setPosition(sfPosition);
                    circle.setRotation(angle * 180.0f / box2d::b2_pi);
                    circle.setFillColor(sf::Color::Transparent);
                    circle.setOutlineColor(shapeColor);
                    circle.setOutlineThickness(1.0f);

                    window.draw(circle);

                    // Ligne pour montrer la rotation
                    sf::Vertex line[2] =
                        {
                            sf::Vertex(sfPosition, shapeColor),
                            sf::Vertex(
                                sf::Vector2f(
                                    sfPosition.x + std::cos(angle) * radius,
                                    sfPosition.y + std::sin(angle) * radius),
                                shapeColor)};

                    window.draw(line, 2, sf::Lines);
                    break;
                }

                case b2ShapeType::b2_polygonShape:
                {
                    // Polygone (boîte)
                    int vertexCount = b2Polygon_GetVertexCount(shapeId);

                    sf::ConvexShape polygon(vertexCount);
                    for (int i = 0; i < vertexCount; ++i)
                    {
                        b2Vec2 vertex = b2Polygon_GetVertex(shapeId, i);

                        // Transformer le vertex selon la position et rotation du corps
                        b2Vec2 worldVertex;
                        b2Rot rotation;
                        b2Rot_Set(&rotation, angle);
                        b2RotMulVec2(&rotation, vertex, &worldVertex);
                        worldVertex.x += position.x;
                        worldVertex.y += position.y;

                        sf::Vector2f sfVertex = metersToPixels(worldVertex);
                        polygon.setPoint(i, sfVertex);
                    }

                    polygon.setFillColor(sf::Color::Transparent);
                    polygon.setOutlineColor(shapeColor);
                    polygon.setOutlineThickness(1.0f);

                    window.draw(polygon);
                    break;
                }

                // Autres types de formes (chaîne, bords...)
                default:
                    // Non supporté pour le moment
                    break;
                }

                fixtureId = b2Fixture_GetNext(fixtureId);
            }

            bodyId = b2Body_GetNext(bodyId);
        }
    }

} // namespace Orenji