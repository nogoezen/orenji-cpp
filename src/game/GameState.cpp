#include "GameState.h"
#include <iostream>

// Initialize static instance
std::unique_ptr<GameState> GameState::s_instance = nullptr;

GameState::GameState()
    : m_currentDay(1), m_currentLocationId(1), m_isRunning(true)
{
    // Constructor
}

GameState &GameState::getInstance()
{
    if (!s_instance)
    {
        s_instance = std::unique_ptr<GameState>(new GameState());
    }
    return *s_instance;
}

void GameState::initialize()
{
    // Initialize player if needed
    if (!m_player)
    {
        m_player = std::make_shared<Player>("Player");
    }

    // Initialize trading system
    TradingSystem::getInstance().initialize();

    // Initialize world
    World::getInstance().initialize();

    // Set initial values
    m_currentDay = 1;
    m_currentLocationId = 1; // Assuming 1 is a valid starting location
    m_isRunning = true;
}

void GameState::update()
{
    // Update all game systems that need regular updates

    // Update the trading system with the current day
    TradingSystem::getInstance().updateMarkets(m_currentDay);

    // Update the world
    World::getInstance().update();

    // Update any other systems that need regular updates
}

void GameState::advanceTime(int days)
{
    if (days <= 0)
        return;

    // Update the day counter
    m_currentDay += days;

    // Update game state for the new day
    update();

    // Log the time advancement
    std::cout << "Jour " << m_currentDay << std::endl;
}

bool GameState::travelToLocation(int destinationId)
{
    // Get current location
    auto &world = World::getInstance();
    const auto &currentLocation = world.findCityById(m_currentLocationId);
    if (!currentLocation)
    {
        std::cout << "Emplacement actuel invalide." << std::endl;
        return false;
    }

    // Get destination location
    const auto &destination = world.findCityById(destinationId);
    if (!destination)
    {
        std::cout << "Destination invalide." << std::endl;
        return false;
    }

    // Calculate travel time (simple version)
    float dx = static_cast<float>(destination->posX - currentLocation->posX);
    float dy = static_cast<float>(destination->posY - currentLocation->posY);
    float distance = std::sqrt(dx * dx + dy * dy);

    // Travel time depends on player's speed (simple formula)
    float speed = m_player->getSpeed();
    if (speed <= 0)
        speed = 1.0f; // Minimum speed

    int travelDays = std::max(1, static_cast<int>(distance / speed));

    // Confirm travel with the player
    std::cout << "Voyager vers " << destination->name << " prendra " << travelDays << " jours." << std::endl;
    std::cout << "Voulez-vous continuer ? (O/N) : ";
    char response;
    std::cin >> response;
    std::cin.ignore(); // Clear input buffer

    if (response != 'O' && response != 'o')
    {
        std::cout << "Voyage annulé." << std::endl;
        return false;
    }

    // Advance time and update location
    advanceTime(travelDays);

    // Set new location
    m_currentLocationId = destinationId;
    m_player->setPosition(destination->posX, destination->posY);

    std::cout << "Vous êtes arrivé à " << destination->name << "." << std::endl;

    return true;
}