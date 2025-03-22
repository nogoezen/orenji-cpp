#pragma once

// Inclure directement le header de Box2D
// Ce fichier sert uniquement à ajouter des constantes personnalisées
// ou des alias pour les types de Box2D

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

namespace Orenji
{
    // Types de collisions
    enum class CollisionCategory
    {
        NONE = 0,
        PLAYER = 1 << 0,
        ENEMY = 1 << 1,
        PROJECTILE = 1 << 2,
        WALL = 1 << 3,
        SENSOR = 1 << 4,
        ITEM = 1 << 5,
        ALL = 0xFFFF
    };

    // Constantes pour Box2D
    namespace box2d
    {
        // Constante utilisée pour les conversions d'angle
        constexpr float b2_pi = 3.14159265359f;
    }

    // Fonctions d'aide pour vérifier la validité des identifiants Box2D
    inline bool IsValid(b2BodyId bodyId)
    {
        return bodyId.index != b2_nullBodyId.index;
    }

    inline bool IsValid(b2FixtureId fixtureId)
    {
        return fixtureId.index != b2_nullFixtureId.index;
    }

    inline bool IsValid(b2JointId jointId)
    {
        return jointId.index != b2_nullJointId.index;
    }

    inline bool IsValid(b2WorldId worldId)
    {
        return worldId.index != b2_nullWorldId.index;
    }

    inline bool IsValid(b2ShapeId shapeId)
    {
        return shapeId.index != b2_nullShapeId.index;
    }

    // Fonctions utilitaires pour l'API Box2D 3.x

    // Récupère l'angle d'un corps (en radians)
    inline float GetBodyAngle(b2BodyId bodyId)
    {
        return b2Body_GetTransform(bodyId).q.s;
    }

    // Définit si un corps est en rotation fixe
    inline void SetBodyFixedRotation(b2BodyId bodyId, bool fixed)
    {
        b2BodyDef bodyDef = b2Body_GetDefinition(bodyId);
        bodyDef.fixedRotation = fixed;
        b2Body_SetDefinition(bodyId, &bodyDef);
    }

    // Définit si un corps est traité comme un projectile
    inline void SetBodyBullet(b2BodyId bodyId, bool bullet)
    {
        b2BodyDef bodyDef = b2Body_GetDefinition(bodyId);
        bodyDef.bullet = bullet;
        b2Body_SetDefinition(bodyId, &bodyDef);
    }

    // Applique une force à un point
    inline void ApplyForce(b2BodyId bodyId, b2Vec2 force, b2Vec2 point, bool wake)
    {
        b2Body_ApplyForce(bodyId, force, point, wake);
    }

    // Applique une force au centre
    inline void ApplyForceToCenter(b2BodyId bodyId, b2Vec2 force, bool wake)
    {
        b2Body_ApplyForce(bodyId, force, b2Body_GetPosition(bodyId), wake);
    }

    // Gestion des données utilisateur
    inline void SetBodyUserData(b2BodyId bodyId, void *userData)
    {
        b2BodyUserData data;
        data.pointer = reinterpret_cast<uintptr_t>(userData);
        b2Body_SetUserData(bodyId, data);
    }

    inline void *GetBodyUserData(b2BodyId bodyId)
    {
        b2BodyUserData data = b2Body_GetUserData(bodyId);
        return reinterpret_cast<void *>(data.pointer);
    }

    // Fonction pour récupérer les fixtures d'un corps
    inline b2FixtureId GetBodyFixtureList(b2BodyId bodyId)
    {
        return b2Body_GetFirstFixture(bodyId);
    }

    // Fonction pour récupérer la fixture suivante
    inline b2FixtureId GetNextFixture(b2FixtureId fixtureId)
    {
        return b2Fixture_GetNext(fixtureId);
    }

    // Fonction d'aide pour récupérer les données de contact
    inline b2BodyId GetContactBodyA(b2ContactId contactId)
    {
        return b2Contact_GetBodyIdA(contactId);
    }

    inline b2BodyId GetContactBodyB(b2ContactId contactId)
    {
        return b2Contact_GetBodyIdB(contactId);
    }

    inline b2FixtureId GetContactFixtureA(b2ContactId contactId)
    {
        return b2Contact_GetFixtureIdA(contactId);
    }

    inline b2FixtureId GetContactFixtureB(b2ContactId contactId)
    {
        return b2Contact_GetFixtureIdB(contactId);
    }

    // Structure pour encapsuler les données de contact spécifiques à notre jeu
    struct ContactData
    {
        b2BodyId bodyA;
        b2BodyId bodyB;
        b2FixtureId fixtureA;
        b2FixtureId fixtureB;
        bool isSensor;

        // Constructeur à partir d'un contact Box2D
        ContactData(b2ContactId contactId)
        {
            bodyA = GetContactBodyA(contactId);
            bodyB = GetContactBodyB(contactId);
            fixtureA = GetContactFixtureA(contactId);
            fixtureB = GetContactFixtureB(contactId);
            isSensor = b2Fixture_IsSensor(fixtureA) || b2Fixture_IsSensor(fixtureB);
        }
    };
}