#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/Core/Entity.hpp"
#include <iostream>

namespace Orenji
{

    PhysicsComponent::PhysicsComponent(Entity *entity)
        : m_body(nullptr),
          m_bodyType(b2_dynamicBody),
          m_fixedRotation(false),
          m_bullet(false),
          m_initialized(false)
    {
        this->setOwner(entity);
    }

    PhysicsComponent::~PhysicsComponent()
    {
        // Si le composant est détruit, détruire aussi le corps Box2D
        if (m_body)
        {
            PhysicsWorld::getInstance().destroyBody(m_body);
            m_body = nullptr;
        }
    }

    void PhysicsComponent::initialize()
    {
        if (m_initialized)
            return;

        // Obtenir la position de l'entité
        Entity *entity = getOwner();
        if (!entity)
        {
            std::cerr << "Erreur: PhysicsComponent sans entité parente" << std::endl;
            return;
        }

        // Créer le corps Box2D
        sf::Vector2f position(0.0f, 0.0f);

        // Si l'entité a un composant de transformation, utiliser sa position
        auto transformComp = entity->getComponent<class TransformComponent>();
        if (transformComp)
        {
            position = transformComp->getPosition();
        }

        m_body = PhysicsWorld::getInstance().createBody(position, m_bodyType);

        if (m_body)
        {
            b2BodySetFixedRotation(m_body, m_fixedRotation);
            b2BodySetBullet(m_body, m_bullet);
            b2BodySetUserData(m_body, static_cast<void *>(entity));
            m_initialized = true;
        }
        else
        {
            std::cerr << "Erreur: Impossible de créer un corps Box2D" << std::endl;
        }
    }

    void PhysicsComponent::update(float deltaTime)
    {
        if (!m_body || !m_initialized)
            return;

        Entity *entity = getOwner();
        if (!entity)
            return;

        // Mettre à jour la position de l'entité en fonction du corps physique
        auto transformComp = entity->getComponent<class TransformComponent>();
        if (transformComp)
        {
            box2d::b2Vec2 pos = b2BodyGetPosition(m_body);
            sf::Vector2f position = PhysicsWorld::metersToPixels(pos);
            float angle = b2BodyGetAngle(m_body) * 180.0f / box2d::b2_pi; // Convertir de radians à degrés

            transformComp->setPosition(position);
            transformComp->setRotation(angle);
        }
    }

    b2FixtureId PhysicsComponent::createBoxFixture(const sf::Vector2f &size, float density,
                                                   float friction, float restitution,
                                                   uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!m_body || !m_initialized)
            return nullptr;

        return PhysicsWorld::getInstance().addBoxFixture(m_body, size, density, friction,
                                                         restitution, categoryBits, maskBits, isSensor);
    }

    b2FixtureId PhysicsComponent::createCircleFixture(float radius, float density,
                                                      float friction, float restitution,
                                                      uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!m_body || !m_initialized)
            return nullptr;

        return PhysicsWorld::getInstance().addCircleFixture(m_body, radius, density, friction,
                                                            restitution, categoryBits, maskBits, isSensor);
    }

    void PhysicsComponent::applyForce(const sf::Vector2f &force, const sf::Vector2f &point)
    {
        if (!m_body || !m_initialized)
            return;

        box2d::b2Vec2 b2Force = PhysicsWorld::pixelsToMeters(force);
        box2d::b2Vec2 b2Point = PhysicsWorld::pixelsToMeters(point);

        box2d::b2Vec2 worldPoint;
        b2TransformMulVec2(b2BodyGetTransform(m_body), b2Point, &worldPoint);

        b2BodyApplyForce(m_body, b2Force, worldPoint);
    }

    void PhysicsComponent::applyForceToCenter(const sf::Vector2f &force)
    {
        if (!m_body || !m_initialized)
            return;

        box2d::b2Vec2 b2Force = PhysicsWorld::pixelsToMeters(force);
        b2BodyApplyForceToCenter(m_body, b2Force);
    }

    void PhysicsComponent::applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point)
    {
        if (!m_body || !m_initialized)
            return;

        box2d::b2Vec2 b2Impulse = PhysicsWorld::pixelsToMeters(impulse);
        box2d::b2Vec2 b2Point = PhysicsWorld::pixelsToMeters(point);

        box2d::b2Vec2 worldPoint;
        b2TransformMulVec2(b2BodyGetTransform(m_body), b2Point, &worldPoint);

        b2BodyApplyLinearImpulse(m_body, b2Impulse, worldPoint);
    }

    void PhysicsComponent::applyAngularImpulse(float impulse)
    {
        if (!m_body || !m_initialized)
            return;

        b2BodyApplyAngularImpulse(m_body, impulse);
    }

    sf::Vector2f PhysicsComponent::getPosition() const
    {
        if (!m_body || !m_initialized)
            return sf::Vector2f(0.0f, 0.0f);

        box2d::b2Vec2 pos = b2BodyGetPosition(m_body);
        return PhysicsWorld::metersToPixels(pos);
    }

    void PhysicsComponent::setPosition(const sf::Vector2f &position)
    {
        if (!m_body || !m_initialized)
            return;

        box2d::b2Vec2 b2Position = PhysicsWorld::pixelsToMeters(position);
        b2BodySetTransform(m_body, b2Position, b2BodyGetAngle(m_body));
    }

    float PhysicsComponent::getAngle() const
    {
        if (!m_body || !m_initialized)
            return 0.0f;

        float angle = b2BodyGetAngle(m_body) * 180.0f / box2d::b2_pi; // Convertir de radians à degrés
        return angle;
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (!m_body || !m_initialized)
            return;

        float radians = angle * box2d::b2_pi / 180.0f; // Convertir de degrés à radians
        box2d::b2Vec2 pos = b2BodyGetPosition(m_body);
        b2BodySetTransform(m_body, pos, radians);
    }

    sf::Vector2f PhysicsComponent::getLinearVelocity() const
    {
        if (!m_body || !m_initialized)
            return sf::Vector2f(0.0f, 0.0f);

        box2d::b2Vec2 velocity = b2BodyGetLinearVelocity(m_body);
        return PhysicsWorld::metersToPixels(velocity);
    }

    void PhysicsComponent::setLinearVelocity(const sf::Vector2f &velocity)
    {
        if (!m_body || !m_initialized)
            return;

        box2d::b2Vec2 b2Velocity = PhysicsWorld::pixelsToMeters(velocity);
        b2BodySetLinearVelocity(m_body, b2Velocity);
    }

    float PhysicsComponent::getAngularVelocity() const
    {
        if (!m_body || !m_initialized)
            return 0.0f;

        return b2BodyGetAngularVelocity(m_body) * 180.0f / box2d::b2_pi; // Convertir de radians/s à degrés/s
    }

    void PhysicsComponent::setAngularVelocity(float velocity)
    {
        if (!m_body || !m_initialized)
            return;

        float radiansPerSec = velocity * box2d::b2_pi / 180.0f; // Convertir de degrés/s à radians/s
        b2BodySetAngularVelocity(m_body, radiansPerSec);
    }

    void PhysicsComponent::setBodyType(b2BodyType type)
    {
        if (!m_body || !m_initialized)
        {
            m_bodyType = type;
            return;
        }

        b2BodySetType(m_body, type);
    }

    b2BodyType PhysicsComponent::getBodyType() const
    {
        if (!m_body || !m_initialized)
            return m_bodyType;

        return b2BodyGetType(m_body);
    }

    void PhysicsComponent::setFixedRotation(bool fixed)
    {
        m_fixedRotation = fixed;

        if (m_body && m_initialized)
        {
            b2BodySetFixedRotation(m_body, fixed);
        }
    }

    bool PhysicsComponent::isFixedRotation() const
    {
        if (!m_body || !m_initialized)
            return m_fixedRotation;

        return b2BodyIsFixedRotation(m_body);
    }

    void PhysicsComponent::setBullet(bool bullet)
    {
        m_bullet = bullet;

        if (m_body && m_initialized)
        {
            b2BodySetBullet(m_body, bullet);
        }
    }

    bool PhysicsComponent::isBullet() const
    {
        if (!m_body || !m_initialized)
            return m_bullet;

        return b2BodyIsBullet(m_body);
    }

    b2BodyId PhysicsComponent::getBody() const
    {
        return m_body;
    }

} // namespace Orenji