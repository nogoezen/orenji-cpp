#include "../../include/Physics/PhysicsSystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include <iostream>

namespace Physics
{

    PhysicsSystem::PhysicsSystem(Core::EntityManager &entityManager)
        : Core::System(entityManager)
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
        // Exemple d'implémentation simple
        // Dans une vraie implémentation, nous aurions Box2D ici

        // Mise à jour de la simulation physique avec le deltaTime
    }

} // namespace Physics