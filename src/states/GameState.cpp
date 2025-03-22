#include "../../include/States/GameState.hpp"
#include "../../include/components/SpriteComponent.hpp"
#include "../../include/components/TransformComponent.hpp"
#include "../../include/Core/RenderSystem.hpp"
#include "../../include/Physics/PhysicsWorld.hpp"
#include "../../include/Physics/PhysicsComponent.hpp"
#include "../../include/TiledMap/TiledMapLoader.hpp"
#include "../../include/TiledMap/TiledMapRenderer.hpp"
#include <iostream>
#include <SFML/Window/Event.hpp>

namespace Orenji
{
        GameState::GameState() : m_isPaused(false), m_tiledMap(nullptr)
        {
                m_entityManager = std::make_shared<EntityManager>();
                m_renderSystem = std::make_shared<RenderSystem>();
                m_renderSystem->setEntityManager(m_entityManager);
                m_renderSystem->setName("RenderSystem");
        }

        GameState::~GameState()
        {
                // Cleanup resources
                if (m_entityManager)
                {
                        m_entityManager->removeAllEntities();
                }

                // Unload map resources
                if (m_tiledMap)
                {
                        TiledMapLoader::unload(m_tiledMap->getFilePath());
                        m_tiledMap = nullptr;
                }
        }

        bool GameState::initialize()
        {
                std::cout << "Initializing GameState..." << std::endl;

                // Initialize systems
                m_renderSystem->initialize();

                // Load the game map
                try
                {
                        // Set the root directory for TiledMap assets
                        TiledMapLoader::setRootDirectory("bin/resources/maps");

                        // Load the map (replace "level1.tmx" with your actual map file)
                        m_tiledMap = TiledMapLoader::load("level1.tmx");

                        if (!m_tiledMap || !m_tiledMap->isLoaded())
                        {
                                std::cerr << "Failed to load the map!" << std::endl;
                                // Continue without a map - will use a solid color background
                        }
                        else
                        {
                                std::cout << "Map loaded successfully: " << m_tiledMap->getWidth() << "x" << m_tiledMap->getHeight() << " tiles" << std::endl;

                                // Create collision objects from the map if you have a collision layer
                                // TODO: Add collision initialization code here
                        }
                }
                catch (const std::exception &e)
                {
                        std::cerr << "Error loading map: " << e.what() << std::endl;
                        // Continue without a map
                }

                // Create a player entity
                auto playerEntity = m_entityManager->createEntity("Player");

                // Add components to the player entity
                auto spriteComponent = playerEntity->addComponent<SpriteComponent>();
                if (!spriteComponent->loadTexture("bin/resources/textures/player.png"))
                {
                        std::cout << "Failed to load player texture!" << std::endl;
                        // Use a placeholder texture or color
                        sf::RectangleShape shape(sf::Vector2f(50, 50));
                        shape.setFillColor(sf::Color::Red);
                        spriteComponent->setShape(shape);
                }

                auto transformComponent = playerEntity->addComponent<TransformComponent>();
                transformComponent->setPosition(400.0f, 300.0f); // Center of a typical 800x600 window

                // Add physics component if physics is enabled
                auto physicsComponent = playerEntity->addComponent<PhysicsComponent>();
                physicsComponent->initialize();

                std::cout << "GameState initialized successfully." << std::endl;
                return true;
        }

        bool GameState::onEnter()
        {
                std::cout << "Entering GameState..." << std::endl;
                return initialize();
        }

        bool GameState::onExit()
        {
                std::cout << "Exiting GameState..." << std::endl;
                return true;
        }

        bool GameState::update(float deltaTime)
        {
                if (m_isPaused)
                {
                        return true;
                }

                // Update the map if needed
                if (m_tiledMap)
                {
                        m_tiledMap->update(deltaTime);
                }

                // Update entity manager (which updates all entities and their components)
                m_entityManager->update(deltaTime);

                // Update render system
                m_renderSystem->update(deltaTime);

                return true;
        }

        bool GameState::render()
        {
                try
                {
                        // Get the SFML window from the state machine as a reference
                        sf::RenderWindow &window = getStateMachine()->getWindow();

                        // Clear the window with a dark blue color
                        window.clear(sf::Color(25, 25, 112));

                        // Render the map if loaded
                        if (m_tiledMap && m_tiledMap->isLoaded())
                        {
                                // Create a view for the map rendering
                                sf::View mapView = window.getView();

                                // Find the player entity to center the view on it
                                auto playerEntities = m_entityManager->getEntitiesByName("Player");
                                if (!playerEntities.empty())
                                {
                                        auto player = playerEntities[0];
                                        auto transform = player->getComponent<TransformComponent>();

                                        if (transform)
                                        {
                                                // Center the view on the player
                                                sf::Vector2f position = transform->getPosition();
                                                mapView.setCenter(position);
                                        }
                                }

                                // Apply the view
                                window.setView(mapView);

                                // Render the map layers
                                for (const auto &layer : m_tiledMap->getTileLayers())
                                {
                                        if (layer.visible)
                                        {
                                                // Rendering code for tile layers would normally go here
                                                // Since we don't have access to TiledMapRenderer, we'll
                                                // add a placeholder comment

                                                // TiledMapRenderer::renderLayer(window, *m_tiledMap, layer);
                                        }
                                }

                                // Render image layers
                                for (const auto &layer : m_tiledMap->getImageLayers())
                                {
                                        if (layer.visible)
                                        {
                                                // Rendering code for image layers would normally go here

                                                // TiledMapRenderer::renderImageLayer(window, layer);
                                        }
                                }
                        }

                        // Render all entities with the render system
                        m_renderSystem->render(window);

                        // Display the rendered content
                        window.display();
                }
                catch (const std::exception &e)
                {
                        std::cerr << "Error in GameState::render: " << e.what() << std::endl;
                        return false;
                }

                return true;
        }

        bool GameState::handleInput()
        {
                try
                {
                        // Get the SFML window from the state machine as a reference
                        sf::RenderWindow &window = getStateMachine()->getWindow();

                        sf::Event event;
                        while (window.pollEvent(event))
                        {
                                if (event.type == sf::Event::Closed)
                                {
                                        window.close();
                                        return false;
                                }

                                if (event.type == sf::Event::KeyPressed)
                                {
                                        if (event.key.code == sf::Keyboard::Escape)
                                        {
                                                m_isPaused = !m_isPaused;
                                                std::cout << "Game " << (m_isPaused ? "Paused" : "Resumed") << std::endl;
                                        }
                                }
                        }

                        // Continuous movement based on key states (for smooth movement)
                        if (!m_isPaused)
                        {
                                auto playerEntities = m_entityManager->getEntitiesByName("Player");
                                if (!playerEntities.empty())
                                {
                                        auto player = playerEntities[0];
                                        auto transform = player->getComponent<TransformComponent>();

                                        if (transform)
                                        {
                                                float moveSpeed = 5.0f;
                                                sf::Vector2f moveDirection(0.0f, 0.0f);

                                                // Get key states for continuous movement
                                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                                                {
                                                        moveDirection.y -= 1.0f;
                                                }
                                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                                                {
                                                        moveDirection.y += 1.0f;
                                                }
                                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                                                {
                                                        moveDirection.x -= 1.0f;
                                                }
                                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                                                {
                                                        moveDirection.x += 1.0f;
                                                }

                                                // Normalize the direction for diagonal movement
                                                if (moveDirection.x != 0.0f && moveDirection.y != 0.0f)
                                                {
                                                        float length = std::sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
                                                        moveDirection.x /= length;
                                                        moveDirection.y /= length;
                                                }

                                                // Apply movement
                                                if (moveDirection.x != 0.0f || moveDirection.y != 0.0f)
                                                {
                                                        transform->move(moveDirection.x * moveSpeed, moveDirection.y * moveSpeed);
                                                }
                                        }
                                }
                        }
                }
                catch (const std::exception &e)
                {
                        std::cerr << "Error in GameState::handleInput: " << e.what() << std::endl;
                        return false;
                }

                return true;
        }

} // namespace Orenji