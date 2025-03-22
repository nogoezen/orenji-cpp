#include "Particles/Particle.hpp"

namespace Orenji
{

    Particle::Particle()
        : position(0.f, 0.f), velocity(0.f, 0.f), color(sf::Color::White), startColor(sf::Color::White), endColor(sf::Color::Transparent), life(0.f), totalLife(0.f), size(1.f), startSize(1.f), endSize(1.f), rotation(0.f), rotationSpeed(0.f), active(false)
    {
    }

    bool Particle::update(float deltaTime)
    {
        if (!active)
        {
            return false;
        }

        // Mise à jour de la durée de vie
        life -= deltaTime;
        if (life <= 0.f)
        {
            active = false;
            return false;
        }

        // Ratio de vie (0 = nouveau, 1 = mort)
        float lifeRatio = 1.f - (life / totalLife);

        // Interpolation linéaire de la couleur
        color.r = static_cast<sf::Uint8>(startColor.r + (endColor.r - startColor.r) * lifeRatio);
        color.g = static_cast<sf::Uint8>(startColor.g + (endColor.g - startColor.g) * lifeRatio);
        color.b = static_cast<sf::Uint8>(startColor.b + (endColor.b - startColor.b) * lifeRatio);
        color.a = static_cast<sf::Uint8>(startColor.a + (endColor.a - startColor.a) * lifeRatio);

        // Interpolation linéaire de la taille
        size = startSize + (endSize - startSize) * lifeRatio;

        // Mise à jour de la position
        position += velocity * deltaTime;

        // Mise à jour de la rotation
        rotation += rotationSpeed * deltaTime;

        return true;
    }

    void Particle::reset()
    {
        position = sf::Vector2f(0.f, 0.f);
        velocity = sf::Vector2f(0.f, 0.f);
        color = sf::Color::White;
        startColor = sf::Color::White;
        endColor = sf::Color::Transparent;
        life = 0.f;
        totalLife = 0.f;
        size = 1.f;
        startSize = 1.f;
        endSize = 1.f;
        rotation = 0.f;
        rotationSpeed = 0.f;
        active = false;
    }

} // namespace Orenji