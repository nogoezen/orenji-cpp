#include "../../include/Core/Scene.hpp"

namespace Core
{

    Scene::Scene(const std::string &name)
        : m_name(name), m_entityManager(nullptr)
    {
    }

    Scene::~Scene()
    {
    }

    const std::string &Scene::getName() const
    {
        return m_name;
    }

    void Scene::setEntityManager(EntityManager *entityManager)
    {
        m_entityManager = entityManager;
    }

    EntityManager *Scene::getEntityManager() const
    {
        return m_entityManager;
    }

} // namespace Core