#pragma once
#include "../Core/Component.hpp"
#include <SFML/Graphics.hpp>
#include "Box2DTypes.hpp"
#include "PhysicsWorld.hpp"
#include <box2d/box2d.h>

namespace Orenji
{
    // Types de collisions (défini dans PhysicsWorld.hpp également)
    enum class CollisionCategory
    {
        NONE = 0,
        PLAYER = 1 << 0,
        ENEMY = 1 << 1,
        OBSTACLE = 1 << 2,
        SENSOR = 1 << 3,
        PROJECTILE = 1 << 4,
        ITEM = 1 << 5,
        ALL = 0xFFFF
    };

    class PhysicsComponent : public Component
    {
    public:
        // Constructeur/Destructeur
        PhysicsComponent(Entity *entity);
        virtual ~PhysicsComponent();

        // Initialiser le composant physique
        virtual void initialize() override;

        // Mettre à jour les positions
        virtual void update(float deltaTime) override;

        // Gestion du type de corps
        void setBodyType(b2BodyType type);
        b2BodyType getBodyType() const;

        // Gestion des propriétés physiques
        void setFixedRotation(bool fixed);
        bool isFixedRotation() const;

        void setBullet(bool bullet);
        bool isBullet() const;

        // Application de forces et d'impulsions
        void applyForce(const sf::Vector2f &force, const sf::Vector2f &point);
        void applyForceToCenter(const sf::Vector2f &force);
        void applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point);
        void applyAngularImpulse(float impulse);

        // Création de fixtures
        /**
         * @brief Crée une fixture de forme rectangulaire
         * @param size Dimensions du rectangle en pixels
         * @param density Densité de la fixture (masse volumique)
         * @param friction Coefficient de friction
         * @param restitution Coefficient de restitution (rebond)
         * @param categoryBits Bits de catégorie pour le filtrage des collisions
         * @param maskBits Bits de masque pour le filtrage des collisions
         * @param isSensor Si true, la fixture est un capteur (ne génère pas de collisions physiques)
         * @return Identifiant de la fixture créée
         */
        b2FixtureId createBoxFixture(const sf::Vector2f &size,
                                     float density = 1.0f, float friction = 0.3f, float restitution = 0.0f,
                                     uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF, bool isSensor = false);

        /**
         * @brief Crée une fixture de forme circulaire
         * @param radius Rayon du cercle en pixels
         * @param density Densité de la fixture (masse volumique)
         * @param friction Coefficient de friction
         * @param restitution Coefficient de restitution (rebond)
         * @param categoryBits Bits de catégorie pour le filtrage des collisions
         * @param maskBits Bits de masque pour le filtrage des collisions
         * @param isSensor Si true, la fixture est un capteur (ne génère pas de collisions physiques)
         * @return Identifiant de la fixture créée
         */
        b2FixtureId createCircleFixture(float radius,
                                        float density = 1.0f, float friction = 0.3f, float restitution = 0.0f,
                                        uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF, bool isSensor = false);

        // Accès au corps Box2D
        b2BodyId getBody() const { return m_body; }

        // Setters pour les données utilisateur du corps
        template <typename T>
        void setUserData(T *userData)
        {
            if (m_body)
            {
                // Version adaptée pour Box2D 2.4.x
                void *userDataPtr = static_cast<void *>(userData);
                b2BodySetUserData(m_body, userDataPtr);
            }
        }

        // Getters pour les données utilisateur du corps
        template <typename T>
        T *getUserData() const
        {
            if (m_body)
            {
                // Version adaptée pour Box2D 2.4.x
                void *userDataPtr = b2BodyGetUserData(m_body);
                return static_cast<T *>(userDataPtr);
            }
            return nullptr;
        }

        // Accès aux propriétés de position et vitesse
        sf::Vector2f getPosition() const;
        void setPosition(const sf::Vector2f &position);

        float getAngle() const;
        void setAngle(float angle);

        sf::Vector2f getLinearVelocity() const;
        void setLinearVelocity(const sf::Vector2f &velocity);

        float getAngularVelocity() const;
        void setAngularVelocity(float velocity);

    private:
        b2BodyId m_body;
        b2BodyType m_bodyType;
        bool m_fixedRotation;
        bool m_bullet;
        bool m_initialized;
    };

} // namespace Orenji