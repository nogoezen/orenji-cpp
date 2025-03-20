#pragma once
#include "System.hpp"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>

namespace Orenji
{
    class EntityManager;

    class SystemManager
    {
    public:
        SystemManager();
        ~SystemManager();

        // Get singleton instance
        static SystemManager &getInstance();

        // Initialize all systems
        void initialize();

        // Register a system with the manager
        template <typename T>
        std::shared_ptr<T> registerSystem()
        {
            static_assert(std::is_base_of<System, T>::value, "T must derive from System");

            // Check if system already exists
            auto systemType = std::type_index(typeid(T));
            if (m_systems.find(systemType) != m_systems.end())
            {
                return std::static_pointer_cast<T>(m_systems[systemType]);
            }

            // Create and store system
            auto system = std::make_shared<T>();
            system->setName(typeid(T).name());
            m_systems[systemType] = system;

            return system;
        }

        // Get a reference to a system
        template <typename T>
        std::shared_ptr<T> getSystem()
        {
            static_assert(std::is_base_of<System, T>::value, "T must derive from System");

            auto systemType = std::type_index(typeid(T));
            if (m_systems.find(systemType) == m_systems.end())
            {
                return nullptr;
            }

            return std::static_pointer_cast<T>(m_systems[systemType]);
        }

        // Set the entity manager to be shared with all systems
        void setEntityManager(std::shared_ptr<EntityManager> entityManager);

        // Update all systems
        void update(float deltaTime);

    private:
        // Prevent copying
        SystemManager(const SystemManager &) = delete;
        SystemManager &operator=(const SystemManager &) = delete;

        // Storage for all systems, indexed by type
        std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;

        // Shared entity manager
        std::shared_ptr<EntityManager> m_entityManager;
    };
}