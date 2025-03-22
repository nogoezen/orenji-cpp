#pragma once

#include <SFML/Graphics.hpp>

namespace Orenji
{
    /**
     * @brief Types de déclenchement pour le système de particules
     */
    enum class ParticleTriggerType
    {
        Continuous, ///< Émission continue
        OneShot,    ///< Émission unique d'un lot
        OnDistance, ///< Émission basée sur distance parcourue
        OnPosition  ///< Émission quand à une position spécifique
    };

    /**
     * @brief Types d'affecteurs pour les particules
     */
    enum class AffectorType
    {
        Gravity,   ///< Force de gravité (vers le bas)
        Wind,      ///< Force horizontale (vent)
        Vortex,    ///< Force rotationnelle
        Attractor, ///< Attire les particules vers un point
        Repeller,  ///< Repousse les particules
        Scale,     ///< Modifie la taille
        ColorShift ///< Modifie la couleur
    };

    /**
     * @brief Structure de particule simple pour l'implémentation sans Thor
     */
    struct SimpleParticle
    {
        sf::Vector2f position; ///< Position
        sf::Vector2f velocity; ///< Vitesse
        sf::Color color;       ///< Couleur
        float lifetime;        ///< Durée de vie totale
        float elapsed;         ///< Temps écoulé
        float size;            ///< Taille
        float rotation;        ///< Rotation en degrés
        float rotationSpeed;   ///< Vitesse de rotation en degrés/sec
    };

    /**
     * @brief Paramètres d'émission pour le système de particules
     */
    struct EmissionParameters
    {
        float emissionRate;          ///< Taux d'émission (particules/sec)
        float minLifetime;           ///< Durée de vie minimale
        float maxLifetime;           ///< Durée de vie maximale
        float minSize;               ///< Taille minimale
        float maxSize;               ///< Taille maximale
        float minSpeed;              ///< Vitesse minimale
        float maxSpeed;              ///< Vitesse maximale
        float minRotationSpeed;      ///< Vitesse de rotation minimale (degrés/sec)
        float maxRotationSpeed;      ///< Vitesse de rotation maximale (degrés/sec)
        float positionRadius;        ///< Rayon du cercle d'émission
        float velocitySpread;        ///< Dispersion des angles de vitesse (radians)
        sf::Vector2f positionOffset; ///< Décalage de la position d'émission
        sf::Vector2f baseVelocity;   ///< Direction de base des particules
        sf::Color startColor;        ///< Couleur initiale
        sf::Color endColor;          ///< Couleur finale
    };

} // namespace Orenji