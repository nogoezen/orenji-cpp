#include "Scene/Scene.hpp"
#include "Core/Entity.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

// À inclure plus tard pour la sérialisation JSON
// #include <nlohmann/json.hpp>

namespace Orenji
{

    Scene::Scene(const std::string &name)
        : m_name(name), m_isActive(false)
    {
        // Crée le nœud racine
        m_rootNode = std::make_unique<SceneNode>("root");
        registerNode(m_rootNode.get());
    }

    Scene::~Scene()
    {
        clear();
    }

    bool Scene::initialize()
    {
        // Initialisation de base, peut être étendue dans les classes dérivées
        return true;
    }

    void Scene::update(float dt)
    {
        if (m_isActive && m_rootNode)
        {
            m_rootNode->update(dt);
        }
    }

    void Scene::render(sf::RenderTarget &target)
    {
        if (m_isActive && m_rootNode)
        {
            target.draw(*m_rootNode);
        }
    }

    SceneNode *Scene::getRootNode()
    {
        return m_rootNode.get();
    }

    const std::string &Scene::getName() const
    {
        return m_name;
    }

    void Scene::setName(const std::string &name)
    {
        m_name = name;
    }

    SceneNode *Scene::findNodeByName(const std::string &name)
    {
        auto it = m_nodes.find(name);
        return (it != m_nodes.end()) ? it->second : nullptr;
    }

    SceneNode *Scene::addNode(SceneNode::Ptr node, const std::string &parent)
    {
        if (!node)
            return nullptr;

        SceneNode *nodePtr = node.get();
        registerNode(nodePtr);

        SceneNode *parentNode = parent.empty() ? m_rootNode.get() : findNodeByName(parent);
        if (!parentNode)
        {
            std::cerr << "Erreur : parent '" << parent << "' non trouvé pour le nœud '" << node->getName() << "'" << std::endl;
            parentNode = m_rootNode.get(); // Fallback à la racine
        }

        parentNode->attachChild(std::move(node));
        return nodePtr;
    }

    SceneNode *Scene::createNode(const std::string &name, const std::string &parent)
    {
        auto node = std::make_unique<SceneNode>(name);
        return addNode(std::move(node), parent);
    }

    bool Scene::removeNode(const std::string &name)
    {
        SceneNode *node = findNodeByName(name);
        if (!node || node == m_rootNode.get())
            return false;

        SceneNode *parent = node->getParent();
        if (!parent)
            return false;

        unregisterNode(node);
        parent->detachChild(*node);
        return true;
    }

    bool Scene::loadFromFile(const std::string &filename)
    {
        // À implémenter avec nlohmann/json
        std::cerr << "Warning: Scene::loadFromFile not implemented yet" << std::endl;
        return false;
    }

    bool Scene::saveToFile(const std::string &filename)
    {
        // À implémenter avec nlohmann/json
        std::cerr << "Warning: Scene::saveToFile not implemented yet" << std::endl;
        return false;
    }

    void Scene::clear()
    {
        m_nodes.clear();

        // Crée un nouveau nœud racine
        m_rootNode = std::make_unique<SceneNode>("root");
        registerNode(m_rootNode.get());
    }

    void Scene::onActivate()
    {
        m_isActive = true;
    }

    void Scene::onDeactivate()
    {
        m_isActive = false;
    }

    void Scene::registerNode(SceneNode *node)
    {
        if (node)
        {
            // Si le nom est déjà pris, génère un nom unique
            std::string name = node->getName();
            if (m_nodes.find(name) != m_nodes.end())
            {
                int counter = 1;
                std::string baseName = name;
                do
                {
                    name = baseName + "_" + std::to_string(counter++);
                } while (m_nodes.find(name) != m_nodes.end());

                node->setName(name);
            }

            m_nodes[name] = node;
        }
    }

    void Scene::unregisterNode(SceneNode *node)
    {
        if (node)
        {
            auto it = m_nodes.find(node->getName());
            if (it != m_nodes.end() && it->second == node)
            {
                m_nodes.erase(it);
            }
        }
    }

} // namespace Orenji