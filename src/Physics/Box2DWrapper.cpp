#include "../../include/Physics/Box2DWrapper.hpp"
#include <cmath>
#include <box2d/constants.h>
#include <SFML/Graphics.hpp>

namespace Physics
{
    Box2DWrapper::Box2DWrapper(b2Vec2 gravity)
        : m_debugDrawEnabled(false)
    {
        // Créer une définition de monde avec la gravité
        b2WorldDef worldDef;
        worldDef.gravity = gravity;

        // Créer le monde avec la définition
        m_worldId = b2CreateWorld(&worldDef);
    }

    Box2DWrapper::~Box2DWrapper()
    {
        if (b2World_IsValid(m_worldId))
        {
            b2DestroyWorld(m_worldId);
        }
    }

    void Box2DWrapper::step(float timeStep, int velocityIterations, int positionIterations)
    {
        if (b2World_IsValid(m_worldId))
        {
            b2World_Step(m_worldId, timeStep, 1); // subStepCount = 1
        }
    }

    b2BodyId Box2DWrapper::createStaticBody(b2Vec2 position, float angle, void *userData)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position = pixelsToMeters(position);

        // Conversion en radians - utilisez la constante B2_pi de box2d/constants.h
        const float angleInRadians = angle * B2_pi / 180.0f;
        bodyDef.linearVelocity = {0.0f, 0.0f};
        bodyDef.angularVelocity = 0.0f;
        bodyDef.transform.q.s = sinf(angleInRadians);
        bodyDef.transform.q.c = cosf(angleInRadians);
        bodyDef.userData = userData;

        return b2CreateBody(m_worldId, &bodyDef);
    }

    b2BodyId Box2DWrapper::createDynamicBody(b2Vec2 position, float angle, void *userData)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = pixelsToMeters(position);

        // Conversion en radians - utilisez la constante B2_pi de box2d/constants.h
        const float angleInRadians = angle * B2_pi / 180.0f;
        bodyDef.linearVelocity = {0.0f, 0.0f};
        bodyDef.angularVelocity = 0.0f;
        bodyDef.transform.q.s = sinf(angleInRadians);
        bodyDef.transform.q.c = cosf(angleInRadians);
        bodyDef.userData = userData;

        // Utilisation des propriétés correctes pour Box2D 3.0
        bodyDef.fixedRotation = false;
        bodyDef.isBullet = false;
        bodyDef.enableSleep = true;

        return b2CreateBody(m_worldId, &bodyDef);
    }

    b2ShapeId Box2DWrapper::addBoxFixture(b2BodyId bodyId, float width, float height,
                                          float density, float friction, float restitution,
                                          b2Vec2 offset)
    {
        if (!b2Body_IsValid(bodyId))
            return {0}; // Retourner un ShapeId invalide

        // Créer une définition de forme
        b2ShapeDef shapeDef;
        shapeDef.density = density;
        shapeDef.friction = friction;
        shapeDef.restitution = restitution;

        // Convertir la demi-largeur et la demi-hauteur en mètres
        float halfWidth = pixelsToMeters(width * 0.5f);
        float halfHeight = pixelsToMeters(height * 0.5f);

        // Box2D 3.0 utilise b2Hull au lieu de b2Polygon
        b2Hull hull;
        hull.points[0] = {-halfWidth + pixelsToMeters(offset.x), -halfHeight + pixelsToMeters(offset.y)};
        hull.points[1] = {halfWidth + pixelsToMeters(offset.x), -halfHeight + pixelsToMeters(offset.y)};
        hull.points[2] = {halfWidth + pixelsToMeters(offset.x), halfHeight + pixelsToMeters(offset.y)};
        hull.points[3] = {-halfWidth + pixelsToMeters(offset.x), halfHeight + pixelsToMeters(offset.y)};
        hull.count = 4;

        // Utiliser b2ComputeHull pour obtenir la version canonique du hull
        b2Hull canonicalHull;
        b2ComputeHull(&hull, &canonicalHull);

        // Créer un polygone à partir du hull
        b2Polygon polygon;
        b2CreatePolygonFromHull(&canonicalHull, &polygon);

        // Créer la forme
        return b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
    }

    b2ShapeId Box2DWrapper::addCircleFixture(b2BodyId bodyId, float radius,
                                             float density, float friction, float restitution,
                                             b2Vec2 offset)
    {
        if (!b2Body_IsValid(bodyId))
            return {0}; // Retourner un ShapeId invalide

        // Créer une définition de forme
        b2ShapeDef shapeDef;
        shapeDef.density = density;
        shapeDef.friction = friction;
        shapeDef.restitution = restitution;

        // Créer un cercle - corriger la création pour Box2D 3.0
        b2Circle circle;
        circle.center = pixelsToMeters(offset);
        circle.radius = pixelsToMeters(radius);

        // Créer la forme
        return b2CreateCircleShape(bodyId, &shapeDef, &circle);
    }

    void Box2DWrapper::setFilterData(b2ShapeId shapeId, uint16_t categoryBits, uint16_t maskBits, int16_t groupIndex)
    {
        if (!b2Shape_IsValid(shapeId))
            return;

        b2Filter filter;
        filter.categoryBits = categoryBits;
        filter.maskBits = maskBits;
        filter.groupIndex = groupIndex;

        b2Shape_SetFilterData(shapeId, filter);
    }

    void Box2DWrapper::setFilterData(b2ShapeId shapeId, CollisionCategory category,
                                     CollisionCategory mask, int16_t groupIndex)
    {
        setFilterData(shapeId,
                      static_cast<uint16_t>(category),
                      static_cast<uint16_t>(mask),
                      groupIndex);
    }

    void Box2DWrapper::setSensor(b2ShapeId shapeId, bool isSensor)
    {
        if (b2Shape_IsValid(shapeId))
        {
            b2ShapeDef shapeDef;
            shapeDef.isSensor = isSensor;
            b2Shape_SetSensor(shapeId, isSensor);
        }
    }

    void Box2DWrapper::setLinearVelocity(b2BodyId bodyId, b2Vec2 velocity)
    {
        if (b2Body_IsValid(bodyId))
        {
            b2Body_SetLinearVelocity(bodyId, pixelsToMeters(velocity));
        }
    }

    void Box2DWrapper::applyForce(b2BodyId bodyId, b2Vec2 force, bool wake)
    {
        if (b2Body_IsValid(bodyId))
        {
            b2Body_ApplyForceToCenter(bodyId, pixelsToMeters(force), wake);
        }
    }

    void Box2DWrapper::destroyBody(b2BodyId bodyId)
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    void Box2DWrapper::setDebugDraw(bool enabled)
    {
        m_debugDrawEnabled = enabled;
    }

    void Box2DWrapper::debugDraw(sf::RenderTarget &target, sf::RenderStates states)
    {
        if (!m_debugDrawEnabled || !b2World_IsValid(m_worldId))
            return;

        // Obtenir toutes les formes du monde
        // Parcourir tous les corps du monde Box2D
        for (b2BodyId bodyId = b2World_GetFirstBody(m_worldId);
             b2Body_IsValid(bodyId);
             bodyId = b2Body_GetNext(bodyId))
        {
            // Obtenir la position et l'angle du corps
            b2Vec2 bodyPosition = b2Body_GetPosition(bodyId);
            float bodyAngle = b2Body_GetAngle(bodyId);

            // Convertir en coordonnées en pixels
            sf::Vector2f sfmlPosition(metersToPixels(bodyPosition.x), metersToPixels(bodyPosition.y));

            // Parcourir toutes les formes du corps
            for (b2ShapeId shapeId = b2Body_GetFirstShape(bodyId);
                 b2Shape_IsValid(shapeId);
                 shapeId = b2Shape_GetNext(shapeId))
            {
                // Déterminer le type de forme
                b2ShapeType shapeType = b2Shape_GetType(shapeId);

                // Couleur basée sur le type de corps
                sf::Color color;
                b2BodyType bodyType = b2Body_GetType(bodyId);
                if (bodyType == b2_staticBody)
                    color = sf::Color(0, 255, 0, 128); // Vert pour les corps statiques
                else if (bodyType == b2_dynamicBody)
                    color = sf::Color(255, 0, 0, 128); // Rouge pour les corps dynamiques
                else
                    color = sf::Color(0, 0, 255, 128); // Bleu pour les corps cinématiques

                // Dessiner en fonction du type de forme
                if (shapeType == b2ShapeType::e_circle)
                {
                    // Obtenir les données du cercle
                    b2CircleShape *circle = b2Shape_GetCircle(shapeId);
                    float radius = metersToPixels(circle->radius);

                    // Créer un cercle SFML
                    sf::CircleShape sfmlCircle(radius);
                    sfmlCircle.setOrigin(radius, radius);
                    sfmlCircle.setPosition(sfmlPosition);
                    sfmlCircle.setRotation(bodyAngle * 180.0f / B2_pi);
                    sfmlCircle.setFillColor(sf::Color::Transparent);
                    sfmlCircle.setOutlineColor(color);
                    sfmlCircle.setOutlineThickness(1.0f);

                    // Dessiner le cercle
                    target.draw(sfmlCircle, states);
                }
                else if (shapeType == b2ShapeType::e_polygon)
                {
                    // Obtenir les données du polygone
                    b2PolygonShape *polygon = b2Shape_GetPolygon(shapeId);
                    int vertexCount = polygon->count;

                    // Créer un polygone SFML
                    sf::ConvexShape convex(vertexCount);

                    for (int i = 0; i < vertexCount; ++i)
                    {
                        b2Vec2 vertex = polygon->vertices[i];

                        // Appliquer la rotation et la translation du corps
                        b2Rot rot(bodyAngle);
                        b2Vec2 translatedVertex = bodyPosition + b2Mul(rot, vertex);

                        // Convertir en coordonnées SFML
                        sf::Vector2f sfmlVertex(metersToPixels(translatedVertex.x),
                                                metersToPixels(translatedVertex.y));
                        convex.setPoint(i, sfmlVertex);
                    }

                    convex.setFillColor(sf::Color::Transparent);
                    convex.setOutlineColor(color);
                    convex.setOutlineThickness(1.0f);

                    // Dessiner le polygone
                    target.draw(convex, states);
                }
            }
        }
    }

    float Box2DWrapper::pixelsToMeters(float pixels)
    {
        return pixels * METERS_PER_PIXEL;
    }

    float Box2DWrapper::metersToPixels(float meters)
    {
        return meters * PIXELS_PER_METER;
    }

    b2Vec2 Box2DWrapper::pixelsToMeters(b2Vec2 pixels)
    {
        return {pixelsToMeters(pixels.x), pixelsToMeters(pixels.y)};
    }

    b2Vec2 Box2DWrapper::metersToPixels(b2Vec2 meters)
    {
        return {metersToPixels(meters.x), metersToPixels(meters.y)};
    }

    b2WorldId Box2DWrapper::getWorld() const
    {
        return m_worldId;
    }

} // namespace Physics