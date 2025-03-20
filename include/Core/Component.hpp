#pragma once
#include <memory>

namespace Orenji
{

    class Entity;

    class Component
    {
    public:
        Component() : m_owner(nullptr) {}
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