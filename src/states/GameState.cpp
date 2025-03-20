#include "../../include/States/GameState.hpp"
#include "../../include/Core/SystemManager.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/Core/RenderSystem.hpp"
#include "../../include/Game.hpp"
#include "../../include/Utilities/InputHandler.hpp"
#include <iostream>

namespace Orenji
{

    GameState::GameState() : m_isPaused(false)
    {
        // Constructor
    }

    GameState::~GameState()
    {
        // Destructor
    }

    bool GameState::initialize()
    {
        // Create the entity manager
        m_entityManager = std::make_shared<EntityManager>();

        // Get the system manager and initialize it
        auto &systemManager = SystemManager::getInstance();
        systemManager.setEntityManager(m_entityManager);

        // Register core systems
        m_renderSystem = systemManager.registerSystem<RenderSystem>();

        // Initialize all systems
        systemManager.initialize();

        std::cout << "GameState initialized successfully" << std::endl;
        return true;
    }

    bool GameState::handleInput()
    {
        auto &input = InputHandler::getInstance();

        // Example of checking for key presses
        if (input.isKeyPressed(sf::Keyboard::Escape))
        {
            m_isPaused = !m_isPaused;
        }

        // Example of checking for mouse clicks
        if (input.isMouseButtonPressed(sf::Mouse::Left))
        {
            // Handle mouse click
            sf::Vector2i mousePos = input.getMousePosition();
            std::cout << "Mouse clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;
        }

        return true;
    }

    bool GameState::update(float deltaTime)
    {
        if (m_isPaused)
        {
            return true;
        }

        // Update all systems
        SystemManager::getInstance().update(deltaTime);

        return true;
    }

    bool GameState::render()
    {
        auto &window = Game::getInstance().getWindow();

        // Use the render system to draw all entities
        if (m_renderSystem)
        {
            m_renderSystem->render(window);
        }

        return true;
    }

    bool GameState::onEnter()
    {
        std::cout << "Entering GameState" << std::endl;
        return initialize();
    }

    bool GameState::onExit()
    {
        std::cout << "Exiting GameState" << std::endl;
        return true;
    }

} // namespace Orenji