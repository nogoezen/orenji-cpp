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

    // Vérificateurs pour les IDs Box2D
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
}