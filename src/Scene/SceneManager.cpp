#include "Scene/SceneManager.hpp"
#include <iostream>

namespace Orenji
{

    SceneManager::SceneManager()
        : m_activeSceneName("")
    {
    }

    SceneManager::~SceneManager()
    {
        m_scenes.clear();
    }

    bool SceneManager::addScene(const std::string &name, ScenePtr scene)
    {
        if (!scene)
        {
            std::cerr << "SceneManager::addScene: Scène invalide" << std::endl;
            return false;
        }

        if (hasScene(name))
        {
            std::cerr << "SceneManager::addScene: Une scène avec ce nom existe déjà: " << name << std::endl;
            return false;
        }

        m_scenes[name] = scene;
        scene->setName(name);

        // Si c'est la première scène, la définir comme active
        if (m_scenes.size() == 1)
        {
            m_activeSceneName = name;
        }

        return true;
    }

    ScenePtr SceneManager::createScene(const std::string &name)
    {
        if (hasScene(name))
        {
            std::cerr << "SceneManager::createScene: Une scène avec ce nom existe déjà: " << name << std::endl;
            return nullptr;
        }

        auto scene = std::make_shared<Scene>(name);
        if (addScene(name, scene))
        {
            return scene;
        }

        return nullptr;
    }

    bool SceneManager::removeScene(const std::string &name)
    {
        if (!hasScene(name))
        {
            return false;
        }

        m_scenes.erase(name);

        // Si la scène active a été supprimée, en définir une autre si possible
        if (m_activeSceneName == name)
        {
            m_activeSceneName = "";
            if (!m_scenes.empty())
            {
                m_activeSceneName = m_scenes.begin()->first;
            }
        }

        return true;
    }

    ScenePtr SceneManager::getScene(const std::string &name) const
    {
        auto it = m_scenes.find(name);
        if (it != m_scenes.end())
        {
            return it->second;
        }

        return nullptr;
    }

    bool SceneManager::setActiveScene(const std::string &name)
    {
        if (!hasScene(name))
        {
            std::cerr << "SceneManager::setActiveScene: Scène non trouvée: " << name << std::endl;
            return false;
        }

        m_activeSceneName = name;
        return true;
    }

    ScenePtr SceneManager::getActiveScene() const
    {
        return getScene(m_activeSceneName);
    }

    bool SceneManager::hasScene(const std::string &name) const
    {
        return m_scenes.find(name) != m_scenes.end();
    }

    void SceneManager::update(float deltaTime)
    {
        auto activeScene = getActiveScene();
        if (activeScene)
        {
            activeScene->update(deltaTime);
        }
    }

    void SceneManager::render(sf::RenderWindow &window)
    {
        auto activeScene = getActiveScene();
        if (activeScene)
        {
            activeScene->render(window);
        }
    }

} // namespace Orenji