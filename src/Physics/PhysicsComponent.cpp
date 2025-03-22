#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/Core/Entity.hpp"
#include "../../include/Core/Component.hpp"
#include "../../include/Core/TransformComponent.hpp"
#include "../../include/Physics/PhysicsWorld.hpp"
#include "../../include/Physics/Box2DTypes.hpp"
#include <iostream>

namespace Orenji
{

    PhysicsComponent::PhysicsComponent(const std::string &id)
        : Component(id),
          m_body(b2_nullBodyId),
          m_bodyType(b2_dynamicBody),
          m_linearDamping(0.1f),
          m_angularDamping(0.1f),
          m_fixedRotation(false),
          m_bullet(false),
          m_userData(nullptr),
          m_enabled(true),
          m_owner(nullptr)
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
        if (!m_owner)
        {
            std::cerr << "PhysicsComponent::initialize() - No owner entity set!" << std::endl;
            return;
        }

        if (!IsValid(m_body))
        {
            createBody();

            if (m_userData != nullptr)
            {
                SetBodyUserData(m_body, m_userData);
            }
            else
            {
                SetBodyUserData(m_body, m_owner);
            }
        }
    }

    void PhysicsComponent::update(float dt)
    {
        (void)dt;

        if (!m_enabled || !IsValid(m_body) || !m_owner)
            return;

        sf::Transformable &transform = m_owner->getTransformable();
        sf::Vector2f position = PhysicsWorld::metersToPixels(b2Body_GetPosition(m_body));
        transform.setPosition(position);

        if (!m_fixedRotation)
        {
            float angle = GetBodyAngle(m_body) * 180.0f / box2d::b2_pi;
            transform.setRotation(sf::degrees(angle));
        }
    }

    b2FixtureId PhysicsComponent::createBoxFixture(const sf::Vector2f &size, float density,
                                                   float friction, uint16_t categoryBits,
                                                   uint16_t maskBits, bool isSensor)
    {
        if (!IsValid(m_body))
        {
            std::cerr << "PhysicsComponent::createBoxFixture() - No valid body!" << std::endl;
            return b2_nullFixtureId;
        }

        return PhysicsWorld::getInstance().addBoxFixture(m_body, size, density, friction, categoryBits, maskBits, isSensor);
    }

    b2FixtureId PhysicsComponent::createCircleFixture(float radius, float density,
                                                      float friction, uint16_t categoryBits,
                                                      uint16_t maskBits, bool isSensor)
    {
        if (!IsValid(m_body))
        {
            std::cerr << "PhysicsComponent::createCircleFixture() - No valid body!" << std::endl;
            return b2_nullFixtureId;
        }

        return PhysicsWorld::getInstance().addCircleFixture(m_body, radius, density, friction, categoryBits, maskBits, isSensor);
    }

    void PhysicsComponent::createBody()
    {
        if (IsValid(m_body))
        {
            std::cerr << "PhysicsComponent::createBody() - Body already exists!" << std::endl;
            return;
        }

        m_body = PhysicsWorld::getInstance().createBody(m_owner->getPosition(), m_bodyType);

        b2Body_SetLinearDamping(m_body, m_linearDamping);
        b2Body_SetAngularDamping(m_body, m_angularDamping);
        b2Body_SetFixedRotation(m_body, m_fixedRotation);
        b2Body_SetBullet(m_body, m_bullet);
    }

    void PhysicsComponent::setPosition(const sf::Vector2f &position)
    {
        if (IsValid(m_body))
        {
            b2Vec2 pos = PhysicsWorld::pixelsToMeters(position);
            float angle = GetBodyAngle(m_body);
            b2Body_SetTransform(m_body, pos, angle);

            if (m_owner)
            {
                m_owner->setPosition(position);
            }
        }
    }

    sf::Vector2f PhysicsComponent::getPosition() const
    {
        if (IsValid(m_body))
        {
            return PhysicsWorld::metersToPixels(b2Body_GetPosition(m_body));
        }
        return m_owner ? m_owner->getPosition() : sf::Vector2f(0.0f, 0.0f);
    }

    float PhysicsComponent::getAngle() const
    {
        if (IsValid(m_body))
        {
            return GetBodyAngle(m_body) * 180.0f / box2d::b2_pi;
        }
        return m_owner ? m_owner->getRotation() : 0.0f;
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (IsValid(m_body))
        {
            float rad = angle * box2d::b2_pi / 180.0f;

            b2Vec2 pos = b2Body_GetPosition(m_body);
            b2Body_SetTransform(m_body, pos, rad);

            if (m_owner)
            {
                m_owner->setRotation(angle);
            }
        }
    }

    void PhysicsComponent::applyForce(const sf::Vector2f &force, const sf::Vector2f &point)
    {
        if (IsValid(m_body))
        {
            b2Vec2 b2Force = PhysicsWorld::pixelsToMeters(force);
            b2Vec2 b2Point = PhysicsWorld::pixelsToMeters(point);

            ApplyForce(m_body, b2Force, b2Point, true);
        }
    }

    void PhysicsComponent::applyForceToCenter(const sf::Vector2f &force)
    {
        if (IsValid(m_body))
        {
            b2Vec2 b2Force = PhysicsWorld::pixelsToMeters(force);

            ApplyForceToCenter(m_body, b2Force, true);
        }
    }

    void PhysicsComponent::setLinearVelocity(const sf::Vector2f &velocity)
    {
        if (IsValid(m_body))
        {
            b2Vec2 b2Velocity = PhysicsWorld::pixelsToMeters(velocity);
            b2Body_SetLinearVelocity(m_body, b2Velocity);
        }
    }

    sf::Vector2f PhysicsComponent::getLinearVelocity() const
    {
        if (IsValid(m_body))
        {
            return PhysicsWorld::metersToPixels(b2Body_GetLinearVelocity(m_body));
        }
        return sf::Vector2f(0.0f, 0.0f);
    }

    void PhysicsComponent::setBodyType(b2BodyType type)
    {
        m_bodyType = type;

        if (IsValid(m_body))
        {
            b2Body_SetType(m_body, type);
        }
    }

    b2BodyType PhysicsComponent::getBodyType() const
    {
        if (IsValid(m_body))
        {
            return b2Body_GetType(m_body);
        }
        return m_bodyType;
    }

    void PhysicsComponent::setLinearDamping(float damping)
    {
        m_linearDamping = damping;

        if (IsValid(m_body))
        {
            b2Body_SetLinearDamping(m_body, damping);
        }
    }

    float PhysicsComponent::getLinearDamping() const
    {
        if (IsValid(m_body))
        {
            return b2Body_GetLinearDamping(m_body);
        }
        return m_linearDamping;
    }

    void PhysicsComponent::setAngularDamping(float damping)
    {
        m_angularDamping = damping;

        if (IsValid(m_body))
        {
            b2Body_SetAngularDamping(m_body, damping);
        }
    }

    float PhysicsComponent::getAngularDamping() const
    {
        if (IsValid(m_body))
        {
            return b2Body_GetAngularDamping(m_body);
        }
        return m_angularDamping;
    }

    void PhysicsComponent::setFixedRotation(bool fixed)
    {
        m_fixedRotation = fixed;

        if (IsValid(m_body))
        {
            SetBodyFixedRotation(m_body, fixed);
        }
    }

    bool PhysicsComponent::isFixedRotation() const
    {
        if (IsValid(m_body))
        {
            return b2Body_IsFixedRotation(m_body);
        }
        return m_fixedRotation;
    }

    void PhysicsComponent::setBullet(bool bullet)
    {
        m_bullet = bullet;

        if (IsValid(m_body))
        {
            SetBodyBullet(m_body, bullet);
        }
    }

    bool PhysicsComponent::isBullet() const
    {
        if (IsValid(m_body))
        {
            return b2Body_IsBullet(m_body);
        }
        return m_bullet;
    }

    void PhysicsComponent::setEnabled(bool enabled)
    {
        m_enabled = enabled;

        if (IsValid(m_body))
        {
            b2Body_SetEnabled(m_body, enabled);
        }
    }

    bool PhysicsComponent::isEnabled() const
    {
        if (IsValid(m_body))
        {
            return b2Body_IsEnabled(m_body);
        }
        return m_enabled;
    }

    void PhysicsComponent::setOwner(Entity *owner)
    {
        m_owner = owner;
    }

    Entity *PhysicsComponent::getOwner() const
    {
        return m_owner;
    }

    b2BodyId PhysicsComponent::getBody() const
    {
        return m_body;
    }

} // namespace Orenji