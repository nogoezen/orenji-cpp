#include "Scene/SceneManager.hpp"
#include <iostream>
#include <algorithm>

namespace Orenji
{

    SceneManager::SceneManager()
    {
    }

    SceneManager::~SceneManager()
    {
        m_scenes.clear();
        m_activeScenes.clear();
    }

    bool SceneManager::initialize()
    {
        return true;
    }

    void SceneManager::addScene(const std::string &id, ScenePtr scene)
    {
        if (scene)
        {
            m_scenes[id] = std::move(scene);
        }
    }

    Scene *SceneManager::createScene(const std::string &id, const std::string &sceneName)
    {
        auto it = m_factories.find(id);
        if (it == m_factories.end())
        {
            std::cerr << "Erreur : type de scène '" << id << "' non enregistré" << std::endl;
            return nullptr;
        }

        auto scene = it->second();
        if (!scene)
        {
            std::cerr << "Erreur : échec de création de la scène de type '" << id << "'" << std::endl;
            return nullptr;
        }

        scene->setName(sceneName);
        if (!scene->initialize())
        {
            std::cerr << "Erreur : échec d'initialisation de la scène '" << sceneName << "'" << std::endl;
            return nullptr;
        }

        Scene *scenePtr = scene.get();
        m_scenes[sceneName] = std::move(scene);
        return scenePtr;
    }

    bool SceneManager::activateScene(const std::string &id, bool exclusive)
    {
        auto it = m_scenes.find(id);
        if (it == m_scenes.end())
            return false;

        if (exclusive)
        {
            // Désactive toutes les scènes actuellement actives
            for (const auto &activeId : m_activeScenes)
            {
                auto sceneIt = m_scenes.find(activeId);
                if (sceneIt != m_scenes.end())
                {
                    sceneIt->second->onDeactivate();
                }
            }
            m_activeScenes.clear();
        }
        else if (isSceneActive(id))
        {
            // La scène est déjà active
            return true;
        }

        // Active la scène
        it->second->onActivate();
        m_activeScenes.push_back(id);
        return true;
    }

    bool SceneManager::deactivateScene(const std::string &id)
    {
        auto it = std::find(m_activeScenes.begin(), m_activeScenes.end(), id);
        if (it == m_activeScenes.end())
            return false;

        auto sceneIt = m_scenes.find(id);
        if (sceneIt != m_scenes.end())
        {
            sceneIt->second->onDeactivate();
        }

        m_activeScenes.erase(it);
        return true;
    }

    bool SceneManager::removeScene(const std::string &id)
    {
        // Désactive d'abord la scène si elle est active
        deactivateScene(id);

        // Supprime la scène
        auto it = m_scenes.find(id);
        if (it == m_scenes.end())
            return false;

        m_scenes.erase(it);
        return true;
    }

    Scene *SceneManager::getScene(const std::string &id)
    {
        auto it = m_scenes.find(id);
        return (it != m_scenes.end()) ? it->second.get() : nullptr;
    }

    bool SceneManager::isSceneActive(const std::string &id) const
    {
        return std::find(m_activeScenes.begin(), m_activeScenes.end(), id) != m_activeScenes.end();
    }

    void SceneManager::update(float dt)
    {
        // Met à jour toutes les scènes actives
        for (const auto &id : m_activeScenes)
        {
            auto it = m_scenes.find(id);
            if (it != m_scenes.end())
            {
                it->second->update(dt);
            }
        }
    }

    void SceneManager::render(sf::RenderTarget &target)
    {
        // Dessine toutes les scènes actives dans l'ordre
        for (const auto &id : m_activeScenes)
        {
            auto it = m_scenes.find(id);
            if (it != m_scenes.end())
            {
                it->second->render(target);
            }
        }
    }

    bool SceneManager::loadSceneFromFile(const std::string &filename, const std::string &id)
    {
        // Crée une nouvelle scène vide
        auto scene = std::make_unique<Scene>(id);

        // Charge la scène depuis le fichier
        if (!scene->loadFromFile(filename))
        {
            std::cerr << "Erreur : échec du chargement de la scène depuis '" << filename << "'" << std::endl;
            return false;
        }

        // Ajoute la scène
        m_scenes[id] = std::move(scene);
        return true;
    }

    bool SceneManager::saveSceneToFile(const std::string &id, const std::string &filename)
    {
        auto it = m_scenes.find(id);
        if (it == m_scenes.end())
        {
            std::cerr << "Erreur : scène '" << id << "' non trouvée" << std::endl;
            return false;
        }

        // Sauvegarde la scène dans le fichier
        return it->second->saveToFile(filename);
    }

} // namespace Orenji