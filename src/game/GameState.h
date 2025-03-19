#pragma once

#include <memory>
#include <string>
#include "../models/Player.h"
#include "TradingSystem.h"
#include "World.h"

// Class to manage the game state
class GameState
{
private:
    // Singleton instance
    static std::unique_ptr<GameState> s_instance;

    // Private constructor for singleton
    GameState();

    // Game time
    int m_currentDay;

    // Current player
    std::shared_ptr<Player> m_player;

    // Current location ID
    int m_currentLocationId;

    // Is the game running?
    bool m_isRunning;

public:
    // Get singleton instance
    static GameState &getInstance();

    // Initialize the game state
    void initialize();

    // Update game state (call once per game loop)
    void update();

    // Advance game time by specified days
    void advanceTime(int days = 1);

    // Get current day
    int getCurrentDay() const { return m_currentDay; }

    // Get/set player
    std::shared_ptr<Player> getPlayer() { return m_player; }
    void setPlayer(std::shared_ptr<Player> player) { m_player = player; }

    // Get/set current location
    int getCurrentLocationId() const { return m_currentLocationId; }
    void setCurrentLocationId(int locationId) { m_currentLocationId = locationId; }

    // Game control
    bool isRunning() const { return m_isRunning; }
    void setRunning(bool running) { m_isRunning = running; }
    void quitGame() { m_isRunning = false; }

    // Travel to a new location
    bool travelToLocation(int destinationId);
};