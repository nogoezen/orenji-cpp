#include "../../include/Physics/PhysicsSystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/Physics/PhysicsComponent.hpp"
#include <iostream>

namespace Physics
{

    PhysicsSystem::PhysicsSystem(Core::EntityManager &entityManager, b2Vec2 gravity)
        : Core::System(entityManager), m_physics(std::make_unique<Box2DWrapper>(gravity)), m_velocityIterations(6), m_positionIterations(2)
    {
        std::cout << "PhysicsSystem created" << std::endl;
    }

    PhysicsSystem::~PhysicsSystem()
    {
        std::cout << "PhysicsSystem destroyed" << std::endl;
    }

    void PhysicsSystem::init()
    {
        std::cout << "PhysicsSystem initialized" << std::endl;
    }

    void PhysicsSystem::update(float deltaTime)
    {
        // Mise à jour du monde Box2D
        m_physics->step(deltaTime, m_velocityIterations, m_positionIterations);

        // Mise à jour des composants physiques
        // Dans un système ECS, cela pourrait être implémenté différemment
        // en fonction de la façon dont les entités et les composants sont gérés
        auto entities = m_entityManager.getEntitiesWithComponent<PhysicsComponent>();
        for (auto &entity : entities)
        {
            auto physComponent = entity->getComponent<PhysicsComponent>();
            if (physComponent && physComponent->isActive())
            {
                physComponent->update(deltaTime);
            }
        }
    }

    Box2DWrapper &PhysicsSystem::getPhysics()
    {
        return *m_physics;
    }

    void PhysicsSystem::setGravity(b2Vec2 gravity)
    {
        b2WorldId worldId = m_physics->getWorld();
        if (b2World_IsValid(worldId))
        {
            b2World_SetGravity(worldId, gravity);
        }
    }

    b2Vec2 PhysicsSystem::getGravity() const
    {
        b2WorldId worldId = m_physics->getWorld();
        if (b2World_IsValid(worldId))
        {
            return b2World_GetGravity(worldId);
        }
        return {0.0f, 0.0f};
    }

    void PhysicsSystem::setAllowSleep(bool enableSleep)
    {
        b2WorldId worldId = m_physics->getWorld();
        if (b2World_IsValid(worldId))
        {
            b2World_EnableSleeping(worldId, enableSleep);
        }
    }

    void PhysicsSystem::setIterations(int velocityIterations, int positionIterations)
    {
        m_velocityIterations = velocityIterations;
        m_positionIterations = positionIterations;
    }

} // namespace Physics