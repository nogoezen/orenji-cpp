#pragma once

#include "../Core/Scene.hpp"
#include "../Resources/ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

// Forward declaration
namespace Core
{
    class Engine;
}

namespace Scenes
{
    // Forward declarations
    class GameScene;

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
        MainMenuScene(Core::Engine &engine);

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
        Core::Engine &m_engine;

        // Menu elements
        std::unique_ptr<sf::Text> m_titleText;
        std::vector<sf::Text> m_menuOptions;
        int m_selectedOption;

        // Background
        std::unique_ptr<sf::Sprite> m_backgroundSprite;
        sf::Texture *m_backgroundTexture{nullptr};

        // Menu transitions
        float m_transitionAlpha;
        bool m_isTransitioning;

        // Demo menu
        std::unique_ptr<sf::Text> m_demosTitle;
        std::unique_ptr<sf::Text> m_backText;
        std::unique_ptr<sf::Text> m_comingSoonText;
        std::vector<sf::Text> m_demoItems;
        std::vector<std::string> m_demoDescriptions;
        std::vector<std::string> m_demoCommands;
        bool m_showDemosList;
        size_t m_selectedDemo;

        // Sprite
        std::unique_ptr<sf::Sprite> m_background;

        // Menu items
        std::vector<sf::Text> m_menuItems;
        size_t m_selectedItem;

        // Music
        sf::Music *m_music;
        sf::Sound *m_selectionSound;

        // Resources
        Resources::ResourceManager m_resourceManager;

        /**
         * @brief Create a menu item
         * @param text Text of the menu item
         * @param yPos Y position of the menu item
         */
        void createMenuItem(const std::string &text, float yPos);

        /**
         * @brief Update the selected menu item
         */
        void updateMenuSelection();

        /**
         * @brief Update the selected demo
         */
        void updateDemoSelection();

        /**
         * @brief Center text
         * @param text Text to center
         * @param position Position to center around
         */
        void centerText(sf::Text &text, const sf::Vector2f &position);

        /**
         * @brief Load the list of available demos
         */
        void loadDemosList();

        /**
         * @brief Show the demos list
         */
        void showDemosList();
    };

} // namespace Scenes