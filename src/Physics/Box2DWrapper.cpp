#include "../../include/Physics/Box2DWrapper.hpp"
#include <cmath>
#include <box2d/constants.h>

namespace Physics
{
    Box2DWrapper::Box2DWrapper(b2Vec2 gravity)
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

    void Box2DWrapper::destroyBody(b2BodyId bodyId)
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
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