#include "Scene/Scene.hpp"
#include "Core/Entity.hpp"
#include <algorithm>

namespace Orenji
{

    Scene::Scene(const std::string &name)
        : m_name(name), m_nextEntityId(1)
    {
    }

    Scene::~Scene()
    {
        clearEntities();
    }

    bool Scene::initialize()
    {
        for (auto &entity : m_entities)
        {
            if (!entity->initialize())
            {
                return false;
            }
        }
        return true;
    }

    void Scene::update(float deltaTime)
    {
        for (auto &entity : m_entities)
        {
            if (entity->isActive())
            {
                entity->update(deltaTime);
            }
        }
    }

    void Scene::render(sf::RenderWindow &window)
    {
        for (auto &entity : m_entities)
        {
            if (entity->isActive())
            {
                window.draw(*entity);
            }
        }
    }

    EntityPtr Scene::addEntity(EntityPtr entity)
    {
        if (!entity)
        {
            return nullptr;
        }

        m_entities.push_back(entity);
        return entity;
    }

    EntityPtr Scene::createEntity(const std::string &name)
    {
        auto entity = std::make_shared<Entity>(m_nextEntityId++, name);
        return addEntity(entity);
    }

    EntityPtr Scene::findEntityById(unsigned int id) const
    {
        auto it = std::find_if(m_entities.begin(), m_entities.end(),
                               [id](const EntityPtr &entity)
                               { return entity->getId() == id; });

        if (it != m_entities.end())
        {
            return *it;
        }

        return nullptr;
    }

    EntityPtr Scene::findEntityByName(const std::string &name) const
    {
        auto it = std::find_if(m_entities.begin(), m_entities.end(),
                               [&name](const EntityPtr &entity)
                               { return entity->getName() == name; });

        if (it != m_entities.end())
        {
            return *it;
        }

        return nullptr;
    }

    bool Scene::removeEntity(unsigned int id)
    {
        auto it = std::find_if(m_entities.begin(), m_entities.end(),
                               [id](const EntityPtr &entity)
                               { return entity->getId() == id; });

        if (it != m_entities.end())
        {
            m_entities.erase(it);
            return true;
        }

        return false;
    }

    void Scene::clearEntities()
    {
        m_entities.clear();
    }

} // namespace Orenji