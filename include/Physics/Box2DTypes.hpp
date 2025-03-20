#pragma once

// Forward declarations pour les types Box2D
// Utiliser ce fichier quand vous avez besoin de référencer des types Box2D
// sans inclure tout le header box2d.h

// Dans Box2D 2.4.x, beaucoup de ces types sont désormais des handles ou des structures
// alors qu'ils étaient auparavant des classes

namespace box2d
{
    struct b2BodyDef;
    struct b2FixtureDef;
    struct b2JointDef;
    struct b2ShapeDef;
    struct b2Contact;
    struct b2Manifold;
    struct b2ContactImpulse;
    class b2ContactListener;
    struct b2Vec2;
    struct b2Filter;
}

// Types d'ID
using b2BodyId = void *;
using b2FixtureId = void *;
using b2JointId = void *;
using b2ShapeId = void *;
using b2WorldId = void *;

// Enum constants
enum b2BodyType
{
    b2_staticBody = 0,
    b2_kinematicBody,
    b2_dynamicBody
};

// Constante utilisée pour les conversions d'angle
constexpr float b2_pi = 3.14159265359f;