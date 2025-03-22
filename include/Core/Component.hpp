#pragma once
#include <memory>
#include <string>

namespace Orenji
{

    class Entity;

    class Component
    {
    public:
        Component() : m_owner(nullptr), m_id("") {}
        Component(Entity *entity) : m_owner(entity), m_id("") {}
        Component(const std::string &id) : m_owner(nullptr), m_id(id) {}
        Component(Entity *entity, const std::string &id) : m_owner(entity), m_id(id) {}
        virtual ~Component() = default;

        void setOwner(Entity *owner) { m_owner = owner; }
        Entity *getOwner() const { return m_owner; }

        const std::string &getId() const { return m_id; }
        void setId(const std::string &id) { m_id = id; }

        virtual void initialize() {}
        virtual void update(float deltaTime) {}
        virtual void render() {}

    private:
        Entity *m_owner;
        std::string m_id;
    };

} // namespace Orenji