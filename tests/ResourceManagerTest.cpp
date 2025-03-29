#include "Resources/ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <filesystem>

int main()
{
    // Create a window for displaying textures and sprite sheets
    sf::RenderWindow window(sf::VideoMode(800, 600), "ResourceManager Test");

    // Create resource manager
    Resources::ResourceManager resourceManager;

    // Initialize with a custom base path
    // Note: Adjust this path to where your resources are located
    resourceManager.init("resources/");

    try
    {
        // Load resources
        std::cout << "Loading resources..." << std::endl;

        // Load textures
        sf::Texture &playerTexture = resourceManager.loadTexture("player", "characters/player.png", true, false);
        sf::Texture &tilesetTexture = resourceManager.loadTexture("tileset", "tilesets/tileset.png", false, true);

        // Load sprite sheet from the player texture
        Resources::SpriteSheet &playerSheet = resourceManager.loadSpriteSheet(
            "playerSheet", "player", 32, 32, 4);

        // Load a font
        sf::Font &font = resourceManager.loadFont("main", "fonts/main.ttf");

        // Load a sound
        sf::SoundBuffer &jumpSound = resourceManager.loadSoundBuffer("jump", "sounds/jump.wav");

        // Load music
        sf::Music &backgroundMusic = resourceManager.loadMusic("background", "music/background.ogg");

        // Resource checks
        std::cout << "Resource availability:" << std::endl;
        std::cout << "- Player texture: " << (resourceManager.hasTexture("player") ? "Available" : "Not available") << std::endl;
        std::cout << "- Player sprite sheet: " << (resourceManager.hasSpriteSheet("playerSheet") ? "Available" : "Not available") << std::endl;
        std::cout << "- Main font: " << (resourceManager.hasFont("main") ? "Available" : "Not available") << std::endl;
        std::cout << "- Jump sound: " << (resourceManager.hasSoundBuffer("jump") ? "Available" : "Not available") << std::endl;
        std::cout << "- Background music: " << (resourceManager.hasMusic("background") ? "Available" : "Not available") << std::endl;

        // Create sprites to display textures and sprite sheet frames
        sf::Sprite playerSprite(playerTexture);
        playerSprite.setPosition(100, 100);

        // Create text object
        sf::Text titleText("Resource Manager Test", font, 24);
        titleText.setPosition(20, 20);

        // Create sound object
        sf::Sound jumpSfx(jumpSound);

        // Start playing background music
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();

        // Animation variables
        int currentFrame = 0;
        sf::Clock animationClock;

        // Create sprite for sprite sheet demonstration
        sf::Sprite sheetSprite;
        sheetSprite.setTexture(*playerSheet.texture);
        sheetSprite.setTextureRect(playerSheet.frames[0]);
        sheetSprite.setPosition(400, 200);
        sheetSprite.setScale(2.0f, 2.0f);

        // Create instruction text
        sf::Text instructionsText("Press Space to play jump sound\nPress Esc to quit", font, 18);
        instructionsText.setPosition(20, 500);

        // Main loop
        while (window.isOpen())
        {
            // Handle events
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }
                    else if (event.key.code == sf::Keyboard::Space)
                    {
                        // Play the jump sound
                        jumpSfx.play();
                    }
                }
            }

            // Update animation
            if (animationClock.getElapsedTime().asSeconds() > 0.25f)
            {
                currentFrame = (currentFrame + 1) % playerSheet.frames.size();
                sheetSprite.setTextureRect(playerSheet.frames[currentFrame]);
                animationClock.restart();
            }

            // Clear the window
            window.clear(sf::Color(50, 50, 50));

            // Draw everything
            window.draw(titleText);
            window.draw(playerSprite);
            window.draw(sheetSprite);
            window.draw(instructionsText);

            // Display the window contents
            window.display();
        }
    }
    catch (const Resources::ResourceLoadException &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Make sure the resource files exist in the correct directories." << std::endl;
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return 1;
    }

    return 0;
}