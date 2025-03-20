#include "../../include/Core/SystemManager.hpp"
#include "../../include/Core/EntityManager.hpp"
#include <iostream>

namespace Orenji
{

    SystemManager::SystemManager()
    {
        // Constructor implementation
    }

    SystemManager::~SystemManager()
    {
        // Cleanup all systems
        m_systems.clear();
    }

    SystemManager &SystemManager::getInstance()
    {
        static SystemManager instance;
        return instance;
    }

    void SystemManager::initialize()
    {
        // Initialize all registered systems
        for (auto &pair : m_systems)
        {
            pair.second->initialize();
        }
    }

    void SystemManager::setEntityManager(std::shared_ptr<EntityManager> entityManager)
    {
        m_entityManager = entityManager;

        // Share the entity manager with all systems
        for (auto &pair : m_systems)
        {
            pair.second->setEntityManager(entityManager);
        }
    }

    void SystemManager::update(float deltaTime)
    {
        // Update all registered systems
        for (auto &pair : m_systems)
        {
            pair.second->update(deltaTime);
        }
    }

} // namespace Orenji