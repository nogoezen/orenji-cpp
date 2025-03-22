#pragma once

#include "../Core/System.hpp"

namespace AI
{

    class AISystem : public Core::System
    {
    public:
        AISystem(Core::EntityManager &entityManager);
        virtual ~AISystem();

        virtual void init() override;
        virtual void update(float deltaTime) override;
    };

} // namespace AI