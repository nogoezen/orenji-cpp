#include "Core/EntityManager.hpp"

namespace Orenji
{
    EntityManager::EntityManager()
    {
    }

    EntityManager::~EntityManager()
    {
        clear();
    }

    void EntityManager::update(float dt)
    {
        for (auto &pair : m_entities)
        {
            if (pair.second && pair.second->isActive())
            {
                pair.second->update(dt);
            }
        }
    }

    void EntityManager::draw(sf::RenderTarget &target)
    {
        for (auto &pair : m_entities)
        {
            if (pair.second && pair.second->isActive())
            {
                target.draw(*pair.second);
            }
        }
    }

    EntityPtr EntityManager::createEntity(const std::string &id)
    {
        // Vérifier si l'entité existe déjà
        auto it = m_entities.find(id);
        if (it != m_entities.end())
        {
            return it->second;
        }

        // Créer une nouvelle entité
        EntityPtr entity = std::make_shared<Entity>(id);

        // Stocker l'entité
        m_entities[id] = entity;

        return entity;
    }

    EntityPtr EntityManager::getEntity(const std::string &id)
    {
        auto it = m_entities.find(id);
        if (it != m_entities.end())
        {
            return it->second;
        }
        return nullptr;
    }

    std::vector<EntityPtr> EntityManager::getAllEntities() const
    {
        std::vector<EntityPtr> entities;
        entities.reserve(m_entities.size());

        for (const auto &pair : m_entities)
        {
            entities.push_back(pair.second);
        }

        return entities;
    }

    std::vector<EntityPtr> EntityManager::getActiveEntities() const
    {
        std::vector<EntityPtr> entities;

        for (const auto &pair : m_entities)
        {
            if (pair.second && pair.second->isActive())
            {
                entities.push_back(pair.second);
            }
        }

        return entities;
    }

    bool EntityManager::removeEntity(const std::string &id)
    {
        auto it = m_entities.find(id);
        if (it != m_entities.end())
        {
            m_entities.erase(it);
            return true;
        }
        return false;
    }

    void EntityManager::clear()
    {
        m_entities.clear();
    }

} // namespace Orenji