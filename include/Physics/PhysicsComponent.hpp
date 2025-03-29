#pragma once

#include "../Core/Component.hpp"
#include "Box2DWrapper.hpp"
#include <box2d/box2d.h>
#include <memory>
#include <functional>

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
     * @brief Résultat d'une collision
     */
    struct CollisionResult
    {
        Core::EntityId entityA; // Première entité impliquée dans la collision
        Core::EntityId entityB; // Deuxième entité impliquée dans la collision
        bool isSensor;          // Si la collision concerne un capteur
        b2Vec2 contactPoint;    // Point de contact en pixels
        b2Vec2 normal;          // Normale de collision
        float impulse;          // Impulsion générée par la collision
    };

    /**
     * @brief Callback pour les événements de collision
     */
    using CollisionCallback = std::function<void(const CollisionResult &)>;

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
         * @param category Catégorie de collision
         * @param mask Masque de collision
         * @param isSensor Si la forme est un capteur
         * @return ID de la forme créée
         */
        b2ShapeId addBox(float width, float height,
                         float density = 1.0f, float friction = 0.3f,
                         float restitution = 0.0f, const b2Vec2 &offset = b2Vec2(0, 0),
                         CollisionCategory category = CollisionCategory::All,
                         CollisionCategory mask = CollisionCategory::All,
                         bool isSensor = false);

        /**
         * @brief Ajouter une forme de cercle au corps
         * @param radius Rayon en pixels
         * @param density Densité
         * @param friction Friction
         * @param restitution Restitution (rebond)
         * @param offset Décalage par rapport au centre du corps en pixels
         * @param category Catégorie de collision
         * @param mask Masque de collision
         * @param isSensor Si la forme est un capteur
         * @return ID de la forme créée
         */
        b2ShapeId addCircle(float radius,
                            float density = 1.0f, float friction = 0.3f,
                            float restitution = 0.0f, const b2Vec2 &offset = b2Vec2(0, 0),
                            CollisionCategory category = CollisionCategory::All,
                            CollisionCategory mask = CollisionCategory::All,
                            bool isSensor = false);

        /**
         * @brief Définir la catégorie et le masque de collision pour une forme
         * @param shapeId ID de la forme
         * @param category Catégorie de collision
         * @param mask Masque de collision
         * @param groupIndex Groupe de collision (valeurs négatives = ne pas entrer en collision avec le même groupe)
         */
        void setCollisionFilter(b2ShapeId shapeId, CollisionCategory category,
                                CollisionCategory mask = CollisionCategory::All,
                                int16_t groupIndex = 0);

        /**
         * @brief Définir si une forme est un capteur (ne génère pas de collisions physiques)
         * @param shapeId ID de la forme
         * @param isSensor Si la forme est un capteur
         */
        void setSensor(b2ShapeId shapeId, bool isSensor);

        /**
         * @brief Définir le callback pour les collisions
         * @param callback Fonction à appeler lors d'une collision
         */
        void setCollisionCallback(CollisionCallback callback);

        /**
         * @brief Gérer le début d'une collision
         * @param otherEntity Entité avec laquelle la collision a lieu
         * @param contact Informations sur le contact
         */
        void onCollisionBegin(Core::EntityId otherEntity, b2ContactId contact);

        /**
         * @brief Gérer la fin d'une collision
         * @param otherEntity Entité avec laquelle la collision a lieu
         * @param contact Informations sur le contact
         */
        void onCollisionEnd(Core::EntityId otherEntity, b2ContactId contact);

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

        /**
         * @brief Obtenir l'ID du corps Box2D
         * @return ID du corps Box2D
         */
        b2BodyId getBodyId() const;

    private:
        Box2DWrapper &m_physics;
        b2BodyId m_bodyId;
        b2Vec2 m_initialPosition;
        BodyType m_bodyType;
        bool m_initialized;
        CollisionCallback m_collisionCallback;
    };

} // namespace Physics