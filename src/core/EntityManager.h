#pragma once

#include "Entity.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    std::shared_ptr<Entity> createEntity(const std::string &name = "");
    void destroyEntity(Entity::EntityId entityId);
    void destroyEntity(std::shared_ptr<Entity> entity);

    void destroyAllEntities();

    std::shared_ptr<Entity> getEntity(Entity::EntityId entityId) const;

    std::vector<std::shared_ptr<Entity>> getEntities() const;

    template <typename T>
    std::vector<std::shared_ptr<Entity>> getEntitiesWithComponent() const;

    template <typename... ComponentTypes>
    std::vector<std::shared_ptr<Entity>> getEntitiesWithComponents() const;

    void update(float deltaTime);
    void render();

private:
    Entity::EntityId m_nextEntityId;
    std::unordered_map<Entity::EntityId, std::shared_ptr<Entity>> m_entities;
    std::vector<Entity::EntityId> m_entitiesToDestroy;

    void processEntityRemovals();
};

// Template implementation
template <typename T>
std::vector<std::shared_ptr<Entity>> EntityManager::getEntitiesWithComponent() const
{
    std::vector<std::shared_ptr<Entity>> result;
    for (const auto &pair : m_entities)
    {
        if (pair.second->hasComponent<T>())
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

template <typename... ComponentTypes>
std::vector<std::shared_ptr<Entity>> EntityManager::getEntitiesWithComponents() const
{
    std::vector<std::shared_ptr<Entity>> result;
    for (const auto &pair : m_entities)
    {
        if ((pair.second->hasComponent<ComponentTypes>() && ...))
        {
            result.push_back(pair.second);
        }
    }
    return result;
}