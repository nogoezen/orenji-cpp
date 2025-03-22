#pragma once

#include <SFML/Graphics.hpp>

namespace Orenji
{
    /**
     * @brief Type de déclencheur de particules
     */
    enum class ParticleTriggerType
    {
        Continuous, // Émission continue
        OneShot,    // Émission unique (un burst)
        OnCommand,  // Émission sur commande
        OnDistance  // Émission en fonction de la distance parcourue
    };

    /**
     * @brief Structure représentant une particule simple
     */
    struct SimpleParticle
    {
        sf::Vector2f position; // Position
        sf::Vector2f velocity; // Vitesse
        sf::Color color;       // Couleur
        float lifetime;        // Durée de vie maximale
        float elapsed;         // Temps écoulé
        float size;            // Taille
        float rotation;        // Rotation
        float rotationSpeed;   // Vitesse de rotation
    };

    /**
     * @brief Structure représentant les paramètres d'émission de particules
     */
    struct EmissionParameters
    {
        float emissionRate = 30.0f;
        float minLifetime = 1.0f;
        float maxLifetime = 3.0f;
        float minSize = 2.0f;
        float maxSize = 5.0f;
        float minSpeed = 20.0f;
        float maxSpeed = 50.0f;
        float minRotationSpeed = -180.0f;
        float maxRotationSpeed = 180.0f;
        sf::Color startColor = sf::Color::White;
        sf::Color endColor = sf::Color(255, 255, 255, 0);
        sf::Vector2f positionOffset = sf::Vector2f(0.0f, 0.0f);
        float positionRadius = 2.0f;
        sf::Vector2f baseVelocity = sf::Vector2f(0.0f, -30.0f);
        float velocitySpread = 10.0f;
    };

    /**
     * @brief Type d'affecteur pour les particules
     */
    enum class AffectorType
    {
        GRAVITY,  ///< Applique une gravité
        FADE,     ///< Fait disparaître progressivement
        SCALE,    ///< Change la taille
        COLOR,    ///< Change la couleur
        ROTATION, ///< Fait tourner la particule
        DRAG,     ///< Applique une résistance de l'air
        VORTEX    ///< Crée un effet de tourbillon
    };
}