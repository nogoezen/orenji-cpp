#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <typeindex>
#include <algorithm>
#include "Component.hpp"
#include <SFML/Graphics/Transformable.hpp>

namespace Orenji
{
    class TransformComponent;

    class Entity
    {
    public:
        Entity(unsigned int id, const std::string &name = "");
        virtual ~Entity();

        // Entity identification
        unsigned int getId() const;
        std::string getName() const;
        void setName(const std::string &name);

        // Position and transform
        sf::Vector2f getPosition() const;
        void setPosition(const sf::Vector2f &position);
        void setPosition(float x, float y);
        void move(const sf::Vector2f &offset);
        void move(float offsetX, float offsetY);
        float getRotation() const;
        void setRotation(float angle);
        void rotate(float angle);
        sf::Vector2f getScale() const;
        void setScale(const sf::Vector2f &scale);
        void setScale(float x, float y);

        // Returns a reference to the transform component
        sf::Transformable &getTransform();
        const sf::Transformable &getTransform() const;

        // Component management
        template <typename T, typename... Args>
        std::shared_ptr<T> addComponent(Args &&...args);

        template <typename T>
        std::shared_ptr<T> getComponent() const;

        template <typename T>
        bool hasComponent() const;

        template <typename T>
        void removeComponent();

        void removeAllComponents();

    private:
        unsigned int m_id;
        std::string m_name;
        std::map<std::type_index, std::shared_ptr<Component>> m_components;
        sf::Transformable m_transform; // Built-in transform for position/rotation/scale
    };

    // Template implementations
    template <typename T, typename... Args>
    std::shared_ptr<T> Entity::addComponent(Args &&...args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        m_components[typeid(T)] = component;
        component->setOwner(this);
        return component;
    }

    template <typename T>
    std::shared_ptr<T> Entity::getComponent() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        auto it = m_components.find(typeid(T));
        if (it != m_components.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    template <typename T>
    bool Entity::hasComponent() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        return m_components.find(typeid(T)) != m_components.end();
    }

    template <typename T>
    void Entity::removeComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        auto it = m_components.find(typeid(T));
        if (it != m_components.end())
        {
            m_components.erase(it);
        }
    }

} // namespace Orenji