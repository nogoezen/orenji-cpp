#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Component.hpp"

namespace Orenji
{

    /**
     * @class Entity
     * @brief Represents a game entity in the ECS system
     */
    class Entity : public std::enable_shared_from_this<Entity>, public sf::Transformable, public sf::Drawable
    {
    public:
        /**
         * @brief Constructor
         * @param id Unique identifier for the entity
         */
        explicit Entity(unsigned int id);

        /**
         * @brief Virtual destructor
         */
        virtual ~Entity() = default;

        /**
         * @brief Get entity ID
         * @return The unique ID of the entity
         */
        unsigned int getId() const { return m_id; }

        /**
         * @brief Set entity activity state
         * @param active Whether the entity is active
         */
        void setActive(bool active) { m_active = active; }

        /**
         * @brief Check if entity is active
         * @return True if entity is active
         */
        bool isActive() const { return m_active; }

        /**
         * @brief Add a component to the entity
         * @tparam T Component type
         * @tparam Args Constructor argument types
         * @param args Constructor arguments
         * @return Shared pointer to the added component
         */
        template <typename T, typename... Args>
        std::shared_ptr<T> addComponent(Args &&...args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            auto baseComponent = std::static_pointer_cast<Component>(component);
            baseComponent->initialize(shared_from_this());

            m_components[typeid(T)] = baseComponent;
            return component;
        }

        /**
         * @brief Check if entity has a specific component
         * @tparam T Component type
         * @return True if entity has component of type T
         */
        template <typename T>
        bool hasComponent() const
        {
            return m_components.find(typeid(T)) != m_components.end();
        }

        /**
         * @brief Get a component of specific type
         * @tparam T Component type
         * @return Shared pointer to the component or nullptr if not found
         */
        template <typename T>
        std::shared_ptr<T> getComponent()
        {
            auto it = m_components.find(typeid(T));
            if (it != m_components.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        /**
         * @brief Remove a component of specific type
         * @tparam T Component type
         * @return True if component was removed
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
         * @brief Update all components
         * @param deltaTime Time elapsed since last update
         */
        virtual void update(float deltaTime);

        /**
         * @brief Render the entity and its components
         * @param target Render target
         * @param states Render states
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        unsigned int m_id;                                              // Unique identifier
        bool m_active = true;                                           // Whether the entity is active
        std::unordered_map<std::type_index, ComponentPtr> m_components; // Components map
    };

    using EntityPtr = std::shared_ptr<Entity>;

} // namespace Orenji