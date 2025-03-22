#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>
#include "Component.hpp"

namespace Core
{

    /**
     * @brief Entity class representing a game object with components
     */
    class Entity
    {
    public:
        /**
         * @brief Constructor
         * @param id Unique entity ID
         * @param name Optional entity name
         */
        Entity(unsigned int id, const std::string &name = "");

        /**
         * @brief Destructor
         */
        ~Entity();

        /**
         * @brief Get the entity ID
         * @return Entity ID
         */
        unsigned int getId() const;

        /**
         * @brief Get the entity name
         * @return Entity name
         */
        const std::string &getName() const;

        /**
         * @brief Set the entity name
         * @param name New entity name
         */
        void setName(const std::string &name);

        /**
         * @brief Add a component to the entity
         * @tparam T Component type
         * @tparam Args Constructor arguments types
         * @param args Constructor arguments
         * @return Reference to the added component
         */
        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            component->setOwner(this);
            m_components[typeid(T)] = component;
            return *static_cast<T *>(component.get());
        }

        /**
         * @brief Get a component by type
         * @tparam T Component type
         * @return Pointer to the component, or nullptr if not found
         */
        template <typename T>
        T *getComponent()
        {
            auto it = m_components.find(typeid(T));
            if (it != m_components.end())
            {
                return static_cast<T *>(it->second.get());
            }
            return nullptr;
        }

        /**
         * @brief Check if the entity has a component
         * @tparam T Component type
         * @return true if the component exists
         */
        template <typename T>
        bool hasComponent()
        {
            return m_components.find(typeid(T)) != m_components.end();
        }

        /**
         * @brief Remove a component by type
         * @tparam T Component type
         * @return true if the component was removed
         */
        template <typename T>
        bool removeComponent()
        {
            auto it = m_components.find(typeid(T));
            if (it != m_components.end())
            {
                m_components.erase(it);
                return true;
            }
            return false;
        }

        /**
         * @brief Remove all components
         */
        void removeAllComponents();

        /**
         * @brief Set whether the entity is active
         * @param active Active state
         */
        void setActive(bool active);

        /**
         * @brief Check if the entity is active
         * @return true if the entity is active
         */
        bool isActive() const;

    private:
        unsigned int m_id;
        std::string m_name;
        bool m_active;
        std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
    };

} // namespace Core