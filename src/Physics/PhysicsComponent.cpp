#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/Core/Entity.hpp"
#include <cmath>

namespace Physics
{
    PhysicsComponent::PhysicsComponent(Box2DWrapper &physics, const b2Vec2 &position, BodyType bodyType)
        : m_physics(physics), m_body(nullptr), m_initialPosition(position), m_bodyType(bodyType), m_initialized(false)
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
        if (m_body)
        {
            m_physics.destroyBody(m_body);
            m_body = nullptr;
        }
    }

    void PhysicsComponent::init()
    {
        if (m_initialized)
            return;

        // Créer le corps Box2D en fonction du type
        switch (m_bodyType)
        {
        case BodyType::Static:
            m_body = m_physics.createStaticBody(m_initialPosition, 0.0f, getOwner());
            break;
        case BodyType::Dynamic:
            m_body = m_physics.createDynamicBody(m_initialPosition, 0.0f, getOwner());
            break;
        case BodyType::Kinematic:
            // Pour un corps cinématique, nous utilisons un corps dynamique mais spécial
            m_body = m_physics.createDynamicBody(m_initialPosition, 0.0f, getOwner());
            if (m_body)
            {
                m_body->SetType(b2_kinematicBody);
            }
            break;
        }

        m_initialized = true;
    }

    void PhysicsComponent::update(float deltaTime)
    {
        if (!m_body || !isActive())
            return;

        // Si l'entité possède des composants de transformation, on peut les mettre à jour ici
        // En fonction de la position du corps physique
        if (auto *owner = getOwner())
        {
            // Exemple: owner->setPosition(getPosition());
            // Exemple: owner->setRotation(getAngle());
        }
    }

    b2Fixture *PhysicsComponent::addBox(float width, float height,
                                        float density, float friction, float restitution,
                                        const b2Vec2 &offset)
    {
        if (!m_body)
            return nullptr;
        return m_physics.addBoxFixture(m_body, width, height, density, friction, restitution, offset);
    }

    b2Fixture *PhysicsComponent::addCircle(float radius,
                                           float density, float friction, float restitution,
                                           const b2Vec2 &offset)
    {
        if (!m_body)
            return nullptr;
        return m_physics.addCircleFixture(m_body, radius, density, friction, restitution, offset);
    }

    b2Vec2 PhysicsComponent::getPosition() const
    {
        if (!m_body)
            return m_initialPosition;
        return Box2DWrapper::metersToPixels(m_body->GetPosition());
    }

    float PhysicsComponent::getAngle() const
    {
        if (!m_body)
            return 0.0f;
        // Convertir des radians en degrés
        return m_body->GetAngle() * 180.0f / b2_pi;
    }

    b2Vec2 PhysicsComponent::getLinearVelocity() const
    {
        if (!m_body)
            return b2Vec2(0.0f, 0.0f);
        return Box2DWrapper::metersToPixels(m_body->GetLinearVelocity());
    }

    float PhysicsComponent::getAngularVelocity() const
    {
        if (!m_body)
            return 0.0f;
        // Convertir des radians/sec en degrés/sec
        return m_body->GetAngularVelocity() * 180.0f / b2_pi;
    }

    void PhysicsComponent::setPosition(const b2Vec2 &position)
    {
        if (!m_body)
        {
            m_initialPosition = position;
            return;
        }

        b2Vec2 pos = Box2DWrapper::pixelsToMeters(position);
        m_body->SetTransform(pos, m_body->GetAngle());
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (!m_body)
            return;

        // Convertir des degrés en radians
        float angleRad = angle * b2_pi / 180.0f;
        m_body->SetTransform(m_body->GetPosition(), angleRad);
    }

    void PhysicsComponent::setLinearVelocity(const b2Vec2 &velocity)
    {
        if (!m_body)
            return;

        b2Vec2 vel = Box2DWrapper::pixelsToMeters(velocity);
        m_body->SetLinearVelocity(vel);
    }

    void PhysicsComponent::setAngularVelocity(float velocity)
    {
        if (!m_body)
            return;

        // Convertir des degrés/sec en radians/sec
        float velocityRad = velocity * b2_pi / 180.0f;
        m_body->SetAngularVelocity(velocityRad);
    }

    void PhysicsComponent::applyForce(const b2Vec2 &force, bool wake)
    {
        if (!m_body)
            return;

        m_body->ApplyForceToCenter(force, wake);
    }

    void PhysicsComponent::applyForceToPoint(const b2Vec2 &force, const b2Vec2 &point, bool wake)
    {
        if (!m_body)
            return;

        b2Vec2 worldPoint = Box2DWrapper::pixelsToMeters(point);
        m_body->ApplyForce(force, worldPoint, wake);
    }

    void PhysicsComponent::applyLinearImpulse(const b2Vec2 &impulse, bool wake)
    {
        if (!m_body)
            return;

        m_body->ApplyLinearImpulseToCenter(impulse, wake);
    }

    void PhysicsComponent::applyLinearImpulseToPoint(const b2Vec2 &impulse, const b2Vec2 &point, bool wake)
    {
        if (!m_body)
            return;

        b2Vec2 worldPoint = Box2DWrapper::pixelsToMeters(point);
        m_body->ApplyLinearImpulse(impulse, worldPoint, wake);
    }

    void PhysicsComponent::applyTorque(float torque, bool wake)
    {
        if (!m_body)
            return;

        m_body->ApplyTorque(torque, wake);
    }

    void PhysicsComponent::applyAngularImpulse(float impulse, bool wake)
    {
        if (!m_body)
            return;

        m_body->ApplyAngularImpulse(impulse, wake);
    }

    void PhysicsComponent::setFixedRotation(bool fixed)
    {
        if (!m_body)
            return;

        m_body->SetFixedRotation(fixed);
    }

    void PhysicsComponent::setBullet(bool bullet)
    {
        if (!m_body)
            return;

        m_body->SetBullet(bullet);
    }

    b2Body *PhysicsComponent::getBody()
    {
        return m_body;
    }

} // namespace Physics