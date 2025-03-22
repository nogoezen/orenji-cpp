#include "../../include/Core/EntityManager.hpp"
#include <algorithm>

namespace Core
{

    EntityManager::EntityManager()
        : m_nextEntityId(0)
    {
    }

    EntityManager::~EntityManager()
    {
        removeAllEntities();
    }

    Entity *EntityManager::createEntity(const std::string &name)
    {
        unsigned int id = m_nextEntityId++;
        auto entity = std::make_unique<Entity>(id, name);
        Entity *entityPtr = entity.get();
        m_entities[id] = std::move(entity);
        return entityPtr;
    }

    Entity *EntityManager::getEntity(unsigned int id)
    {
        auto it = m_entities.find(id);
        if (it != m_entities.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    std::vector<Entity *> EntityManager::getEntitiesByName(const std::string &name)
    {
        std::vector<Entity *> result;

        for (auto &pair : m_entities)
        {
            if (pair.second->getName() == name)
            {
                result.push_back(pair.second.get());
            }
        }

        return result;
    }

    std::vector<Entity *> EntityManager::getAllEntities()
    {
        std::vector<Entity *> result;
        result.reserve(m_entities.size());

        for (auto &pair : m_entities)
        {
            result.push_back(pair.second.get());
        }

        return result;
    }

    bool EntityManager::removeEntity(unsigned int id)
    {
        auto it = m_entities.find(id);
        if (it != m_entities.end())
        {
            m_entities.erase(it);
            return true;
        }
        return false;
    }

    void EntityManager::removeAllEntities()
    {
        m_entities.clear();
    }

    void EntityManager::update(float deltaTime)
    {
        for (auto &pair : m_entities)
        {
            auto entity = pair.second.get();
            if (entity->isActive())
            {
                // No direct update method on entity, components handle updates
            }
        }
    }

} // namespace Core