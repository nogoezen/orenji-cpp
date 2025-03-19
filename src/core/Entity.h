#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <typeindex>
#include <functional>

class Component;
class EntityManager;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
    using EntityId = std::size_t;

    Entity(EntityId id, std::string name = "");
    virtual ~Entity();

    EntityId getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

    // Component management
    template <typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args &&...args);

    template <typename T>
    std::shared_ptr<T> getComponent() const;

    template <typename T>
    bool hasComponent() const;

    template <typename T>
    bool removeComponent();

    void removeAllComponents();

    // For the entity system
    friend class EntityManager;

private:
    EntityId m_id;
    std::string m_name;
    bool m_active;
    std::map<std::type_index, std::shared_ptr<Component>> m_components;
    EntityManager *m_manager;

    void setManager(EntityManager *manager) { m_manager = manager; }
};

class Component
{
public:
    Component() {}
    virtual ~Component() = default;

    virtual void initialize() {}
    virtual void update(float deltaTime) {}
    virtual void render() {}

    std::shared_ptr<Entity> getEntity() const { return m_entity.lock(); }

protected:
    friend class Entity;
    std::weak_ptr<Entity> m_entity;

    void setEntity(std::shared_ptr<Entity> entity)
    {
        m_entity = entity;
    }
};

// Template implementation
template <typename T, typename... Args>
std::shared_ptr<T> Entity::addComponent(Args &&...args)
{
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    auto typeId = std::type_index(typeid(T));
    if (m_components.find(typeId) != m_components.end())
    {
        return std::dynamic_pointer_cast<T>(m_components[typeId]);
    }

    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    component->setEntity(shared_from_this());
    component->initialize();

    m_components[typeId] = component;
    return component;
}

template <typename T>
std::shared_ptr<T> Entity::getComponent() const
{
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    auto typeId = std::type_index(typeid(T));
    auto it = m_components.find(typeId);
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
    return m_components.find(std::type_index(typeid(T))) != m_components.end();
}

template <typename T>
bool Entity::removeComponent()
{
    static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

    auto typeId = std::type_index(typeid(T));
    auto it = m_components.find(typeId);
    if (it != m_components.end())
    {
        m_components.erase(it);
        return true;
    }
    return false;
}