#include "EntityManager.h"

EntityManager::EntityManager()
    : m_nextEntityId(0)
{
}

EntityManager::~EntityManager()
{
    destroyAllEntities();
}

std::shared_ptr<Entity> EntityManager::createEntity(const std::string &name)
{
    auto entity = std::make_shared<Entity>(m_nextEntityId++, name);
    entity->setManager(this);
    m_entities[entity->getId()] = entity;
    return entity;
}

void EntityManager::destroyEntity(Entity::EntityId entityId)
{
    m_entitiesToDestroy.push_back(entityId);
}

void EntityManager::destroyEntity(std::shared_ptr<Entity> entity)
{
    if (entity)
    {
        destroyEntity(entity->getId());
    }
}

void EntityManager::destroyAllEntities()
{
    m_entities.clear();
    m_entitiesToDestroy.clear();
}

std::shared_ptr<Entity> EntityManager::getEntity(Entity::EntityId entityId) const
{
    auto it = m_entities.find(entityId);
    if (it != m_entities.end())
    {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Entity>> EntityManager::getEntities() const
{
    std::vector<std::shared_ptr<Entity>> entities;
    entities.reserve(m_entities.size());

    for (const auto &pair : m_entities)
    {
        entities.push_back(pair.second);
    }

    return entities;
}

void EntityManager::update(float deltaTime)
{
    processEntityRemovals();

    for (auto &pair : m_entities)
    {
        auto &entity = pair.second;
        if (entity->isActive())
        {
            for (auto &componentPair : entity->m_components)
            {
                componentPair.second->update(deltaTime);
            }
        }
    }
}

void EntityManager::render()
{
    for (auto &pair : m_entities)
    {
        auto &entity = pair.second;
        if (entity->isActive())
        {
            for (auto &componentPair : entity->m_components)
            {
                componentPair.second->render();
            }
        }
    }
}

void EntityManager::processEntityRemovals()
{
    for (auto entityId : m_entitiesToDestroy)
    {
        m_entities.erase(entityId);
    }
    m_entitiesToDestroy.clear();
}