#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

// Core includes
#include "Core/EntityManager.hpp"
#include "Core/Scene.hpp"

// Resources
#include "Resources/ResourceManager.hpp"

// Forward declarations
namespace Physics
{
    class PhysicsSystem;
}

namespace Graphics
{
    class RenderSystem;
}

namespace AI
{
    class AISystem;
}

namespace UI
{
    class UIManager;
}

namespace Resources
{
    class TiledMapLoader;
}

/**
 * @brief Main engine class that coordinates all systems and manages game loop
 */
class Engine
{
public:
    /**
     * @brief Constructor
     * @param title Window title
     * @param width Window width
     * @param height Window height
     */
    Engine(const std::string &title, int width, int height);

    /**
     * @brief Destructor
     */
    ~Engine();

    /**
     * @brief Initialize the engine and all its systems
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Run the main game loop
     */
    void run();

    /**
     * @brief Shut down the engine and all systems
     */
    void shutdown();

    /**
     * @brief Get the resource manager
     * @return Reference to the resource manager
     */
    Resources::ResourceManager &getResourceManager();

    /**
     * @brief Get the window
     * @return Reference to the window
     */
    sf::RenderWindow &getWindow();

    /**
     * @brief Set the current scene
     * @param scene Shared pointer to the scene
     */
    void setScene(std::shared_ptr<Core::Scene> scene);

private:
    std::string m_title;
    int m_width;
    int m_height;
    sf::RenderWindow m_window;

    // Core systems
    std::unique_ptr<Core::EntityManager> m_entityManager;
    std::shared_ptr<Core::Scene> m_currentScene;

    // Subsystems
    std::unique_ptr<Physics::PhysicsSystem> m_physicsSystem;
    std::unique_ptr<Graphics::RenderSystem> m_renderSystem;
    std::unique_ptr<AI::AISystem> m_aiSystem;
    std::unique_ptr<UI::UIManager> m_uiManager;

    // Resource management
    std::unique_ptr<Resources::ResourceManager> m_resourceManager;
    std::unique_ptr<Resources::TiledMapLoader> m_tiledMapLoader;

    // Timing variables
    sf::Clock m_clock;
    float m_deltaTime;

    /**
     * @brief Process all input events
     */
    void processEvents();

    /**
     * @brief Update all systems
     * @param deltaTime Time since last frame in seconds
     */
    void update(float deltaTime);

    /**
     * @brief Render the current frame
     */
    void render();
};