#pragma once

#include "../Core/Component.hpp"
#include "Box2DWrapper.hpp"
#include <box2d/box2d.h>
#include <memory>

namespace Physics
{
    /**
     * @brief Type de corps physique
     */
    enum class BodyType
    {
        Static,
        Dynamic,
        Kinematic
    };

    /**
     * @brief Type de forme de collision
     */
    enum class ShapeType
    {
        Box,
        Circle,
        Polygon
    };

    /**
     * @brief Composant physique pour les entités
     */
    class PhysicsComponent : public Core::Component
    {
    public:
        /**
         * @brief Constructeur
         * @param physics Référence au système physique
         * @param position Position initiale en pixels
         * @param bodyType Type de corps
         */
        PhysicsComponent(Box2DWrapper &physics, const b2Vec2 &position, BodyType bodyType = BodyType::Dynamic);

        /**
         * @brief Destructeur
         */
        virtual ~PhysicsComponent();

        /**
         * @brief Initialiser le composant
         */
        virtual void init() override;

        /**
         * @brief Mettre à jour le composant
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Ajouter une forme de boîte au corps
         * @param width Largeur en pixels
         * @param height Hauteur en pixels
         * @param density Densité
         * @param friction Friction
         * @param restitution Restitution (rebond)
         * @param offset Décalage par rapport au centre du corps en pixels
         * @return Pointeur vers la fixture créée
         */
        b2Fixture *addBox(float width, float height,
                          float density = 1.0f, float friction = 0.3f,
                          float restitution = 0.0f, const b2Vec2 &offset = b2Vec2(0, 0));

        /**
         * @brief Ajouter une forme de cercle au corps
         * @param radius Rayon en pixels
         * @param density Densité
         * @param friction Friction
         * @param restitution Restitution (rebond)
         * @param offset Décalage par rapport au centre du corps en pixels
         * @return Pointeur vers la fixture créée
         */
        b2Fixture *addCircle(float radius,
                             float density = 1.0f, float friction = 0.3f,
                             float restitution = 0.0f, const b2Vec2 &offset = b2Vec2(0, 0));

        /**
         * @brief Obtenir la position du corps en pixels
         * @return Position en pixels
         */
        b2Vec2 getPosition() const;

        /**
         * @brief Obtenir l'angle du corps en degrés
         * @return Angle en degrés
         */
        float getAngle() const;

        /**
         * @brief Obtenir la vitesse linéaire du corps en pixels par seconde
         * @return Vitesse linéaire en pixels par seconde
         */
        b2Vec2 getLinearVelocity() const;

        /**
         * @brief Obtenir la vitesse angulaire du corps en degrés par seconde
         * @return Vitesse angulaire en degrés par seconde
         */
        float getAngularVelocity() const;

        /**
         * @brief Définir la position du corps en pixels
         * @param position Position en pixels
         */
        void setPosition(const b2Vec2 &position);

        /**
         * @brief Définir l'angle du corps en degrés
         * @param angle Angle en degrés
         */
        void setAngle(float angle);

        /**
         * @brief Définir la vitesse linéaire du corps en pixels par seconde
         * @param velocity Vitesse linéaire en pixels par seconde
         */
        void setLinearVelocity(const b2Vec2 &velocity);

        /**
         * @brief Définir la vitesse angulaire du corps en degrés par seconde
         * @param velocity Vitesse angulaire en degrés par seconde
         */
        void setAngularVelocity(float velocity);

        /**
         * @brief Appliquer une force au centre du corps en newtons
         * @param force Force en newtons
         * @param wake Réveiller le corps si nécessaire
         */
        void applyForce(const b2Vec2 &force, bool wake = true);

        /**
         * @brief Appliquer une force à un point du corps en newtons
         * @param force Force en newtons
         * @param point Point d'application en pixels
         * @param wake Réveiller le corps si nécessaire
         */
        void applyForceToPoint(const b2Vec2 &force, const b2Vec2 &point, bool wake = true);

        /**
         * @brief Appliquer une impulsion au centre du corps en kg·m/s
         * @param impulse Impulsion en kg·m/s
         * @param wake Réveiller le corps si nécessaire
         */
        void applyLinearImpulse(const b2Vec2 &impulse, bool wake = true);

        /**
         * @brief Appliquer une impulsion à un point du corps en kg·m/s
         * @param impulse Impulsion en kg·m/s
         * @param point Point d'application en pixels
         * @param wake Réveiller le corps si nécessaire
         */
        void applyLinearImpulseToPoint(const b2Vec2 &impulse, const b2Vec2 &point, bool wake = true);

        /**
         * @brief Appliquer un couple au corps en N·m
         * @param torque Couple en N·m
         * @param wake Réveiller le corps si nécessaire
         */
        void applyTorque(float torque, bool wake = true);

        /**
         * @brief Appliquer une impulsion angulaire au corps en kg·m²/s
         * @param impulse Impulsion angulaire en kg·m²/s
         * @param wake Réveiller le corps si nécessaire
         */
        void applyAngularImpulse(float impulse, bool wake = true);

        /**
         * @brief Définir si le corps est fixe en rotation
         * @param fixed Si le corps est fixe en rotation
         */
        void setFixedRotation(bool fixed);

        /**
         * @brief Définir si le corps est une balle (pour les collisions continues)
         * @param bullet Si le corps est une balle
         */
        void setBullet(bool bullet);

        /**
         * @brief Obtenir le corps Box2D
         * @return Pointeur vers le corps Box2D
         */
        b2Body *getBody();

    private:
        Box2DWrapper &m_physics;
        b2Body *m_body;
        b2Vec2 m_initialPosition;
        BodyType m_bodyType;
        bool m_initialized;
    };

} // namespace Physics