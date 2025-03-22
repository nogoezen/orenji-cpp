#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Entity.hpp"

namespace Orenji
{

    /**
     * @class System
     * @brief Base class for all systems in the ECS architecture
     */
    class System
    {
    public:
        /**
         * @brief Constructor
         */
        System() = default;

        /**
         * @brief Virtual destructor
         */
        virtual ~System() = default;

        /**
         * @brief Add an entity to be processed by this system
         * @param entity Entity to be added
         */
        virtual void addEntity(EntityPtr entity)
        {
            m_entities.push_back(entity);
        }

        /**
         * @brief Remove an entity from this system
         * @param entity Entity to be removed
         */
        virtual void removeEntity(EntityPtr entity)
        {
            m_entities.erase(
                std::remove_if(m_entities.begin(), m_entities.end(),
                               [entity](const EntityPtr &e)
                               { return e->getId() == entity->getId(); }),
                m_entities.end());
        }

        /**
         * @brief Initialize the system
         */
        virtual void initialize() {}

        /**
         * @brief Update all entities managed by this system
         * @param deltaTime Time elapsed since last update
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Get the system type
         * @return The system type as a string
         */
        virtual std::string getType() const = 0;

    protected:
        std::vector<EntityPtr> m_entities; // Entities managed by this system
    };

    using SystemPtr = std::shared_ptr<System>;

} // namespace Orenji