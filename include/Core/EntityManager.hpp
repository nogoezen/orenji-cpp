#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>

#include "Entity.hpp"
#include "System.hpp"

namespace Orenji
{

    /**
     * @class EntityManager
     * @brief Manages all entities and systems in the game
     */
    class EntityManager
    {
    public:
        /**
         * @brief Constructor
         */
        EntityManager();

        /**
         * @brief Destructor
         */
        ~EntityManager() = default;

        /**
         * @brief Create a new entity
         * @return Shared pointer to the created entity
         */
        EntityPtr createEntity();

        /**
         * @brief Remove an entity by its ID
         * @param id ID of the entity to remove
         * @return True if the entity was removed
         */
        bool removeEntity(unsigned int id);

        /**
         * @brief Remove an entity
         * @param entity Entity to remove
         * @return True if the entity was removed
         */
        bool removeEntity(EntityPtr entity);

        /**
         * @brief Get an entity by its ID
         * @param id ID of the entity to get
         * @return Shared pointer to the entity or nullptr if not found
         */
        EntityPtr getEntity(unsigned int id) const;

        /**
         * @brief Get all entities
         * @return Vector of all entities
         */
        const std::vector<EntityPtr> &getEntities() const;

        /**
         * @brief Register a system
         * @tparam T System type
         * @tparam Args Constructor argument types
         * @param args Constructor arguments
         * @return Shared pointer to the created system
         */
        template <typename T, typename... Args>
        std::shared_ptr<T> registerSystem(Args &&...args)
        {
            static_assert(std::is_base_of<System, T>::value, "T must derive from System");

            auto system = std::make_shared<T>(std::forward<Args>(args)...);
            auto baseSystem = std::static_pointer_cast<System>(system);

            std::string systemType = baseSystem->getType();
            m_systems[systemType] = baseSystem;

            // Initialize the system after registration
            baseSystem->initialize();

            return system;
        }

        /**
         * @brief Get a system by its type
         * @tparam T System type
         * @return Shared pointer to the system or nullptr if not found
         */
        template <typename T>
        std::shared_ptr<T> getSystem()
        {
            // Create a temporary instance to get its type
            T temp;
            std::string systemType = temp.getType();

            auto it = m_systems.find(systemType);
            if (it != m_systems.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        /**
         * @brief Update all entities and systems
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime);

        /**
         * @brief Process pending entity removals
         */
        void processPendingRemovals();

    private:
        unsigned int m_nextEntityId = 0;                           // Next available entity ID
        std::unordered_map<unsigned int, EntityPtr> m_entitiesMap; // Entities map by ID
        std::vector<EntityPtr> m_entities;                         // Vector of all entities for iteration
        std::unordered_map<std::string, SystemPtr> m_systems;      // Systems map by type
        std::queue<unsigned int> m_pendingRemovals;                // Queue of entities to be removed
    };

} // namespace Orenji