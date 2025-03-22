#include "Scene/SceneNode.hpp"
#include <cassert>
#include <algorithm>

namespace Orenji
{
    SceneNode::SceneNode(const std::string &name)
        : m_parent(nullptr), m_name(name), m_visible(true)
    {
    }

    SceneNode *SceneNode::attachChild(Ptr child)
    {
        SceneNode *childPtr = child.get();
        childPtr->setParent(this);
        m_children.push_back(std::move(child));
        return childPtr;
    }

    SceneNode::Ptr SceneNode::detachChild(const SceneNode &node)
    {
        auto found = std::find_if(m_children.begin(), m_children.end(),
                                  [&](const Ptr &p)
                                  { return p.get() == &node; });

        assert(found != m_children.end());

        Ptr result = std::move(*found);
        result->setParent(nullptr);
        m_children.erase(found);
        return result;
    }

    SceneNode::Ptr SceneNode::detachChild(size_t index)
    {
        assert(index < m_children.size());

        Ptr result = std::move(m_children[index]);
        result->setParent(nullptr);
        m_children.erase(m_children.begin() + index);
        return result;
    }

    void SceneNode::update(float dt)
    {
        updateCurrent(dt);

        for (auto &child : m_children)
        {
            child->update(dt);
        }
    }

    void SceneNode::updateCurrent(float dt)
    {
        // Par défaut ne fait rien, à surcharger dans les classes dérivées
    }

    void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_visible)
            return;

        // Applique la transformation de ce nœud
        states.transform *= getTransform();

        // Dessine ce nœud
        drawCurrent(target, states);

        // Dessine les enfants
        for (const auto &child : m_children)
        {
            child->draw(target, states);
        }
    }

    void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Par défaut ne fait rien, à surcharger dans les classes dérivées
    }

    sf::Transform SceneNode::getWorldTransform() const
    {
        sf::Transform transform = sf::Transform::Identity;

        for (const SceneNode *node = this; node != nullptr; node = node->m_parent)
        {
            transform = node->getTransform() * transform;
        }

        return transform;
    }

    sf::Vector2f SceneNode::getWorldPosition() const
    {
        return getWorldTransform() * sf::Vector2f();
    }

    void SceneNode::setParent(SceneNode *parent)
    {
        m_parent = parent;
    }

    SceneNode *SceneNode::getParent() const
    {
        return m_parent;
    }

    size_t SceneNode::getChildCount() const
    {
        return m_children.size();
    }

    SceneNode *SceneNode::getChild(size_t index)
    {
        return (index < m_children.size()) ? m_children[index].get() : nullptr;
    }

    SceneNode *SceneNode::getChildByName(const std::string &name)
    {
        // Recherche directe dans les enfants de ce nœud
        auto found = std::find_if(m_children.begin(), m_children.end(),
                                  [&name](const Ptr &child)
                                  { return child->getName() == name; });

        if (found != m_children.end())
        {
            return found->get();
        }

        // Recherche récursive dans les enfants des enfants
        for (auto &child : m_children)
        {
            if (SceneNode *result = child->getChildByName(name))
            {
                return result;
            }
        }

        return nullptr;
    }

    const std::string &SceneNode::getName() const
    {
        return m_name;
    }

    void SceneNode::setName(const std::string &name)
    {
        m_name = name;
    }

    void SceneNode::setVisible(bool visible)
    {
        m_visible = visible;
    }

    bool SceneNode::isVisible() const
    {
        return m_visible;
    }
} // namespace Orenji