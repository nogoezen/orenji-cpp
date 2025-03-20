#include "../../include/Physics/PhysicsWorld.hpp"
#include <iostream>

// Fonctions callbacks externes pour Box2D
extern "C" void BeginContactCallback(void *userdata, void *contact)
{
    Orenji::ContactListener *listener = static_cast<Orenji::ContactListener *>(userdata);
    listener->BeginContact(static_cast<box2d::b2Contact *>(contact));
}

extern "C" void EndContactCallback(void *userdata, void *contact)
{
    Orenji::ContactListener *listener = static_cast<Orenji::ContactListener *>(userdata);
    listener->EndContact(static_cast<box2d::b2Contact *>(contact));
}

extern "C" void PreSolveCallback(void *userdata, void *contact, void *oldManifold)
{
    Orenji::ContactListener *listener = static_cast<Orenji::ContactListener *>(userdata);
    listener->PreSolve(static_cast<box2d::b2Contact *>(contact),
                       static_cast<box2d::b2Manifold *>(oldManifold));
}

extern "C" void PostSolveCallback(void *userdata, void *contact, void *impulse)
{
    Orenji::ContactListener *listener = static_cast<Orenji::ContactListener *>(userdata);
    listener->PostSolve(static_cast<box2d::b2Contact *>(contact),
                        static_cast<box2d::b2ContactImpulse *>(impulse));
}

namespace Orenji
{

    std::unique_ptr<PhysicsWorld> PhysicsWorld::s_instance = nullptr;

    //--------------------------------
    // Implémentation du ContactListener
    //--------------------------------

    void ContactListener::BeginContact(box2d::b2Contact *contact)
    {
        if (m_beginContactCallback)
        {
            m_beginContactCallback(contact);
        }
    }

    void ContactListener::EndContact(box2d::b2Contact *contact)
    {
        if (m_endContactCallback)
        {
            m_endContactCallback(contact);
        }
    }

    void ContactListener::PreSolve(box2d::b2Contact *contact, const box2d::b2Manifold *oldManifold)
    {
        if (m_preSolveCallback)
        {
            m_preSolveCallback(contact, oldManifold);
        }
    }

    void ContactListener::PostSolve(box2d::b2Contact *contact, const box2d::b2ContactImpulse *impulse)
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

    //--------------------------------
    // Implémentation du PhysicsWorld
    //--------------------------------

    PhysicsWorld::PhysicsWorld(const sf::Vector2f &gravity)
        : m_debugLines(sf::Lines), m_debugDrawEnabled(false)
    {
        // Créer le monde Box2D avec la gravité définie
        b2WorldDef worldDef;
        worldDef.gravity.x = gravity.x;
        worldDef.gravity.y = gravity.y;
        m_world = b2CreateWorld(&worldDef);

        // Configurer les callbacks de contact en passant le listener comme userdata
        b2ContactListenerJointDef listenerDef;
        listenerDef.beginContactFcn = BeginContactCallback;
        listenerDef.endContactFcn = EndContactCallback;
        listenerDef.preSolveFcn = PreSolveCallback;
        listenerDef.postSolveFcn = PostSolveCallback;
        listenerDef.userData = &m_contactListener;
        b2WorldSetContactListener(m_world, &listenerDef);
    }

    PhysicsWorld::~PhysicsWorld()
    {
        // Box2D nettoie automatiquement
        if (m_world)
        {
            b2DestroyWorld(m_world);
            m_world = nullptr;
        }
    }

    PhysicsWorld &PhysicsWorld::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::make_unique<PhysicsWorld>();
        }
        return *s_instance;
    }

    sf::Vector2f PhysicsWorld::metersToPixels(const box2d::b2Vec2 &meterPos)
    {
        return sf::Vector2f(meterPos.x * PIXELS_PER_METER, meterPos.y * PIXELS_PER_METER);
    }

    box2d::b2Vec2 PhysicsWorld::pixelsToMeters(const sf::Vector2f &pixelPos)
    {
        box2d::b2Vec2 result;
        result.x = pixelPos.x * METERS_PER_PIXEL;
        result.y = pixelPos.y * METERS_PER_PIXEL;
        return result;
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
        if (!m_world)
            return;

        float timeStep = 1.0f / 60.0f; // Fixed timestep (60 FPS)
        b2WorldStep(m_world, timeStep, m_velocityIterations, m_positionIterations);
    }

    b2BodyId PhysicsWorld::createBody(const sf::Vector2f &position, b2BodyType type)
    {
        if (!m_world)
            return nullptr;

        // Définir les propriétés du corps
        b2BodyDef bodyDef;
        bodyDef.type = type;
        box2d::b2Vec2 pos = pixelsToMeters(position);
        bodyDef.position.x = pos.x;
        bodyDef.position.y = pos.y;

        return b2WorldCreateBody(m_world, &bodyDef);
    }

    void PhysicsWorld::destroyBody(b2BodyId body)
    {
        if (!m_world || !body)
            return;

        b2WorldDestroyBody(m_world, body);
    }

    b2FixtureId PhysicsWorld::addBoxFixture(b2BodyId body, const sf::Vector2f &size,
                                            float density, float friction, float restitution,
                                            uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!m_world || !body)
            return nullptr;

        // Créer la forme de boîte
        b2PolygonShape boxShape;
        box2d::b2Vec2 halfSize = pixelsToMeters(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
        b2PolygonShapeSetAsBox(&boxShape, halfSize.x, halfSize.y);

        // Créer la fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.isSensor = isSensor;

        // Configurer les bits de catégorie et de masque pour les collisions
        box2d::b2Filter filter;
        filter.categoryBits = categoryBits;
        filter.maskBits = maskBits;
        fixtureDef.filter = filter;

        return b2BodyCreateFixture(body, &fixtureDef);
    }

    b2FixtureId PhysicsWorld::addCircleFixture(b2BodyId body, float radius,
                                               float density, float friction, float restitution,
                                               uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!m_world || !body)
            return nullptr;

        // Créer la forme circulaire
        b2CircleShape circleShape;
        circleShape.m_radius = pixelsToMeters(radius);

        // Créer la fixture
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        fixtureDef.isSensor = isSensor;

        // Configurer les bits de catégorie et de masque pour les collisions
        box2d::b2Filter filter;
        filter.categoryBits = categoryBits;
        filter.maskBits = maskBits;
        fixtureDef.filter = filter;

        return b2BodyCreateFixture(body, &fixtureDef);
    }

    ContactListener &PhysicsWorld::getContactListener()
    {
        return m_contactListener;
    }

    void PhysicsWorld::debugDraw(sf::RenderWindow &window)
    {
        if (!m_world || !m_debugDrawEnabled)
            return;

        m_debugLines.clear();

        // Parcourir tous les corps et dessiner leurs formes
        for (b2BodyId body = b2WorldGetBodyList(m_world); body; body = b2BodyGetNext(body))
        {
            // Convertir la position du corps en pixels pour l'affichage
            box2d::b2Vec2 position = b2BodyGetPosition(body);
            float angle = b2BodyGetAngle(body);
            sf::Vector2f sfmlPosition = metersToPixels(position);

            // Parcourir toutes les fixtures du corps
            for (b2FixtureId fixture = b2BodyGetFixtureList(body); fixture; fixture = b2FixtureGetNext(fixture))
            {
                // Obtenir la forme de la fixture
                b2ShapeId shape = b2FixtureGetShape(fixture);
                b2ShapeType shapeType = b2ShapeGetType(shape);

                // Définir la couleur en fonction du type de corps
                sf::Color color;
                switch (b2BodyGetType(body))
                {
                case b2_staticBody:
                    color = sf::Color(128, 128, 128, 128); // Gris pour les corps statiques
                    break;
                case b2_kinematicBody:
                    color = sf::Color(0, 128, 128, 128); // Cyan pour les corps cinématiques
                    break;
                case b2_dynamicBody:
                    color = sf::Color(0, 255, 0, 128); // Vert pour les corps dynamiques
                    break;
                default:
                    color = sf::Color::White;
                    break;
                }

                // Si la fixture est un capteur, utiliser une couleur différente
                if (b2FixtureIsSensor(fixture))
                {
                    color = sf::Color::Yellow;
                }

                // Dessiner la forme en fonction de son type
                switch (shapeType)
                {
                case b2ShapeType::e_circle:
                {
                    // Dessiner un cercle
                    float radius = metersToPixels(b2CircleShapeGetRadius(shape));

                    // Dessiner le cercle avec des lignes
                    const int segments = 16;
                    for (int i = 0; i < segments; i++)
                    {
                        float angle1 = angle + (i / (float)segments) * 2 * box2d::b2_pi;
                        float angle2 = angle + ((i + 1) / (float)segments) * 2 * box2d::b2_pi;

                        sf::Vector2f p1(sfmlPosition.x + radius * cos(angle1),
                                        sfmlPosition.y + radius * sin(angle1));
                        sf::Vector2f p2(sfmlPosition.x + radius * cos(angle2),
                                        sfmlPosition.y + radius * sin(angle2));

                        m_debugLines.append(sf::Vertex(p1, color));
                        m_debugLines.append(sf::Vertex(p2, color));
                    }

                    // Dessiner une ligne du centre au bord pour montrer la rotation
                    sf::Vector2f center = sfmlPosition;
                    sf::Vector2f edge(center.x + radius * cos(angle),
                                      center.y + radius * sin(angle));

                    m_debugLines.append(sf::Vertex(center, color));
                    m_debugLines.append(sf::Vertex(edge, color));
                }
                break;

                case b2ShapeType::e_polygon:
                {
                    // Dessiner un polygone
                    int vertexCount = b2PolygonShapeGetCount(shape);

                    // Dessiner le polygone avec des lignes
                    for (int i = 0; i < vertexCount; i++)
                    {
                        box2d::b2Vec2 v1 = b2PolygonShapeGetVertex(shape, i);
                        box2d::b2Vec2 v2 = b2PolygonShapeGetVertex(shape, (i + 1) % vertexCount);

                        // Transformer les coordonnées en fonction de la position et rotation du corps
                        float c = cos(angle);
                        float s = sin(angle);

                        sf::Vector2f p1(sfmlPosition.x + metersToPixels(v1.x * c - v1.y * s),
                                        sfmlPosition.y + metersToPixels(v1.x * s + v1.y * c));
                        sf::Vector2f p2(sfmlPosition.x + metersToPixels(v2.x * c - v2.y * s),
                                        sfmlPosition.y + metersToPixels(v2.x * s + v2.y * c));

                        m_debugLines.append(sf::Vertex(p1, color));
                        m_debugLines.append(sf::Vertex(p2, color));
                    }
                }
                break;

                default:
                    // Type de forme non pris en charge pour le debug drawing
                    break;
                }
            }
        }

        // Dessiner toutes les lignes debug accumulées
        window.draw(m_debugLines);
    }

} // namespace Orenji