#pragma once
#include "../Core/Component.hpp"
#include <SFML/Graphics.hpp>
#include "Box2DTypes.hpp"
#include "PhysicsWorld.hpp"
#include <box2d/box2d.h>
#include <vector>

namespace Orenji
{
    class Entity;

    /**
     * @brief Composant physique pour les entités, basé sur Box2D.
     *
     * Ce composant gère l'interaction physique d'une entité avec le monde.
     * Il permet l'application de forces, d'impulsions, et la gestion des collisions.
     */
    class PhysicsComponent : public Component
    {
    public:
        /**
         * @brief Constructeur du composant physique.
         *
         * @param entity L'entité à laquelle ce composant est attaché.
         * @param bodyType Le type de corps (b2_dynamicBody, b2_staticBody, b2_kinematicBody).
         */
        PhysicsComponent(Entity *entity, b2BodyType bodyType = b2_dynamicBody);

        /**
         * @brief Destructeur du composant physique.
         *
         * Détruit le corps Box2D associé.
         */
        virtual ~PhysicsComponent();

        /**
         * @brief Initialise le composant physique.
         *
         * Crée le corps Box2D et configure ses propriétés initiales.
         */
        virtual void initialize() override;

        /**
         * @brief Met à jour le composant.
         *
         * Synchronise la position de l'entité avec celle du corps Box2D.
         *
         * @param deltaTime Le temps écoulé depuis la dernière mise à jour.
         */
        virtual void update(float deltaTime) override;

        // --- Méthodes pour la création de fixtures ---

        /**
         * @brief Crée une fixture rectangulaire pour le corps.
         *
         * @param size Dimensions du rectangle (en pixels).
         * @param density Densité de la fixture.
         * @param friction Coefficient de friction.
         * @param categoryBits Bits de catégorie pour le filtrage des collisions.
         * @param maskBits Bits de masque pour le filtrage des collisions.
         * @param isSensor Si true, la fixture agit comme un capteur.
         * @return L'ID de la fixture créée.
         */
        b2FixtureId createBoxFixture(const sf::Vector2f &size, float density = 1.0f,
                                     float friction = 0.3f,
                                     uint16_t categoryBits = 0x0001,
                                     uint16_t maskBits = 0xFFFF,
                                     bool isSensor = false);

        /**
         * @brief Crée une fixture circulaire pour le corps.
         *
         * @param radius Rayon du cercle (en pixels).
         * @param density Densité de la fixture.
         * @param friction Coefficient de friction.
         * @param categoryBits Bits de catégorie pour le filtrage des collisions.
         * @param maskBits Bits de masque pour le filtrage des collisions.
         * @param isSensor Si true, la fixture agit comme un capteur.
         * @return L'ID de la fixture créée.
         */
        b2FixtureId createCircleFixture(float radius, float density = 1.0f,
                                        float friction = 0.3f,
                                        uint16_t categoryBits = 0x0001,
                                        uint16_t maskBits = 0xFFFF,
                                        bool isSensor = false);

        // --- Méthodes d'application de forces ---

        /**
         * @brief Applique une force au corps.
         *
         * @param force La force à appliquer.
         * @param point Le point d'application de la force (en pixels).
         */
        void applyForce(const sf::Vector2f &force, const sf::Vector2f &point);

        /**
         * @brief Applique une force au centre du corps.
         *
         * @param force La force à appliquer.
         */
        void applyForceToCenter(const sf::Vector2f &force);

        /**
         * @brief Applique une impulsion linéaire au corps.
         *
         * @param impulse L'impulsion à appliquer.
         * @param point Le point d'application de l'impulsion (en pixels).
         */
        void applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point);

        /**
         * @brief Applique une impulsion angulaire au corps.
         *
         * @param impulse L'impulsion angulaire à appliquer.
         */
        void applyAngularImpulse(float impulse);

        // --- Accesseurs de position et vitesse ---

        /**
         * @brief Obtient la position actuelle du corps (en pixels).
         *
         * @return La position du corps.
         */
        sf::Vector2f getPosition() const;

        /**
         * @brief Définit la position du corps (en pixels).
         *
         * @param position La nouvelle position.
         */
        void setPosition(const sf::Vector2f &position);

        /**
         * @brief Obtient l'angle de rotation du corps (en degrés).
         *
         * @return L'angle de rotation.
         */
        float getAngle() const;

        /**
         * @brief Définit l'angle de rotation du corps (en degrés).
         *
         * @param angle Le nouvel angle.
         */
        void setAngle(float angle);

        /**
         * @brief Obtient la vitesse linéaire du corps (en pixels/s).
         *
         * @return La vitesse linéaire.
         */
        sf::Vector2f getLinearVelocity() const;

        /**
         * @brief Définit la vitesse linéaire du corps (en pixels/s).
         *
         * @param velocity La nouvelle vitesse.
         */
        void setLinearVelocity(const sf::Vector2f &velocity);

        /**
         * @brief Obtient la vitesse angulaire du corps (en degrés/s).
         *
         * @return La vitesse angulaire.
         */
        float getAngularVelocity() const;

        /**
         * @brief Définit la vitesse angulaire du corps (en degrés/s).
         *
         * @param velocity La nouvelle vitesse angulaire.
         */
        void setAngularVelocity(float velocity);

        /**
         * @brief Définit le type du corps physique.
         *
         * @param type Le nouveau type de corps.
         */
        void setBodyType(b2BodyType type);

        /**
         * @brief Obtient le type du corps physique.
         *
         * @return Le type du corps.
         */
        b2BodyType getBodyType() const;

        /**
         * @brief Définit si le corps peut tourner ou non.
         *
         * @param fixed Si true, le corps ne peut pas tourner.
         */
        void setFixedRotation(bool fixed);

        /**
         * @brief Vérifie si le corps est fixé en rotation.
         *
         * @return true si le corps ne peut pas tourner.
         */
        bool isFixedRotation() const;

        /**
         * @brief Définit si le corps est un projectile (pour une meilleure détection de collision).
         *
         * @param bullet Si true, le corps est traité comme un projectile.
         */
        void setBullet(bool bullet);

        /**
         * @brief Vérifie si le corps est traité comme un projectile.
         *
         * @return true si le corps est un projectile.
         */
        bool isBullet() const;

        /**
         * @brief Obtient l'ID du corps Box2D.
         *
         * @return L'ID du corps Box2D.
         */
        b2BodyId getBody() const { return m_body; }

        /**
         * @brief Définit les données utilisateur associées au corps.
         *
         * @tparam T Le type des données utilisateur.
         * @param userData Les données à associer.
         */
        template <typename T>
        void setUserData(T *userData)
        {
            m_userData = static_cast<void *>(userData);
            if (b2IsValid(m_body))
            {
                b2Body_SetUserData(m_body, m_userData);
            }
        }

        /**
         * @brief Obtient les données utilisateur associées au corps.
         *
         * @tparam T Le type des données utilisateur à récupérer.
         * @return Les données utilisateur, castées au type demandé.
         */
        template <typename T>
        T *getUserData() const
        {
            if (b2IsValid(m_body))
            {
                return static_cast<T *>(b2Body_GetUserData(m_body));
            }
            return static_cast<T *>(m_userData);
        }

    private:
        b2BodyId m_body;                     ///< L'ID du corps Box2D.
        b2BodyType m_bodyType;               ///< Le type de corps.
        bool m_initialized;                  ///< Indique si le composant est initialisé.
        bool m_fixedRotation;                ///< Indique si la rotation est fixée.
        bool m_isBullet;                     ///< Indique si le corps est traité comme un projectile.
        void *m_userData;                    ///< Données utilisateur associées au corps.
        std::vector<b2FixtureId> m_fixtures; ///< Liste des fixtures attachées au corps.
    };

} // namespace Orenji