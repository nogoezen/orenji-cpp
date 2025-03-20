#pragma once

// Inclure directement le header de Box2D
// Ce fichier sert uniquement à ajouter des constantes personnalisées
// ou des alias pour les types de Box2D

#include <box2d/box2d.h>

namespace Orenji
{
// Constante utilisée pour les conversions d'angle si Box2D ne la fournit pas
#ifndef b2_pi
    constexpr float b2_pi = 3.14159265359f;
#endif

    // Types de collisions
    enum class CollisionCategory
    {
        NONE = 0,
        PLAYER = 1 << 0,
        ENEMY = 1 << 1,
        OBSTACLE = 1 << 2,
        SENSOR = 1 << 3,
        PROJECTILE = 1 << 4,
        ITEM = 1 << 5,
        ALL = 0xFFFF
    };
}