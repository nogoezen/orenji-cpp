#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/Core/Entity.hpp"
#include <cmath>
#include <box2d/constants.h>

namespace Physics
{
    PhysicsComponent::PhysicsComponent(Box2DWrapper &physics, const b2Vec2 &position, BodyType bodyType)
        : m_physics(physics),
          m_bodyId({0}),
          m_initialPosition(position),
          m_bodyType(bodyType),
          m_initialized(false),
          m_collisionCallback(nullptr)
    {
    }

    PhysicsComponent::~PhysicsComponent()
    {
        // Détruire le corps s'il existe
        if (b2Body_IsValid(m_bodyId))
        {
            m_physics.destroyBody(m_bodyId);
        }
    }

    void PhysicsComponent::init()
    {
        // Éviter l'initialisation multiple
        if (m_initialized)
            return;

        // Créer le corps en fonction du type
        switch (m_bodyType)
        {
        case BodyType::Static:
            m_bodyId = m_physics.createStaticBody(m_initialPosition, 0.0f, reinterpret_cast<void *>(getEntity().getId()));
            break;
        case BodyType::Dynamic:
            m_bodyId = m_physics.createDynamicBody(m_initialPosition, 0.0f, reinterpret_cast<void *>(getEntity().getId()));
            break;
        case BodyType::Kinematic:
            // Pour le corps cinématique, on utilise un corps dynamique avec une masse nulle
            m_bodyId = m_physics.createDynamicBody(m_initialPosition, 0.0f, reinterpret_cast<void *>(getEntity().getId()));
            // Définir les propriétés cinématiques
            b2Body_SetType(m_bodyId, b2_kinematicBody);
            break;
        }

        m_initialized = true;
    }

    void PhysicsComponent::update(float deltaTime)
    {
        // Rien à faire ici, Box2D gère la physique dans le PhysicsSystem
    }

    b2ShapeId PhysicsComponent::addBox(float width, float height,
                                       float density, float friction, float restitution,
                                       const b2Vec2 &offset, CollisionCategory category,
                                       CollisionCategory mask, bool isSensor)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return {0};

        // Créer une boîte avec le wrapper
        b2ShapeId shapeId = m_physics.addBoxFixture(m_bodyId, width, height, density, friction, restitution, offset);

        // Définir les filtres de collision
        m_physics.setFilterData(shapeId, category, mask);

        // Définir si c'est un capteur
        if (isSensor)
        {
            b2Shape_SetSensor(shapeId, true);
        }

        return shapeId;
    }

    b2ShapeId PhysicsComponent::addCircle(float radius,
                                          float density, float friction, float restitution,
                                          const b2Vec2 &offset, CollisionCategory category,
                                          CollisionCategory mask, bool isSensor)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return {0};

        // Créer un cercle avec le wrapper
        b2ShapeId shapeId = m_physics.addCircleFixture(m_bodyId, radius, density, friction, restitution, offset);

        // Définir les filtres de collision
        m_physics.setFilterData(shapeId, category, mask);

        // Définir si c'est un capteur
        if (isSensor)
        {
            b2Shape_SetSensor(shapeId, true);
        }

        return shapeId;
    }

    void PhysicsComponent::setCollisionFilter(b2ShapeId shapeId, CollisionCategory category,
                                              CollisionCategory mask, int16_t groupIndex)
    {
        if (!b2Shape_IsValid(shapeId))
            return;

        m_physics.setFilterData(shapeId, category, mask, groupIndex);
    }

    void PhysicsComponent::setSensor(b2ShapeId shapeId, bool isSensor)
    {
        if (!b2Shape_IsValid(shapeId))
            return;

        b2Shape_SetSensor(shapeId, isSensor);
    }

    void PhysicsComponent::setCollisionCallback(CollisionCallback callback)
    {
        m_collisionCallback = callback;
    }

    void PhysicsComponent::onCollisionBegin(Core::EntityId otherEntity, b2ContactId contact)
    {
        if (!m_collisionCallback)
            return;

        // Extraire les informations du contact
        b2ContactData *contactData = b2Contact_GetData(contact);
        b2ContactManifold *manifold = b2Contact_GetManifold(contact);

        // Préparer le résultat de la collision
        CollisionResult result;
        result.entityA = getEntity().getId();
        result.entityB = otherEntity;
        result.isSensor = b2Contact_IsSensor(contact);

        // S'il n'y a pas de points de contact, utiliser les positions des corps
        if (manifold->pointCount == 0)
        {
            result.contactPoint = Box2DWrapper::metersToPixels(b2Body_GetPosition(m_bodyId));
            result.normal = b2Vec2(0, 0);
            result.impulse = 0;
        }
        else
        {
            // Utiliser le premier point de contact
            b2Vec2 position = manifold->localPoints[0];

            // Convertir en coordonnées mondiales
            b2Transform transform = b2Body_GetTransform(m_bodyId);
            b2Vec2 worldPoint = b2Mul(transform, position);

            result.contactPoint = Box2DWrapper::metersToPixels(worldPoint);
            result.normal = manifold->normal;

            // On ne peut obtenir l'impulsion que pendant la résolution du contact
            result.impulse = 0;
        }

        // Appeler le callback
        m_collisionCallback(result);
    }

    void PhysicsComponent::onCollisionEnd(Core::EntityId otherEntity, b2ContactId contact)
    {
        if (!m_collisionCallback)
            return;

        // Pour la fin de collision, on ne dispose pas d'autant d'informations
        CollisionResult result;
        result.entityA = getEntity().getId();
        result.entityB = otherEntity;
        result.isSensor = b2Contact_IsSensor(contact);
        result.contactPoint = Box2DWrapper::metersToPixels(b2Body_GetPosition(m_bodyId));
        result.normal = b2Vec2(0, 0);
        result.impulse = 0;

        // Appeler le callback
        m_collisionCallback(result);
    }

    b2Vec2 PhysicsComponent::getPosition() const
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return m_initialPosition;

        return Box2DWrapper::metersToPixels(b2Body_GetPosition(m_bodyId));
    }

    float PhysicsComponent::getAngle() const
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return 0.0f;

        return b2Body_GetAngle(m_bodyId) * 180.0f / B2_pi;
    }

    b2Vec2 PhysicsComponent::getLinearVelocity() const
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return b2Vec2(0.0f, 0.0f);

        return Box2DWrapper::metersToPixels(b2Body_GetLinearVelocity(m_bodyId));
    }

    float PhysicsComponent::getAngularVelocity() const
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return 0.0f;

        return b2Body_GetAngularVelocity(m_bodyId) * 180.0f / B2_pi;
    }

    void PhysicsComponent::setPosition(const b2Vec2 &position)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_SetTransform(m_bodyId, Box2DWrapper::pixelsToMeters(position), b2Body_GetAngle(m_bodyId));
    }

    void PhysicsComponent::setAngle(float angle)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_SetTransform(m_bodyId, b2Body_GetPosition(m_bodyId), angle * B2_pi / 180.0f);
    }

    void PhysicsComponent::setLinearVelocity(const b2Vec2 &velocity)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_SetLinearVelocity(m_bodyId, Box2DWrapper::pixelsToMeters(velocity));
    }

    void PhysicsComponent::setAngularVelocity(float velocity)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_SetAngularVelocity(m_bodyId, velocity * B2_pi / 180.0f);
    }

    void PhysicsComponent::applyForce(const b2Vec2 &force, bool wake)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_ApplyForce(m_bodyId, Box2DWrapper::pixelsToMeters(force), b2Body_GetPosition(m_bodyId), wake);
    }

    void PhysicsComponent::applyForceToPoint(const b2Vec2 &force, const b2Vec2 &point, bool wake)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_ApplyForce(m_bodyId, Box2DWrapper::pixelsToMeters(force), Box2DWrapper::pixelsToMeters(point), wake);
    }

    void PhysicsComponent::applyLinearImpulse(const b2Vec2 &impulse, bool wake)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_ApplyLinearImpulse(m_bodyId, Box2DWrapper::pixelsToMeters(impulse), b2Body_GetPosition(m_bodyId), wake);
    }

    void PhysicsComponent::applyLinearImpulseToPoint(const b2Vec2 &impulse, const b2Vec2 &point, bool wake)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_ApplyLinearImpulse(m_bodyId, Box2DWrapper::pixelsToMeters(impulse), Box2DWrapper::pixelsToMeters(point), wake);
    }

    void PhysicsComponent::applyTorque(float torque, bool wake)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_ApplyTorque(m_bodyId, torque, wake);
    }

    void PhysicsComponent::applyAngularImpulse(float impulse, bool wake)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_ApplyAngularImpulse(m_bodyId, impulse, wake);
    }

    void PhysicsComponent::setFixedRotation(bool fixed)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_SetFixedRotation(m_bodyId, fixed);
    }

    void PhysicsComponent::setBullet(bool bullet)
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return;

        b2Body_SetBullet(m_bodyId, bullet);
    }

    b2Body *PhysicsComponent::getBody()
    {
        if (!m_initialized || !b2Body_IsValid(m_bodyId))
            return nullptr;

        // This is incorrect for Box2D 3.0, but we keep the method for compatibility
        // Use getBodyId() instead for Box2D 3.0
        return reinterpret_cast<b2Body *>(nullptr);
    }

    b2BodyId PhysicsComponent::getBodyId() const
    {
        return m_bodyId;
    }

} // namespace Physics