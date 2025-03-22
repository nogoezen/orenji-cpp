#pragma once

#include "../Core/System.hpp"

namespace Physics
{

    class PhysicsSystem : public Core::System
    {
    public:
        PhysicsSystem(Core::EntityManager &entityManager);
        virtual ~PhysicsSystem();

        virtual void init() override;
        virtual void update(float deltaTime) override;
    };

} // namespace Physics