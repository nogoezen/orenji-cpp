#pragma once

#include "Particles/Particle.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

namespace Orenji
{

    /**
     * @enum EmitterShape
     * @brief Forme de l'émetteur de particules
     */
    enum class EmitterShape
    {
        Point,     ///< Émission à partir d'un point
        Line,      ///< Émission le long d'une ligne
        Rectangle, ///< Émission dans un rectangle
        Circle,    ///< Émission dans un cercle
        Ring       ///< Émission le long d'un anneau
    };

    /**
     * @typedef ParticleAffector
     * @brief Fonction qui affecte une particule
     */
    using ParticleAffector = std::function<void(Particle &, float)>;

    /**
     * @class ParticleEmitter
     * @brief Gère l'émission et la configuration des particules
     */
    class ParticleEmitter
    {
    public:
        /**
         * @brief Constructeur
         * @param maxParticles Nombre maximum de particules
         */
        explicit ParticleEmitter(unsigned int maxParticles = 1000);

        /**
         * @brief Destructeur
         */
        ~ParticleEmitter();

        /**
         * @brief Émet des particules
         * @param count Nombre de particules à émettre
         */
        void emit(unsigned int count);

        /**
         * @brief Met à jour les particules
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

        /**
         * @brief Ajoute un affecteur de particules
         * @param affector Fonction d'affecteur
         */
        void addAffector(const ParticleAffector &affector);

        /**
         * @brief Supprime tous les affecteurs
         */
        void clearAffectors();

        /**
         * @brief Définit la position de l'émetteur
         * @param position Nouvelle position
         */
        void setPosition(const sf::Vector2f &position);

        /**
         * @brief Obtient la position de l'émetteur
         * @return Position de l'émetteur
         */
        const sf::Vector2f &getPosition() const;

        /**
         * @brief Définit la forme de l'émetteur
         * @param shape Nouvelle forme
         */
        void setShape(EmitterShape shape);

        /**
         * @brief Obtient la forme de l'émetteur
         * @return Forme de l'émetteur
         */
        EmitterShape getShape() const;

        /**
         * @brief Définit les dimensions de la forme de l'émetteur
         * @param size Nouvelles dimensions
         */
        void setSize(const sf::Vector2f &size);

        /**
         * @brief Obtient les dimensions de la forme de l'émetteur
         * @return Dimensions de la forme
         */
        const sf::Vector2f &getSize() const;

        /**
         * @brief Définit la durée de vie minimale des particules
         * @param minLife Durée de vie minimale
         */
        void setMinLife(float minLife);

        /**
         * @brief Obtient la durée de vie minimale des particules
         * @return Durée de vie minimale
         */
        float getMinLife() const;

        /**
         * @brief Définit la durée de vie maximale des particules
         * @param maxLife Durée de vie maximale
         */
        void setMaxLife(float maxLife);

        /**
         * @brief Obtient la durée de vie maximale des particules
         * @return Durée de vie maximale
         */
        float getMaxLife() const;

        /**
         * @brief Définit la taille initiale minimale des particules
         * @param minStartSize Taille initiale minimale
         */
        void setMinStartSize(float minStartSize);

        /**
         * @brief Obtient la taille initiale minimale des particules
         * @return Taille initiale minimale
         */
        float getMinStartSize() const;

        /**
         * @brief Définit la taille initiale maximale des particules
         * @param maxStartSize Taille initiale maximale
         */
        void setMaxStartSize(float maxStartSize);

        /**
         * @brief Obtient la taille initiale maximale des particules
         * @return Taille initiale maximale
         */
        float getMaxStartSize() const;

        /**
         * @brief Définit la taille finale minimale des particules
         * @param minEndSize Taille finale minimale
         */
        void setMinEndSize(float minEndSize);

        /**
         * @brief Obtient la taille finale minimale des particules
         * @return Taille finale minimale
         */
        float getMinEndSize() const;

        /**
         * @brief Définit la taille finale maximale des particules
         * @param maxEndSize Taille finale maximale
         */
        void setMaxEndSize(float maxEndSize);

        /**
         * @brief Obtient la taille finale maximale des particules
         * @return Taille finale maximale
         */
        float getMaxEndSize() const;

        /**
         * @brief Définit la vitesse minimale des particules
         * @param minVelocity Vitesse minimale
         */
        void setMinVelocity(float minVelocity);

        /**
         * @brief Obtient la vitesse minimale des particules
         * @return Vitesse minimale
         */
        float getMinVelocity() const;

        /**
         * @brief Définit la vitesse maximale des particules
         * @param maxVelocity Vitesse maximale
         */
        void setMaxVelocity(float maxVelocity);

        /**
         * @brief Obtient la vitesse maximale des particules
         * @return Vitesse maximale
         */
        float getMaxVelocity() const;

        /**
         * @brief Définit l'angle d'émission minimal
         * @param minAngle Angle minimal en degrés
         */
        void setMinAngle(float minAngle);

        /**
         * @brief Obtient l'angle d'émission minimal
         * @return Angle minimal en degrés
         */
        float getMinAngle() const;

        /**
         * @brief Définit l'angle d'émission maximal
         * @param maxAngle Angle maximal en degrés
         */
        void setMaxAngle(float maxAngle);

        /**
         * @brief Obtient l'angle d'émission maximal
         * @return Angle maximal en degrés
         */
        float getMaxAngle() const;

        /**
         * @brief Définit la vitesse de rotation minimale des particules
         * @param minRotation Vitesse de rotation minimale en degrés/seconde
         */
        void setMinRotationSpeed(float minRotation);

        /**
         * @brief Obtient la vitesse de rotation minimale des particules
         * @return Vitesse de rotation minimale en degrés/seconde
         */
        float getMinRotationSpeed() const;

        /**
         * @brief Définit la vitesse de rotation maximale des particules
         * @param maxRotation Vitesse de rotation maximale en degrés/seconde
         */
        void setMaxRotationSpeed(float maxRotation);

        /**
         * @brief Obtient la vitesse de rotation maximale des particules
         * @return Vitesse de rotation maximale en degrés/seconde
         */
        float getMaxRotationSpeed() const;

        /**
         * @brief Définit la couleur initiale des particules
         * @param color Couleur initiale
         */
        void setStartColor(const sf::Color &color);

        /**
         * @brief Obtient la couleur initiale des particules
         * @return Couleur initiale
         */
        const sf::Color &getStartColor() const;

        /**
         * @brief Définit la couleur finale des particules
         * @param color Couleur finale
         */
        void setEndColor(const sf::Color &color);

        /**
         * @brief Obtient la couleur finale des particules
         * @return Couleur finale
         */
        const sf::Color &getEndColor() const;

        /**
         * @brief Obtient le tableau de particules
         * @return Tableau de particules
         */
        const std::vector<Particle> &getParticles() const;

        /**
         * @brief Obtient le nombre de particules actives
         * @return Nombre de particules actives
         */
        unsigned int getActiveParticleCount() const;

    private:
        /**
         * @brief Initialise une particule
         * @param particle Particule à initialiser
         */
        void initializeParticle(Particle &particle);

        /**
         * @brief Obtient une position d'émission selon la forme de l'émetteur
         * @return Position d'émission
         */
        sf::Vector2f getEmissionPosition() const;

        std::vector<Particle> m_particles;         ///< Tableau de particules
        std::vector<ParticleAffector> m_affectors; ///< Tableau d'affecteurs

        sf::Vector2f m_position; ///< Position de l'émetteur
        sf::Vector2f m_size;     ///< Dimensions de la forme de l'émetteur
        EmitterShape m_shape;    ///< Forme de l'émetteur

        float m_minLife;          ///< Durée de vie minimale des particules
        float m_maxLife;          ///< Durée de vie maximale des particules
        float m_minStartSize;     ///< Taille initiale minimale des particules
        float m_maxStartSize;     ///< Taille initiale maximale des particules
        float m_minEndSize;       ///< Taille finale minimale des particules
        float m_maxEndSize;       ///< Taille finale maximale des particules
        float m_minVelocity;      ///< Vitesse minimale des particules
        float m_maxVelocity;      ///< Vitesse maximale des particules
        float m_minAngle;         ///< Angle d'émission minimal en degrés
        float m_maxAngle;         ///< Angle d'émission maximal en degrés
        float m_minRotationSpeed; ///< Vitesse de rotation minimale des particules
        float m_maxRotationSpeed; ///< Vitesse de rotation maximale des particules
        sf::Color m_startColor;   ///< Couleur initiale des particules
        sf::Color m_endColor;     ///< Couleur finale des particules
    };

} // namespace Orenji