#include "include/Game.hpp"
#include <iostream>

int main()
{
    // Create and initialize the game
    Orenji::Game &game = Orenji::Game::getInstance();

    Orenji::Game::Settings settings;
    settings.windowTitle = "Orenji Engine";
    settings.windowWidth = 1280;
    settings.windowHeight = 720;
    settings.frameRateLimit = 60;

    try
    {
        if (game.initialize(settings))
        {
            // Start the game
            game.run();
        }
        else
        {
            std::cerr << "Failed to initialize game." << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}