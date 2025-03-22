#include "../include/Engine.hpp"
#include "../include/Physics/PhysicsSystem.hpp"
#include "../include/Graphics/RenderSystem.hpp"
#include "../include/AI/AISystem.hpp"
#include "../include/UI/UIManager.hpp"
#include "../include/Resources/TiledMapLoader.hpp"

#include <iostream>

Engine::Engine(const std::string &title, int width, int height)
    : m_title(title), m_width(width), m_height(height), m_deltaTime(0.0f)
{
    std::cout << "Engine created" << std::endl;
}

Engine::~Engine()
{
    shutdown();
    std::cout << "Engine destroyed" << std::endl;
}

bool Engine::initialize()
{
    // Create window
    m_window.create(sf::VideoMode(sf::Vector2u(m_width, m_height)), m_title, sf::Style::Close);
    m_window.setVerticalSyncEnabled(true);

    // Initialize resource manager
    m_resourceManager = std::make_unique<Resources::ResourceManager>();

    // Initialize entity manager
    m_entityManager = std::make_unique<Core::EntityManager>();

    // Initialize subsystems
    m_physicsSystem = std::make_unique<Physics::PhysicsSystem>(*m_entityManager);
    m_renderSystem = std::make_unique<Graphics::RenderSystem>(*m_entityManager, m_window);
    m_aiSystem = std::make_unique<AI::AISystem>(*m_entityManager);
    m_uiManager = std::make_unique<UI::UIManager>(m_window);

    // Initialize tiled map loader
    m_tiledMapLoader = std::make_unique<Resources::TiledMapLoader>(*m_resourceManager);

    std::cout << "Engine initialized" << std::endl;
    return true;
}

void Engine::run()
{
    if (!m_currentScene)
    {
        std::cerr << "No scene set! Can't run the engine." << std::endl;
        return;
    }

    // Reset the clock
    m_clock.restart();

    while (m_window.isOpen())
    {
        // Calculate delta time
        m_deltaTime = m_clock.restart().asSeconds();

        // Cap delta time to prevent physics issues on lag spikes
        if (m_deltaTime > 0.25f)
        {
            m_deltaTime = 0.25f;
        }

        // Process events
        processEvents();

        // Update systems
        update(m_deltaTime);

        // Render frame
        render();
    }
}

void Engine::shutdown()
{
    // Destroy systems in reverse order of creation
    m_tiledMapLoader.reset();
    m_uiManager.reset();
    m_aiSystem.reset();
    m_renderSystem.reset();
    m_physicsSystem.reset();
    m_entityManager.reset();
    m_resourceManager.reset();

    if (m_window.isOpen())
    {
        m_window.close();
    }

    std::cout << "Engine shut down" << std::endl;
}

Resources::ResourceManager &Engine::getResourceManager()
{
    return *m_resourceManager;
}

sf::RenderWindow &Engine::getWindow()
{
    return m_window;
}

void Engine::setScene(std::shared_ptr<Core::Scene> scene)
{
    m_currentScene = scene;

    if (m_currentScene)
    {
        m_currentScene->init();
    }
}

void Engine::processEvents()
{
    // In SFML 3, pollEvent returns an optional<Event>
    while (auto eventOpt = m_window.pollEvent())
    {
        sf::Event &event = eventOpt.value();

        // Check if event is closed
        if (event.is<sf::Event::Closed>())
        {
            m_window.close();
        }

        // Let UI handle events first
        if (m_uiManager->handleEvent(event))
        {
            continue; // Event was handled by UI
        }

        // Let current scene handle events
        if (m_currentScene)
        {
            m_currentScene->handleEvent(event);
        }
    }
}

void Engine::update(float deltaTime)
{
    // Update physics
    m_physicsSystem->update(deltaTime);

    // Update AI
    m_aiSystem->update(deltaTime);

    // Update current scene
    if (m_currentScene)
    {
        m_currentScene->update(deltaTime);
    }

    // Update UI
    m_uiManager->update(deltaTime);
}

void Engine::render()
{
    // Clear the window
    m_window.clear(sf::Color(40, 40, 40));

    // Let the render system draw all entities
    m_renderSystem->render();

    // Let the current scene draw anything specific
    if (m_currentScene)
    {
        m_currentScene->render(m_window);
    }

    // Draw UI on top
    m_uiManager->render();

    // Display the window
    m_window.display();
}