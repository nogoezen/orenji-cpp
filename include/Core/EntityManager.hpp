#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>
#include "Entity.hpp"

namespace Core
{

    /**
     * @brief Manager class for entities
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
        ~EntityManager();

        /**
         * @brief Create a new entity
         * @param name Optional entity name
         * @return Pointer to the created entity
         */
        Entity *createEntity(const std::string &name = "");

        /**
         * @brief Get an entity by ID
         * @param id Entity ID
         * @return Pointer to the entity, or nullptr if not found
         */
        Entity *getEntity(unsigned int id);

        /**
         * @brief Get entities by name
         * @param name Entity name
         * @return Vector of pointers to entities with matching name
         */
        std::vector<Entity *> getEntitiesByName(const std::string &name);

        /**
         * @brief Get all entities
         * @return Vector of pointers to all entities
         */
        std::vector<Entity *> getAllEntities();

        /**
         * @brief Remove an entity by ID
         * @param id Entity ID
         * @return true if the entity was removed
         */
        bool removeEntity(unsigned int id);

        /**
         * @brief Remove all entities
         */
        void removeAllEntities();

        /**
         * @brief Find entities with specific component types
         * @tparam T Component type
         * @tparam Args Additional component types
         * @return Vector of pointers to entities with the specified component types
         */
        template <typename T, typename... Args>
        std::vector<Entity *> findEntitiesWithComponents()
        {
            std::vector<Entity *> result;

            for (auto &pair : m_entities)
            {
                Entity *entity = pair.second.get();
                if (entity->isActive() && entity->hasComponent<T>() && hasAllComponents<Args...>(entity))
                {
                    result.push_back(entity);
                }
            }

            return result;
        }

        /**
         * @brief Update all entities
         * @param deltaTime Time since last frame in seconds
         */
        void update(float deltaTime);

    private:
        std::unordered_map<unsigned int, std::unique_ptr<Entity>> m_entities;
        unsigned int m_nextEntityId;

        // Helper function for checking if an entity has all component types
        template <typename T>
        bool hasAllComponents(Entity *entity)
        {
            return entity->hasComponent<T>();
        }

        template <typename T, typename U, typename... Args>
        bool hasAllComponents(Entity *entity)
        {
            return entity->hasComponent<T>() && hasAllComponents<U, Args...>(entity);
        }

        // Base case for variadic template
        bool hasAllComponents(Entity *entity)
        {
            return true;
        }
    };

} // namespace Core