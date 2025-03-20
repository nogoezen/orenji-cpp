#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/Core/Entity.hpp"
#include "../../include/Core/Component.hpp"
#include "../../include/Core/TransformComponent.hpp"
#include "../../include/Physics/PhysicsWorld.hpp"
#include "../../include/Physics/Box2DTypes.hpp"
#include <iostream>

namespace Orenji
{

    PhysicsComponent::PhysicsComponent(Entity *entity, b2BodyType bodyType)
        : Component(entity),
          m_body{b2_nullBodyId},
          m_bodyType(bodyType),
          m_initialized(false),
          m_fixedRotation(false),
          m_isBullet(false),
          m_userData{nullptr},
          m_fixtures{}
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
        if (IsValid(m_body))
        {
            PhysicsWorld::getInstance().destroyBody(m_body);
            m_body = b2_nullBodyId;
        }
    }

    void PhysicsComponent::initialize()
    {
        if (m_initialized)
            return;

        Entity *entity = getOwner();
        if (!entity)
            return;

        auto &physicsWorld = PhysicsWorld::getInstance();
        m_body = physicsWorld.createBody(entity->getTransform().getPosition(), m_bodyType);

        if (IsValid(m_body))
        {
            b2Body_SetFixedRotation(m_body, m_fixedRotation);
            b2Body_SetBullet(m_body, m_isBullet);
            m_initialized = true;
        }
    }

    void PhysicsComponent::update(float deltaTime)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        Entity *entity = getOwner();
        if (!entity)
            return;

        auto &transform = entity->getTransform();
        auto &position = transform.getPosition();
        auto physicsPosition = getPosition();

        // Si la position a été modifiée directement via Transform, mise à jour de la physique
        if (position != physicsPosition)
        {
            setPosition(position);
        }
        else // Sinon, mise à jour de la position de l'entité à partir du corps physique
        {
            // Mise à jour de la position et de la rotation de l'entité
            float angle = b2Body_GetAngle(m_body) * 180.0f / b2_pi; // Convertir de radians à degrés
            transform.setPosition(physicsPosition);
            transform.setRotation(angle);
        }
    }

    b2FixtureId PhysicsComponent::createBoxFixture(const sf::Vector2f &size, float density,
                                                   float friction, uint16_t categoryBits,
                                                   uint16_t maskBits, bool isSensor)
    {
        if (!IsValid(m_body) || !m_initialized)
            return b2_nullFixtureId;

        b2FixtureId fixture = PhysicsWorld::getInstance().addBoxFixture(m_body, size, density, friction, categoryBits, maskBits, isSensor);
        if (IsValid(fixture))
        {
            m_fixtures.push_back(fixture);
        }
        return fixture;
    }

    b2FixtureId PhysicsComponent::createCircleFixture(float radius, float density,
                                                      float friction, uint16_t categoryBits,
                                                      uint16_t maskBits, bool isSensor)
    {
        if (!IsValid(m_body) || !m_initialized)
            return b2_nullFixtureId;

        b2FixtureId fixture = PhysicsWorld::getInstance().addCircleFixture(m_body, radius, density, friction, categoryBits, maskBits, isSensor);
        if (IsValid(fixture))
        {
            m_fixtures.push_back(fixture);
        }
        return fixture;
    }

    void PhysicsComponent::applyForce(const sf::Vector2f &force, const sf::Vector2f &point)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        // Conversion des vecteurs SFML en vecteurs Box2D
        b2Vec2 b2Force;
        b2Force.x = force.x;
        b2Force.y = force.y;

        b2Vec2 worldPoint;
        worldPoint.x = point.x;
        worldPoint.y = point.y;

        b2Body_ApplyForce(m_body, b2Force, worldPoint, true);
    }

    void PhysicsComponent::applyForceToCenter(const sf::Vector2f &force)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 b2Force;
        b2Force.x = force.x;
        b2Force.y = force.y;

        b2Body_ApplyForceToCenter(m_body, b2Force, true);
    }

    void PhysicsComponent::applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 b2Impulse;
        b2Impulse.x = impulse.x;
        b2Impulse.y = impulse.y;

        b2Vec2 worldPoint;
        worldPoint.x = point.x;
        worldPoint.y = point.y;

        b2Body_ApplyLinearImpulse(m_body, b2Impulse, worldPoint, true);
    }

    void PhysicsComponent::applyAngularImpulse(float impulse)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Body_ApplyAngularImpulse(m_body, impulse, true);
    }

    sf::Vector2f PhysicsComponent::getPosition() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return sf::Vector2f();

        b2Vec2 pos = b2Body_GetPosition(m_body);
        return PhysicsWorld::metersToPixels(pos);
    }

    void PhysicsComponent::setPosition(const sf::Vector2f &position)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 b2Position = PhysicsWorld::pixelsToMeters(position);

        b2Body_SetTransform(m_body, b2Position, b2Body_GetAngle(m_body));
    }

    float PhysicsComponent::getAngle() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return 0.0f;

        float angle = b2Body_GetAngle(m_body) * 180.0f / b2_pi; // Convertir de radians à degrés
        return angle;
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 pos = b2Body_GetPosition(m_body);
        float radians = angle * b2_pi / 180.0f; // Convertir de degrés à radians

        b2Body_SetTransform(m_body, pos, radians);
    }

    sf::Vector2f PhysicsComponent::getLinearVelocity() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return sf::Vector2f();

        b2Vec2 vel = b2Body_GetLinearVelocity(m_body);
        return sf::Vector2f(vel.x * PhysicsWorld::PIXELS_PER_METER, vel.y * PhysicsWorld::PIXELS_PER_METER);
    }

    void PhysicsComponent::setLinearVelocity(const sf::Vector2f &velocity)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 b2Velocity;
        b2Velocity.x = velocity.x * PhysicsWorld::METERS_PER_PIXEL;
        b2Velocity.y = velocity.y * PhysicsWorld::METERS_PER_PIXEL;

        b2Body_SetLinearVelocity(m_body, b2Velocity);
    }

    float PhysicsComponent::getAngularVelocity() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return 0.0f;

        return b2Body_GetAngularVelocity(m_body) * 180.0f / b2_pi; // Convertir de radians/s à degrés/s
    }

    void PhysicsComponent::setAngularVelocity(float velocity)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        float radiansPerSec = velocity * b2_pi / 180.0f; // Convertir de degrés/s à radians/s
        b2Body_SetAngularVelocity(m_body, radiansPerSec);
    }

    void PhysicsComponent::setBodyType(b2BodyType type)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        m_bodyType = type;
        b2Body_SetType(m_body, type);
    }

    b2BodyType PhysicsComponent::getBodyType() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return m_bodyType; // Retourner le type stocké si le corps n'est pas initialisé

        return b2Body_GetType(m_body);
    }

    void PhysicsComponent::setFixedRotation(bool fixed)
    {
        m_fixedRotation = fixed;

        if (IsValid(m_body) && m_initialized)
        {
            b2Body_SetFixedRotation(m_body, fixed);
        }
    }

    bool PhysicsComponent::isFixedRotation() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return m_fixedRotation;

        return b2Body_IsFixedRotation(m_body);
    }

    void PhysicsComponent::setBullet(bool bullet)
    {
        m_isBullet = bullet;

        if (IsValid(m_body) && m_initialized)
        {
            b2Body_SetBullet(m_body, bullet);
        }
    }

    bool PhysicsComponent::isBullet() const
    {
        if (!IsValid(m_body) || !m_initialized)
            return m_isBullet;

        return b2Body_IsBullet(m_body);
    }

    b2BodyId PhysicsComponent::getBody() const
    {
        return m_body;
    }

} // namespace Orenji