#include "../include/Engine.hpp"
#include "../include/Physics/PhysicsSystem.hpp"
#include "../include/Graphics/RenderSystem.hpp"
#include "../include/AI/AISystem.hpp"
#include "../include/UI/UIManager.hpp"
#include "../include/Resources/TiledMapLoader.hpp"

#include <iostream>

namespace Core
{
    Engine::Engine(const std::string &title, int width, int height)
        : m_title(title), m_width(width), m_height(height)
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
        m_window.create(sf::VideoMode(sf::Vector2u(m_width, m_height)),
                        m_title, sf::Style::Close);
        m_window.setFramerateLimit(60);

        // Initialize resource manager
        m_resourceManager = std::make_unique<Resources::ResourceManager>();

        // Initialize entity manager
        m_entityManager = std::make_unique<Core::EntityManager>();

        // Initialize physics system
        m_physicsSystem = std::make_unique<Physics::PhysicsSystem>(*m_entityManager);
        m_renderSystem = std::make_unique<Graphics::RenderSystem>(*m_entityManager, m_window);
        m_aiSystem = std::make_unique<AI::AISystem>(*m_entityManager);
        m_uiManager = std::make_unique<UI::UIManager>(m_window);

        // Initialize TiledMapLoader
        m_tiledMapLoader = std::make_unique<Resources::TiledMapLoader>(*m_resourceManager);

        return true;
    }

    void Engine::run()
    {
        if (!m_currentScene)
        {
            std::cerr << "No scene set, cannot run engine!" << std::endl;
            return;
        }

        // Initialize time tracking
        m_clock.restart();

        while (m_window.isOpen())
        {
            // Calculate delta time
            m_deltaTime = m_clock.restart().asSeconds();

            // Limit delta time to avoid issues with physics when game runs slowly
            if (m_deltaTime > 0.25f)
            {
                m_deltaTime = 0.25f;
            }

            processEvents();

            update(m_deltaTime);

            render();
        }
    }

    void Engine::shutdown()
    {
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
        // Process window events
        if (auto event = m_window.pollEvent())
        {
            // Check for window close event
            if (event->is<sf::Event::Closed>())
            {
                m_window.close();
                return;
            }

            // Check for key press event - escape to quit
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    m_window.close();
                    return;
                }
            }

            // Pass event to UI manager first
            if (m_uiManager->handleEvent(*event))
            {
                // UI consumed the event, don't pass to scene
                return;
            }

            // Pass event to current scene
            if (m_currentScene)
            {
                m_currentScene->handleEvent(*event);
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

        // Render objects via render system
        m_renderSystem->render();

        // Render current scene
        if (m_currentScene)
        {
            m_currentScene->render(m_window);
        }

        // Render UI on top
        m_uiManager->render();

        // Display the window
        m_window.display();
    }
} // namespace Core