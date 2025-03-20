#include "../../include/Core/EntityManager.hpp"
#include <algorithm>

namespace Orenji
{

    EntityManager::EntityManager()
        : m_nextEntityId(0)
    {
    }

    EntityManager::~EntityManager()
    {
        removeAllEntities();
    }

    std::shared_ptr<Entity> EntityManager::createEntity(const std::string &name)
    {
        auto entity = std::make_shared<Entity>(m_nextEntityId++, name);
        m_entities[entity->getId()] = entity;
        return entity;
    }

    void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
    {
        if (entity)
        {
            removeEntity(entity->getId());
        }
    }

    void EntityManager::removeEntity(unsigned int entityId)
    {
        auto it = m_entities.find(entityId);
        if (it != m_entities.end())
        {
            m_entities.erase(it);
        }
    }

    void EntityManager::removeAllEntities()
    {
        m_entities.clear();
    }

    std::shared_ptr<Entity> EntityManager::getEntity(unsigned int entityId) const
    {
        auto it = m_entities.find(entityId);
        if (it != m_entities.end())
        {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<Entity>> EntityManager::getAllEntities() const
    {
        std::vector<std::shared_ptr<Entity>> entities;
        entities.reserve(m_entities.size());

        for (const auto &pair : m_entities)
        {
            entities.push_back(pair.second);
        }

        return entities;
    }

    std::vector<std::shared_ptr<Entity>> EntityManager::getEntitiesByName(const std::string &name) const
    {
        std::vector<std::shared_ptr<Entity>> matchingEntities;

        for (const auto &pair : m_entities)
        {
            if (pair.second->getName() == name)
            {
                matchingEntities.push_back(pair.second);
            }
        }

        return matchingEntities;
    }

    void EntityManager::update(float deltaTime)
    {
        // Update all entities if needed
    }

} // namespace Orenji