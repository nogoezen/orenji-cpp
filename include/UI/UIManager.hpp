#pragma once

#include <SFML/Graphics.hpp>

namespace UI
{

    class UIManager
    {
    public:
        UIManager(sf::RenderWindow &window);
        ~UIManager();

        /**
         * @brief Initialize the UI system
         */
        void init();

        /**
         * @brief Update the UI system
         * @param deltaTime Time since last frame in seconds
         */
        void update(float deltaTime);

        /**
         * @brief Render the UI
         */
        void render();

        /**
         * @brief Handle an event
         * @param event SFML event
         * @return true if the event was handled by the UI
         */
        bool handleEvent(const sf::Event &event);

    private:
        sf::RenderWindow &m_window;
    };

} // namespace UI