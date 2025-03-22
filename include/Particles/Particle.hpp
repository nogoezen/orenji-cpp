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
        sf::Vector2f position; ///< Position de la particule
        sf::Vector2f velocity; ///< Vitesse de la particule
        sf::Color color;       ///< Couleur de la particule
        sf::Color startColor;  ///< Couleur initiale de la particule
        sf::Color endColor;    ///< Couleur finale de la particule
        float life;            ///< Durée de vie restante de la particule
        float totalLife;       ///< Durée de vie totale de la particule
        float size;            ///< Taille de la particule
        float startSize;       ///< Taille initiale de la particule
        float endSize;         ///< Taille finale de la particule
        float rotation;        ///< Rotation de la particule
        float rotationSpeed;   ///< Vitesse de rotation de la particule
        bool active;           ///< Indique si la particule est active

        /**
         * @brief Constructeur par défaut
         */
        Particle();

        /**
         * @brief Met à jour la particule
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         * @return true si la particule est toujours active, false sinon
         */
        bool update(float deltaTime);

        /**
         * @brief Réinitialise la particule
         */
        void reset();
    };

} // namespace Orenji