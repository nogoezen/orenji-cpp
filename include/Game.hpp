#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "States/StateMachine.hpp"
#include "Utilities/ResourceManager.hpp"
#include "Utilities/InputHandler.hpp"

namespace Orenji
{
    // Configuration settings - déplacé en dehors de la classe Game
    struct GameSettings
    {
        std::string windowTitle = "Orenji Game Engine";
        unsigned int windowWidth = 800;
        unsigned int windowHeight = 600;
        bool fullscreen = false;
        unsigned int frameRateLimit = 60;
        bool vsync = true;
    };

    class Game
    {
    public:
        // Alias pour la compatibilité
        using Settings = GameSettings;

        // Singleton pattern
        static Game &getInstance();

        // Initialization and execution
        bool initialize(const Settings &settings = Settings());
        void run();
        void quit();

        // Resource access
        sf::RenderWindow &getWindow();
        StateMachine &getStateMachine();

        // Delta time for frame-rate independent movement
        float getDeltaTime() const;

    private:
        Game();
        ~Game();

        // Prevent copying
        Game(const Game &) = delete;
        Game &operator=(const Game &) = delete;

        // Game loop methods
        void processEvents();
        void update();
        void render();

        // Member variables
        sf::RenderWindow m_window;
        std::unique_ptr<StateMachine> m_stateMachine;

        sf::Clock m_clock;
        sf::Time m_elapsed;
        float m_deltaTime;

        bool m_isRunning;
        Settings m_settings;
    };

} // namespace Orenji