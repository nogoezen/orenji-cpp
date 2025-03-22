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

    // Aliases pour simplifier l'utilisation de Box2D v2.4.x
    namespace box2d
    {
        // Constante utilisée pour les conversions d'angle si Box2D ne la fournit pas
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
    
    // Fonction pour convertir un ancien pointeur b2Body* vers b2BodyId (utilisée pour la migration)
    // Cette fonction est temporaire et sera supprimée une fois la migration terminée
    inline b2BodyId ConvertToBodyId(void* bodyPtr)
    {
        // Cette implémentation est un placeholder, à remplacer par la bonne logique
        // dans le contexte de votre application
        return b2_nullBodyId;
    }
    
    // Fonctions utilitaires pour l'API Box2D 2.4.x
    
    // Récupère l'angle d'un corps (en radians)
    inline float b2Body_GetAngle(b2BodyId bodyId)
    {
        return b2Body_GetTransform(bodyId).q.s;
    }
    
    // Définit si un corps est en rotation fixe
    inline void b2Body_SetFixedRotation(b2BodyId bodyId, bool fixed)
    {
        b2BodyDef bodyDef = b2Body_GetDefinition(bodyId);
        bodyDef.fixedRotation = fixed;
        b2Body_SetDefinition(bodyId, &bodyDef);
    }
    
    // Définit si un corps est traité comme un projectile
    inline void b2Body_SetBullet(b2BodyId bodyId, bool bullet)
    {
        b2BodyDef bodyDef = b2Body_GetDefinition(bodyId);
        bodyDef.bullet = bullet;
        b2Body_SetDefinition(bodyId, &bodyDef);
    }
    
    // Accessoires pour les fonctions qui peuvent ne pas être directement disponibles dans l'API Box2D 2.4.x
    // Ces fonctions servent de wrapper pour faciliter la transition
    
    inline void b2Body_ApplyForce(b2BodyId bodyId, b2Vec2 force, b2Vec2 point, bool wake)
    {
        b2Body_ApplyForce(bodyId, force, point, wake);
    }
    
    inline void b2Body_ApplyForceToCenter(b2BodyId bodyId, b2Vec2 force, bool wake)
    {
        b2Body_ApplyForce(bodyId, force, b2Body_GetPosition(bodyId), wake);
    }
    
    inline void b2Body_SetUserData(b2BodyId bodyId, void* userData)
    {
        b2BodyUserData data;
        data.pointer = (uintptr_t)userData;
        b2Body_SetUserData(bodyId, data);
    }
    
    inline void* b2Body_GetUserData(b2BodyId bodyId)
    {
        b2BodyUserData data = b2Body_GetUserData(bodyId);
        return (void*)data.pointer;
    }
}