#include "../../include/AI/AISystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include <iostream>

namespace AI
{

    AISystem::AISystem(Core::EntityManager &entityManager)
        : Core::System(entityManager)
    {
        std::cout << "AISystem created" << std::endl;
    }

    AISystem::~AISystem()
    {
        std::cout << "AISystem destroyed" << std::endl;
    }

    void AISystem::init()
    {
        std::cout << "AISystem initialized" << std::endl;
    }

    void AISystem::update(float deltaTime)
    {
        // Dans une vraie implémentation, nous mettrions à jour les comportements d'IA
        // pour toutes les entités ayant des composants d'IA

        // Exemple :
        // for (auto entity : m_entityManager.findEntitiesWithComponents<AIComponent>()) {
        //     auto* aiComponent = entity->getComponent<AIComponent>();
        //     aiComponent->getBehaviorTree()->update(deltaTime);
        // }
    }

} // namespace AI