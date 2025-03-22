#pragma once

#include <SFML/Graphics.hpp>

namespace Orenji
{

    /**
     * @struct Particle
     * @brief Structure représentant une particule
     */
    struct Particle
    {
        sf::Vector2f position; ///< Position actuelle
        sf::Vector2f velocity; ///< Vélocité (direction et vitesse)
        float rotation;        ///< Rotation en degrés
        float rotationSpeed;   ///< Vitesse de rotation en degrés/seconde
        float size;            ///< Taille actuelle
        float startSize;       ///< Taille initiale
        float endSize;         ///< Taille finale
        sf::Color color;       ///< Couleur actuelle
        sf::Color startColor;  ///< Couleur initiale
        sf::Color endColor;    ///< Couleur finale
        float life;            ///< Durée de vie restante
        float totalLife;       ///< Durée de vie totale
        bool active;           ///< Indique si la particule est active

        /**
         * @brief Constructeur par défaut
         */
        Particle()
            : position(0.f, 0.f), velocity(0.f, 0.f), rotation(0.f), rotationSpeed(0.f),
              size(1.f), startSize(1.f), endSize(0.f),
              color(sf::Color::White), startColor(sf::Color::White), endColor(sf::Color::Transparent),
              life(0.f), totalLife(0.f), active(false)
        {
        }

        /**
         * @brief Met à jour la particule
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         * @return true si la particule est toujours active, false sinon
         */
        bool update(float deltaTime)
        {
            if (!active)
            {
                return false;
            }

            // Met à jour la durée de vie
            life -= deltaTime;
            if (life <= 0.f)
            {
                active = false;
                return false;
            }

            // Calcule le facteur de progression (0 = début, 1 = fin)
            float lifeFactor = 1.f - (life / totalLife);

            // Met à jour la position
            position += velocity * deltaTime;

            // Met à jour la rotation
            rotation += rotationSpeed * deltaTime;
            while (rotation >= 360.f)
            {
                rotation -= 360.f;
            }
            while (rotation < 0.f)
            {
                rotation += 360.f;
            }

            // Interpolation linéaire de la taille
            size = startSize + (endSize - startSize) * lifeFactor;

            // Interpolation linéaire de la couleur
            color.r = startColor.r + static_cast<uint8_t>((endColor.r - startColor.r) * lifeFactor);
            color.g = startColor.g + static_cast<uint8_t>((endColor.g - startColor.g) * lifeFactor);
            color.b = startColor.b + static_cast<uint8_t>((endColor.b - startColor.b) * lifeFactor);
            color.a = startColor.a + static_cast<uint8_t>((endColor.a - startColor.a) * lifeFactor);

            return true;
        }

        /**
         * @brief Réinitialise la particule
         */
        void reset()
        {
            position = sf::Vector2f(0.f, 0.f);
            velocity = sf::Vector2f(0.f, 0.f);
            rotation = 0.f;
            rotationSpeed = 0.f;
            size = 1.f;
            startSize = 1.f;
            endSize = 0.f;
            color = sf::Color::White;
            startColor = sf::Color::White;
            endColor = sf::Color::Transparent;
            life = 0.f;
            totalLife = 0.f;
            active = false;
        }
    };

} // namespace Orenji