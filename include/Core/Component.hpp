#pragma once
#include <memory>

namespace Orenji
{

    class Entity;

    class Component
    {
    public:
        Component() : m_owner(nullptr) {}
        Component(Entity *entity) : m_owner(entity) {}
        virtual ~Component() = default;

        void setOwner(Entity *owner) { m_owner = owner; }
        Entity *getOwner() const { return m_owner; }

        virtual void initialize() {}
        virtual void update(float deltaTime) {}
        virtual void render() {}

    private:
        Entity *m_owner;
    };

} // namespace Orenji