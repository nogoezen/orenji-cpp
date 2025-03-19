#pragma once

#include "../core/State.h"
#include "../models/Player.h"
#include "../models/World.h"
#include "../core/TradingSystem.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>

class GameState : public State
{
public:
    GameState();
    ~GameState() override = default;

    void enter() override;
    void exit() override;
    void pause() override;
    void resume() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    // Getters pour les systèmes principaux
    std::shared_ptr<Player> getPlayer() const { return m_player; }
    std::shared_ptr<World> getWorld() const { return m_world; }
    std::shared_ptr<TradingSystem> getTradingSystem() const { return m_tradingSystem; }

protected:
    // Systèmes principaux du jeu
    std::shared_ptr<Player> m_player;
    std::shared_ptr<World> m_world;
    std::shared_ptr<TradingSystem> m_tradingSystem;

    // Interface utilisateur
    std::shared_ptr<sf::Font> m_font;
    std::shared_ptr<sf::Text> m_statusText;

    // Méthodes utilitaires
    void initializeSystems();
    void updateStatusText();
    void handleGameInput();
};