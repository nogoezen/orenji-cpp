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
        return b2Rot_GetAngle(b2Body_GetTransform(bodyId).q);
    }

    // Définit si un corps est en rotation fixe
    inline void SetBodyFixedRotation(b2BodyId bodyId, bool fixed)
    {
        b2Body_SetFixedRotation(bodyId, fixed);
    }

    // Définit si un corps est traité comme un projectile
    inline void SetBodyBullet(b2BodyId bodyId, bool bullet)
    {
        b2Body_SetBullet(bodyId, bullet);
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

    // Gestion des données utilisateur pour Box2D 3
    inline void SetBodyUserData(b2BodyId bodyId, void *userData)
    {
        // Dans Box2D 3, on utilise b2Body_SetUserData avec b2BodyUserData
        b2BodyUserData data;
        data.pointer = reinterpret_cast<uintptr_t>(userData);
        b2Body_SetUserData(bodyId, data);
    }

    inline void *GetBodyUserData(b2BodyId bodyId)
    {
        // Dans Box2D 3, on utilise b2Body_GetUserData pour récupérer b2BodyUserData
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
            bodyA = b2Contact_GetBodyA(contactId);
            bodyB = b2Contact_GetBodyB(contactId);
            fixtureA = b2Contact_GetFixtureA(contactId);
            fixtureB = b2Contact_GetFixtureB(contactId);
            isSensor = b2Fixture_IsSensor(fixtureA) || b2Fixture_IsSensor(fixtureB);
        }
    };
}