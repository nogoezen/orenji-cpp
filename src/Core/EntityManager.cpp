#include "../../include/Core/EntityManager.hpp"

namespace Orenji
{

    EntityManager::EntityManager() : m_nextEntityId(0)
    {
        // Initialize the entity manager
    }

    EntityPtr EntityManager::createEntity()
    {
        // Create a new entity with the next available ID
        auto entity = std::make_shared<Entity>(m_nextEntityId++);

        // Store the entity
        m_entitiesMap[entity->getId()] = entity;
        m_entities.push_back(entity);

        return entity;
    }

    bool EntityManager::removeEntity(unsigned int id)
    {
        // Queue the entity for removal
        m_pendingRemovals.push(id);
        return true;
    }

    bool EntityManager::removeEntity(EntityPtr entity)
    {
        if (entity)
        {
            return removeEntity(entity->getId());
        }
        return false;
    }

    EntityPtr EntityManager::getEntity(unsigned int id) const
    {
        auto it = m_entitiesMap.find(id);
        if (it != m_entitiesMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    const std::vector<EntityPtr> &EntityManager::getEntities() const
    {
        return m_entities;
    }

    void EntityManager::update(float deltaTime)
    {
        // Process any pending entity removals
        processPendingRemovals();

        // Update all entities
        for (auto &entity : m_entities)
        {
            if (entity->isActive())
            {
                entity->update(deltaTime);
            }
        }

        // Update all systems
        for (auto &[type, system] : m_systems)
        {
            system->update(deltaTime);
        }
    }

    void EntityManager::processPendingRemovals()
    {
        // Process all pending entity removals
        while (!m_pendingRemovals.empty())
        {
            unsigned int id = m_pendingRemovals.front();
            m_pendingRemovals.pop();

            // Remove entity from all systems
            auto entity = getEntity(id);
            if (entity)
            {
                for (auto &[type, system] : m_systems)
                {
                    system->removeEntity(entity);
                }

                // Remove entity from container
                m_entities.erase(
                    std::remove_if(m_entities.begin(), m_entities.end(),
                                   [id](const EntityPtr &e)
                                   { return e->getId() == id; }),
                    m_entities.end());

                // Remove entity from map
                m_entitiesMap.erase(id);
            }
        }
    }

} // namespace Orenji