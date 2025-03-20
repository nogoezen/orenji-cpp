#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "State.hpp"
#include "../Core/EntityManager.hpp"

namespace Orenji
{

    class RenderSystem;

    class GameState : public State
    {
    public:
        GameState();
        virtual ~GameState();

        bool initialize();

        virtual bool onEnter() override;
        virtual bool onExit() override;
        virtual bool update(float deltaTime) override;
        virtual bool render() override;
        virtual bool handleInput() override;

    private:
        std::shared_ptr<EntityManager> m_entityManager;
        std::shared_ptr<RenderSystem> m_renderSystem;
        bool m_isPaused;
    };

} // namespace Orenji