#pragma once
#include "../Core/Component.hpp"
#include <SFML/Graphics.hpp>
#include "Box2DTypes.hpp"
#include "PhysicsWorld.hpp"
#include <box2d/box2d.h>

namespace Orenji
{
    // Définition des catégories de collision
    enum class CollisionCategory
    {
        NONE = 0x0000,
        DEFAULT = 0x0001,
        PLAYER = 0x0002,
        ENEMY = 0x0004,
        PROJECTILE = 0x0008,
        SENSOR = 0x0010,
        PLATFORM = 0x0020,
        ALL = 0xFFFF
    };

    class PhysicsComponent : public Component
    {
    public:
        PhysicsComponent(b2BodyType type = b2_dynamicBody,
                         bool fixedRotation = false,
                         bool bullet = false);
        virtual ~PhysicsComponent();

        virtual void initialize() override;
        virtual void update(float deltaTime) override;

        // Créer différents types de fixtures
        b2Fixture *createBoxFixture(const sf::Vector2f &size, float density = 1.0f,
                                    float friction = 0.3f, float restitution = 0.1f,
                                    uint16_t categoryBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                    uint16_t maskBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                    bool isSensor = false);

        b2Fixture *createCircleFixture(float radius, float density = 1.0f,
                                       float friction = 0.3f, float restitution = 0.1f,
                                       uint16_t categoryBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                       uint16_t maskBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                       bool isSensor = false);

        // Appliquer des forces et des impulsions
        void applyForce(const sf::Vector2f &force, const sf::Vector2f &point = sf::Vector2f(0, 0));
        void applyForceToCenter(const sf::Vector2f &force);
        void applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point = sf::Vector2f(0, 0));
        void applyAngularImpulse(float impulse);

        // Obtenir et définir les propriétés du corps
        sf::Vector2f getPosition() const;
        void setPosition(const sf::Vector2f &position);

        float getAngle() const;
        void setAngle(float angle);

        sf::Vector2f getLinearVelocity() const;
        void setLinearVelocity(const sf::Vector2f &velocity);

        float getAngularVelocity() const;
        void setAngularVelocity(float velocity);

        // Accès au corps Box2D
        b2Body *getBody() const;

        // Gestion des données utilisateur
        template <typename T>
        void setUserData(T *userData)
        {
            if (m_body)
            {
                m_body->SetUserData(static_cast<void *>(userData));
            }
        }

        template <typename T>
        T *getUserData() const
        {
            if (m_body)
            {
                return static_cast<T *>(m_body->GetUserData());
            }
            return nullptr;
        }

    private:
        b2Body *m_body;
        b2BodyType m_bodyType;
        bool m_fixedRotation;
        bool m_bullet;
        bool m_initialized;
    };

} // namespace Orenji