#include "include/Engine.hpp"
#include "include/Scenes/MainMenuScene.hpp"
#include <iostream>
#include <memory>

int main()
{
    try
    {
        // Create engine instance
        Engine engine("SFML 3 Game Engine", 1024, 768);

        // Initialize engine
        if (!engine.initialize())
        {
            std::cerr << "Failed to initialize engine" << std::endl;
            return 1;
        }

        // Create main menu scene
        auto mainMenuScene = std::make_shared<Scenes::MainMenuScene>(engine);

        // Set the scene
        engine.setScene(mainMenuScene);

        // Run the game loop
        engine.run();

        // Clean up
        engine.shutdown();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}