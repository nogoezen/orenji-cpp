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
            
            // Définir les données utilisateur si elles existent
            if (m_userData)
            {
                b2BodyUserData userData;
                userData.pointer = (uintptr_t)m_userData;
                b2Body_SetUserData(m_body, userData);
            }
            
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
            float angle = b2Body_GetAngle(m_body) * 180.0f / box2d::b2_pi; // Convertir de radians à degrés
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
        worldPoint.x = PhysicsWorld::pixelsToMeters(point.x);
        worldPoint.y = PhysicsWorld::pixelsToMeters(point.y);

        b2Body_ApplyForce(m_body, b2Force, worldPoint, true);
    }

    void PhysicsComponent::applyForceToCenter(const sf::Vector2f &force)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 b2Force;
        b2Force.x = force.x;
        b2Force.y = force.y;

        // Utiliser la position du corps comme point d'application
        b2Vec2 center = b2Body_GetPosition(m_body);
        
        // Appliquer la force au centre du corps
        b2Body_ApplyForce(m_body, b2Force, center, true);
    }

    void PhysicsComponent::applyLinearImpulse(const sf::Vector2f &impulse, const sf::Vector2f &point)
    {
        if (!IsValid(m_body) || !m_initialized)
            return;

        b2Vec2 b2Impulse;
        b2Impulse.x = impulse.x;
        b2Impulse.y = impulse.y;

        b2Vec2 worldPoint;
        worldPoint.x = PhysicsWorld::pixelsToMeters(point.x);
        worldPoint.y = PhysicsWorld::pixelsToMeters(point.y);

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
        if (!IsValid(m_body))
            return sf::Vector2f(0.0f, 0.0f);

        b2Vec2 position = b2Body_GetPosition(m_body);
        return PhysicsWorld::metersToPixels(position);
    }

    void PhysicsComponent::setPosition(const sf::Vector2f &position)
    {
        if (!IsValid(m_body))
            return;

        b2Vec2 meterPos = PhysicsWorld::pixelsToMeters(position);
        
        // Pour définir la position, on récupère la transformation actuelle
        b2Transform transform = b2Body_GetTransform(m_body);
        
        // On met à jour la position tout en gardant la rotation
        transform.p = meterPos;
        
        // On applique la nouvelle transformation
        b2Body_SetTransform(m_body, transform);
    }

    float PhysicsComponent::getAngle() const
    {
        if (!IsValid(m_body))
            return 0.0f;

        // Convertir l'angle de radians en degrés
        return b2Body_GetAngle(m_body) * 180.0f / box2d::b2_pi;
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (!IsValid(m_body))
            return;

        // Convertir l'angle de degrés en radians
        float radians = angle * box2d::b2_pi / 180.0f;

        // Récupérer la transformation actuelle
        b2Transform transform = b2Body_GetTransform(m_body);
        
        // Créer une nouvelle rotation
        b2Rot rotation;
        rotation.s = sinf(radians);
        rotation.c = cosf(radians);
        
        // Mettre à jour la rotation tout en gardant la position
        transform.q = rotation;
        
        // Appliquer la nouvelle transformation
        b2Body_SetTransform(m_body, transform);
    }

    sf::Vector2f PhysicsComponent::getLinearVelocity() const
    {
        if (!IsValid(m_body))
            return sf::Vector2f(0.0f, 0.0f);

        b2Vec2 velocity = b2Body_GetLinearVelocity(m_body);
        return sf::Vector2f(velocity.x * PhysicsWorld::PIXELS_PER_METER, 
                            velocity.y * PhysicsWorld::PIXELS_PER_METER);
    }

    void PhysicsComponent::setLinearVelocity(const sf::Vector2f &velocity)
    {
        if (!IsValid(m_body))
            return;

        b2Vec2 b2Velocity;
        b2Velocity.x = velocity.x * PhysicsWorld::METERS_PER_PIXEL;
        b2Velocity.y = velocity.y * PhysicsWorld::METERS_PER_PIXEL;

        b2Body_SetLinearVelocity(m_body, b2Velocity);
    }

    float PhysicsComponent::getAngularVelocity() const
    {
        if (!IsValid(m_body))
            return 0.0f;

        // Convertir de radians/seconde à degrés/seconde
        return b2Body_GetAngularVelocity(m_body) * 180.0f / box2d::b2_pi;
    }

    void PhysicsComponent::setAngularVelocity(float velocity)
    {
        if (!IsValid(m_body))
            return;

        // Convertir de degrés/seconde à radians/seconde
        float radians = velocity * box2d::b2_pi / 180.0f;
        b2Body_SetAngularVelocity(m_body, radians);
    }

    void PhysicsComponent::setBodyType(b2BodyType type)
    {
        if (m_bodyType == type)
            return;

        m_bodyType = type;

        if (IsValid(m_body))
        {
            // Avec Box2D 2.4.x, on met à jour la définition du corps
            b2BodyDef bodyDef = b2Body_GetDefinition(m_body);
            bodyDef.type = type;
            b2Body_SetDefinition(m_body, &bodyDef);
        }
    }

    b2BodyType PhysicsComponent::getBodyType() const
    {
        return m_bodyType;
    }

    void PhysicsComponent::setFixedRotation(bool fixed)
    {
        if (m_fixedRotation == fixed)
            return;

        m_fixedRotation = fixed;

        if (IsValid(m_body))
        {
            b2Body_SetFixedRotation(m_body, fixed);
        }
    }

    bool PhysicsComponent::isFixedRotation() const
    {
        return m_fixedRotation;
    }

    void PhysicsComponent::setBullet(bool bullet)
    {
        if (m_isBullet == bullet)
            return;

        m_isBullet = bullet;

        if (IsValid(m_body))
        {
            b2Body_SetBullet(m_body, bullet);
        }
    }

    bool PhysicsComponent::isBullet() const
    {
        return m_isBullet;
    }

    b2BodyId PhysicsComponent::getBody() const
    {
        return m_body;
    }

} // namespace Orenji