#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/Core/Entity.hpp"
#include <iostream>

namespace Orenji
{

    PhysicsComponent::PhysicsComponent(b2BodyType type, bool fixedRotation, bool bullet)
        : m_body(nullptr),
          m_bodyType(type),
          m_fixedRotation(fixedRotation),
          m_bullet(bullet),
          m_initialized(false)
    {
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
            m_body->SetFixedRotation(m_fixedRotation);
            m_body->SetBullet(m_bullet);
            m_body->SetUserData(static_cast<void *>(entity));
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
            sf::Vector2f position = PhysicsWorld::metersToPixels(m_body->GetPosition());
            float angle = m_body->GetAngle() * 180.0f / b2_pi; // Convertir de radians à degrés

            transformComp->setPosition(position);
            transformComp->setRotation(angle);
        }
    }

    b2Fixture *PhysicsComponent::createBoxFixture(const sf::Vector2f &size, float density,
                                                  float friction, float restitution,
                                                  uint16_t categoryBits, uint16_t maskBits, bool isSensor)
    {
        if (!m_body || !m_initialized)
            return nullptr;

        return PhysicsWorld::getInstance().addBoxFixture(m_body, size, density, friction,
                                                         restitution, categoryBits, maskBits, isSensor);
    }

    b2Fixture *PhysicsComponent::createCircleFixture(float radius, float density,
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

        b2Vec2 b2Force = PhysicsWorld::pixelsToMeters(force);
        b2Vec2 b2Point = PhysicsWorld::pixelsToMeters(point);

        m_body->ApplyForce(b2Force, m_body->GetWorldPoint(b2Point), true);
    }

    void PhysicsComponent::applyForceToCenter(const sf::Vector2f &force)
    {
        if (!m_body || !m_initialized)
            return;

        b2Vec2 b2Force = PhysicsWorld::pixelsToMeters(force);
        m_body->ApplyForceToCenter(b2Force, true);
    }

    void PhysicsComponent::applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point)
    {
        if (!m_body || !m_initialized)
            return;

        b2Vec2 b2Impulse = PhysicsWorld::pixelsToMeters(impulse);
        b2Vec2 b2Point = PhysicsWorld::pixelsToMeters(point);

        m_body->ApplyLinearImpulse(b2Impulse, m_body->GetWorldPoint(b2Point), true);
    }

    void PhysicsComponent::applyAngularImpulse(float impulse)
    {
        if (!m_body || !m_initialized)
            return;

        m_body->ApplyAngularImpulse(impulse, true);
    }

    sf::Vector2f PhysicsComponent::getPosition() const
    {
        if (!m_body || !m_initialized)
            return sf::Vector2f(0.0f, 0.0f);

        return PhysicsWorld::metersToPixels(m_body->GetPosition());
    }

    void PhysicsComponent::setPosition(const sf::Vector2f &position)
    {
        if (!m_body || !m_initialized)
            return;

        b2Vec2 b2Position = PhysicsWorld::pixelsToMeters(position);
        m_body->SetTransform(b2Position, m_body->GetAngle());
    }

    float PhysicsComponent::getAngle() const
    {
        if (!m_body || !m_initialized)
            return 0.0f;

        return m_body->GetAngle() * 180.0f / b2_pi; // Convertir de radians à degrés
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (!m_body || !m_initialized)
            return;

        float radians = angle * b2_pi / 180.0f; // Convertir de degrés à radians
        m_body->SetTransform(m_body->GetPosition(), radians);
    }

    sf::Vector2f PhysicsComponent::getLinearVelocity() const
    {
        if (!m_body || !m_initialized)
            return sf::Vector2f(0.0f, 0.0f);

        return PhysicsWorld::metersToPixels(m_body->GetLinearVelocity());
    }

    void PhysicsComponent::setLinearVelocity(const sf::Vector2f &velocity)
    {
        if (!m_body || !m_initialized)
            return;

        b2Vec2 b2Velocity = PhysicsWorld::pixelsToMeters(velocity);
        m_body->SetLinearVelocity(b2Velocity);
    }

    float PhysicsComponent::getAngularVelocity() const
    {
        if (!m_body || !m_initialized)
            return 0.0f;

        return m_body->GetAngularVelocity() * 180.0f / b2_pi; // Convertir de radians/s à degrés/s
    }

    void PhysicsComponent::setAngularVelocity(float velocity)
    {
        if (!m_body || !m_initialized)
            return;

        float radiansPerSec = velocity * b2_pi / 180.0f; // Convertir de degrés/s à radians/s
        m_body->SetAngularVelocity(radiansPerSec);
    }

    b2Body *PhysicsComponent::getBody() const
    {
        return m_body;
    }

} // namespace Orenji