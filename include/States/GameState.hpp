#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "State.hpp"
#include "../Core/EntityManager.hpp"
#include "../TiledMap/TiledMap.hpp"

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
        std::shared_ptr<TiledMap> m_tiledMap; // Carte Tiled chargée
        bool m_isPaused;
    };

} // namespace Orenji