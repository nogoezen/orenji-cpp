#include "../include/Game.hpp"
#include "../include/States/MainMenuState.hpp"
#include "../include/States/TitleState.hpp"
#include <iostream>

namespace Orenji
{

    Game::Game() : m_isRunning(false), m_deltaTime(0.0f), m_clock(), m_elapsed(), m_window(), m_stateMachine(nullptr), m_settings()
    {
        m_stateMachine = std::make_unique<StateMachine>();
    }

    Game::~Game()
    {
        ResourceManager::getInstance().clearAllResources();
    }

    Game &Game::getInstance()
    {
        static Game instance;
        return instance;
    }

    bool Game::initialize(const Settings &settings)
    {
        m_settings = settings;

        // Create the window
        sf::Uint32 style = m_settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
        m_window.create(sf::VideoMode(m_settings.windowWidth, m_settings.windowHeight),
                        m_settings.windowTitle, style);

        m_window.setFramerateLimit(m_settings.frameRateLimit);
        m_window.setVerticalSyncEnabled(m_settings.vsync);

        // Initialize state machine and add initial state
        m_stateMachine->setWindow(m_window);
        m_stateMachine->pushState(std::make_unique<TitleState>());

        m_isRunning = true;

        std::cout << "Game initialized successfully." << std::endl;
        return true;
    }

    void Game::run()
    {
        if (!m_isRunning)
        {
            std::cerr << "Game has not been initialized. Call initialize() first." << std::endl;
            return;
        }

        // Main game loop
        while (m_isRunning && m_window.isOpen())
        {
            m_elapsed = m_clock.restart();
            m_deltaTime = m_elapsed.asSeconds();

            processEvents();
            update();
            render();
        }
    }

    void Game::quit()
    {
        m_isRunning = false;
    }

    sf::RenderWindow &Game::getWindow()
    {
        return m_window;
    }

    StateMachine &Game::getStateMachine()
    {
        return *m_stateMachine;
    }

    float Game::getDeltaTime() const
    {
        return m_deltaTime;
    }

    void Game::processEvents()
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }

            // Process input events
            InputHandler::getInstance().processEvent(event);
        }

        // Handle input in the current state
        m_stateMachine->handleInput();

        // Update the input handler
        InputHandler::getInstance().update();
    }

    void Game::update()
    {
        // Update the current state
        m_stateMachine->update(m_deltaTime);
    }

    void Game::render()
    {
        // Clear the window
        m_window.clear(sf::Color(0, 0, 64, 255)); // Dark blue background

        // Render the current state
        m_stateMachine->render();

        // Display the window contents
        m_window.display();
    }

} // namespace Orenji