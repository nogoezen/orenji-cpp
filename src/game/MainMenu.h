#pragma once

#include <memory>
#include "ConsoleUI.h"
#include "TradingUI.h"
#include "GameState.h"
#include "World.h"

class MainMenu
{
private:
    ConsoleUI m_ui;

    // Show the city menu for the current location
    void showCityMenu();

    // Show the travel menu to navigate between cities
    void showTravelMenu();

    // Show the player status menu
    void showPlayerStatusMenu();

    // Show the options menu
    void showOptionsMenu();

    // Show the fleet management menu
    void showFleetMenu();

public:
    MainMenu();
    ~MainMenu() = default;

    // Run the main menu loop
    void run();

    // Show the main menu
    void showMainMenu();
};