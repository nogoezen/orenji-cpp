#pragma once

#include "../Core/Scene.hpp"
#include <SFML/Graphics.hpp>

// Forward declaration
namespace Core
{
    class Engine;
}

namespace Scenes
{

    /**
     * @brief Main game scene class
     */
    class GameScene : public Core::Scene
    {
    public:
        /**
         * @brief Constructor
         * @param engine Reference to the engine
         */
        GameScene(Core::Engine &engine);

        /**
         * @brief Destructor
         */
        virtual ~GameScene();

        /**
         * @brief Initialize the scene
         */
        virtual void init() override;

        /**
         * @brief Update the scene
         * @param deltaTime Time since last frame in seconds
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Render the scene
         * @param window Reference to the render window
         */
        virtual void render(sf::RenderWindow &window) override;

        /**
         * @brief Handle an event
         * @param event SFML event
         */
        virtual void handleEvent(const sf::Event &event) override;

    private:
        Core::Engine &m_engine;

        // Game elements
        // TODO: Add game-specific variables
    };

} // namespace Scenes