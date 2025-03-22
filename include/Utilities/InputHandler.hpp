#ifndef ORENJI_INPUT_HANDLER_HPP
#define ORENJI_INPUT_HANDLER_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <functional>
#include <map>
#include <string>

namespace Orenji
{

    /**
     * @class InputHandler
     * @brief Handles user input and provides callback functionality
     */
    class InputHandler
    {
    public:
        /**
         * @brief Get the singleton instance
         * @return Reference to the singleton instance
         */
        static InputHandler &getInstance();

        /**
         * @brief Process events from SFML
         * @param event SFML event to process
         */
        void processEvent(const sf::Event &event);

        /**
         * @brief Update the input handler state
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime);

        /**
         * @brief Bind a key to an action
         * @param action Action identifier
         * @param key SFML key to bind
         */
        void bindKey(const std::string &action, sf::Keyboard::Key key);

        /**
         * @brief Bind a mouse button to an action
         * @param action Action identifier
         * @param button SFML mouse button to bind
         */
        void bindMouseButton(const std::string &action, sf::Mouse::Button button);

        /**
         * @brief Register a callback for a key pressed event
         * @param action Action identifier
         * @param callback Function to call when the key is pressed
         */
        void registerKeyPressedCallback(const std::string &action, const std::function<void()> &callback);

        /**
         * @brief Register a callback for a key released event
         * @param action Action identifier
         * @param callback Function to call when the key is released
         */
        void registerKeyReleasedCallback(const std::string &action, const std::function<void()> &callback);

        /**
         * @brief Register a callback for a key held event
         * @param action Action identifier
         * @param callback Function to call when the key is held
         */
        void registerKeyHeldCallback(const std::string &action, const std::function<void(float)> &callback);

        /**
         * @brief Register a callback for a mouse button pressed event
         * @param action Action identifier
         * @param callback Function to call when the mouse button is pressed
         */
        void registerMousePressedCallback(const std::string &action, const std::function<void(int, int)> &callback);

        /**
         * @brief Register a callback for a mouse button released event
         * @param action Action identifier
         * @param callback Function to call when the mouse button is released
         */
        void registerMouseReleasedCallback(const std::string &action, const std::function<void(int, int)> &callback);

        /**
         * @brief Register a callback for a mouse moved event
         * @param callback Function to call when the mouse is moved
         */
        void registerMouseMovedCallback(const std::function<void(int, int)> &callback);

        /**
         * @brief Clear all bindings and callbacks
         */
        void clear();

        /**
         * @brief Check if a key is currently pressed
         * @param action Action identifier
         * @return True if the key bound to the action is pressed
         */
        bool isKeyPressed(const std::string &action) const;

        /**
         * @brief Check if a mouse button is currently pressed
         * @param action Action identifier
         * @return True if the mouse button bound to the action is pressed
         */
        bool isMouseButtonPressed(const std::string &action) const;

    private:
        /**
         * @brief Constructor
         */
        InputHandler() = default;

        /**
         * @brief Destructor
         */
        ~InputHandler() = default;

        /**
         * @brief Copy constructor (deleted)
         */
        InputHandler(const InputHandler &) = delete;

        /**
         * @brief Assignment operator (deleted)
         */
        InputHandler &operator=(const InputHandler &) = delete;

        struct ActionState
        {
            bool isPressed = false;
            float heldTime = 0.0f;
        };

        std::map<std::string, sf::Keyboard::Key> m_keyBindings;
        std::map<std::string, sf::Mouse::Button> m_mouseBindings;
        std::map<std::string, ActionState> m_actionStates;

        std::map<std::string, std::function<void()>> m_keyPressedCallbacks;
        std::map<std::string, std::function<void()>> m_keyReleasedCallbacks;
        std::map<std::string, std::function<void(float)>> m_keyHeldCallbacks;
        std::map<std::string, std::function<void(int, int)>> m_mousePressedCallbacks;
        std::map<std::string, std::function<void(int, int)>> m_mouseReleasedCallbacks;
        std::function<void(int, int)> m_mouseMovedCallback;
    };

} // namespace Orenji

#endif // ORENJI_INPUT_HANDLER_HPP