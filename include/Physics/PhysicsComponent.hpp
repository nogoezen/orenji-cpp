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
         * @param id L'identifiant du composant.
         */
        PhysicsComponent(const std::string &id);

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
         * @param dt Le temps écoulé depuis la dernière mise à jour.
         */
        virtual void update(float dt) override;

        /**
         * @brief Crée le corps physique.
         *
         * Cette méthode est appelée pendant l'initialisation.
         */
        void createBody();

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
         * @brief Définit la valeur de l'amortissement linéaire.
         *
         * @param damping Valeur de l'amortissement.
         */
        void setLinearDamping(float damping);

        /**
         * @brief Obtient la valeur actuelle de l'amortissement linéaire.
         *
         * @return Valeur de l'amortissement linéaire.
         */
        float getLinearDamping() const;

        /**
         * @brief Définit la valeur de l'amortissement angulaire.
         *
         * @param damping Valeur de l'amortissement.
         */
        void setAngularDamping(float damping);

        /**
         * @brief Obtient la valeur actuelle de l'amortissement angulaire.
         *
         * @return Valeur de l'amortissement angulaire.
         */
        float getAngularDamping() const;

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
         * @brief Active ou désactive le corps physique.
         *
         * @param enabled Si true, le corps est actif.
         */
        void setEnabled(bool enabled);

        /**
         * @brief Vérifie si le corps est actif.
         *
         * @return true si le corps est actif.
         */
        bool isEnabled() const;

        /**
         * @brief Définit l'entité propriétaire de ce composant.
         *
         * @param owner Pointeur vers l'entité propriétaire.
         */
        void setOwner(Entity *owner);

        /**
         * @brief Obtient l'entité propriétaire de ce composant.
         *
         * @return Pointeur vers l'entité propriétaire.
         */
        Entity *getOwner() const;

        /**
         * @brief Obtient l'ID du corps Box2D.
         *
         * @return L'ID du corps Box2D.
         */
        b2BodyId getBody() const;

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
            if (IsValid(m_body))
            {
                SetBodyUserData(m_body, m_userData);
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
            if (IsValid(m_body))
            {
                return static_cast<T *>(GetBodyUserData(m_body));
            }
            return static_cast<T *>(m_userData);
        }

    private:
        b2BodyId m_body;        ///< L'ID du corps Box2D.
        b2BodyType m_bodyType;  ///< Le type de corps.
        float m_linearDamping;  ///< Amortissement linéaire du corps.
        float m_angularDamping; ///< Amortissement angulaire du corps.
        bool m_fixedRotation;   ///< Indique si la rotation est fixée.
        bool m_bullet;          ///< Indique si le corps est traité comme un projectile.
        bool m_enabled;         ///< Indique si le corps est actif.
        void *m_userData;       ///< Données utilisateur associées au corps.
        Entity *m_owner;        ///< Entité propriétaire de ce composant.
    };

} // namespace Orenji