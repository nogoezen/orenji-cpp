#pragma once

#include "../Core/Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

// Forward declaration
class Engine;

namespace Scenes
{

    /**
     * @brief Main menu scene class
     */
    class MainMenuScene : public Core::Scene
    {
    public:
        /**
         * @brief Constructor
         * @param engine Reference to the engine
         */
        MainMenuScene(Engine &engine);

        /**
         * @brief Destructor
         */
        virtual ~MainMenuScene();

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
        Engine &m_engine;

        // Menu elements
        sf::Text m_titleText;
        std::vector<sf::Text> m_menuOptions;
        int m_selectedOption;

        // Background
        sf::Sprite m_backgroundSprite;

        // Menu transitions
        float m_transitionAlpha;
        bool m_isTransitioning;

        /**
         * @brief Create the menu options
         */
        void createMenuOptions();

        /**
         * @brief Handle menu selection
         */
        void handleMenuSelection();
    };

} // namespace Scenes