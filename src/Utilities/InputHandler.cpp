#include "../../include/Utilities/InputHandler.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

namespace Orenji
{

    InputHandler::InputHandler()
    {
        // Initialize all keys to not pressed
        for (int key = 0; key < sf::Keyboard::KeyCount; ++key)
        {
            m_keyCurrentState[static_cast<sf::Keyboard::Key>(key)] = false;
            m_keyPreviousState[static_cast<sf::Keyboard::Key>(key)] = false;
        }

        // Initialize all mouse buttons to not pressed
        for (int button = 0; button < sf::Mouse::ButtonCount; ++button)
        {
            m_mouseCurrentState[static_cast<sf::Mouse::Button>(button)] = false;
            m_mousePreviousState[static_cast<sf::Mouse::Button>(button)] = false;
        }
    }

    InputHandler::~InputHandler()
    {
    }

    InputHandler &InputHandler::getInstance()
    {
        static InputHandler instance;
        return instance;
    }

    void InputHandler::processEvent(const sf::Event &event)
    {
        // Update keyboard states
        if (event.type == sf::Event::KeyPressed)
        {
            m_keyCurrentState[event.key.code] = true;
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            m_keyCurrentState[event.key.code] = false;
        }

        // Update mouse states
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            m_mouseCurrentState[event.mouseButton.button] = true;
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            m_mouseCurrentState[event.mouseButton.button] = false;
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            m_mousePosition.x = event.mouseMove.x;
            m_mousePosition.y = event.mouseMove.y;
        }
    }

    void InputHandler::update()
    {
        // Update previous state for keys
        for (int key = 0; key < sf::Keyboard::KeyCount; ++key)
        {
            m_keyPreviousState[static_cast<sf::Keyboard::Key>(key)] =
                m_keyCurrentState[static_cast<sf::Keyboard::Key>(key)];
        }

        // Update previous state for mouse buttons
        for (int button = 0; button < sf::Mouse::ButtonCount; ++button)
        {
            m_mousePreviousState[static_cast<sf::Mouse::Button>(button)] =
                m_mouseCurrentState[static_cast<sf::Mouse::Button>(button)];
        }
    }

    bool InputHandler::isKeyPressed(sf::Keyboard::Key key) const
    {
        return m_keyCurrentState.at(key) && !m_keyPreviousState.at(key);
    }

    bool InputHandler::isKeyReleased(sf::Keyboard::Key key) const
    {
        return !m_keyCurrentState.at(key) && m_keyPreviousState.at(key);
    }

    bool InputHandler::isKeyHeld(sf::Keyboard::Key key) const
    {
        return m_keyCurrentState.at(key);
    }

    bool InputHandler::isMouseButtonPressed(sf::Mouse::Button button) const
    {
        return m_mouseCurrentState.at(button) && !m_mousePreviousState.at(button);
    }

    bool InputHandler::isMouseButtonReleased(sf::Mouse::Button button) const
    {
        return !m_mouseCurrentState.at(button) && m_mousePreviousState.at(button);
    }

    bool InputHandler::isMouseButtonHeld(sf::Mouse::Button button) const
    {
        return m_mouseCurrentState.at(button);
    }

    sf::Vector2i InputHandler::getMousePosition() const
    {
        return m_mousePosition;
    }

    sf::Vector2f InputHandler::getMouseWorldPosition(const sf::View &view) const
    {
        // Convert screen coordinates to world coordinates
        // This would require access to the window, so this is just a placeholder
        // In real implementation, you'd need to get this from the window or store the window
        sf::RenderWindow *window = nullptr; // TODO: Get from game singleton

        if (window)
        {
            return window->mapPixelToCoords(m_mousePosition, view);
        }

        return sf::Vector2f(static_cast<float>(m_mousePosition.x),
                            static_cast<float>(m_mousePosition.y));
    }

    void InputHandler::bindKeyAction(sf::Keyboard::Key key, const std::string &actionName)
    {
        m_keyBindings[actionName] = key;
    }

    void InputHandler::bindMouseAction(sf::Mouse::Button button, const std::string &actionName)
    {
        m_mouseBindings[actionName] = button;
    }

    bool InputHandler::isActionPressed(const std::string &actionName) const
    {
        auto keyIt = m_keyBindings.find(actionName);
        if (keyIt != m_keyBindings.end())
        {
            return isKeyPressed(keyIt->second);
        }

        auto mouseIt = m_mouseBindings.find(actionName);
        if (mouseIt != m_mouseBindings.end())
        {
            return isMouseButtonPressed(mouseIt->second);
        }

        return false;
    }

    bool InputHandler::isActionReleased(const std::string &actionName) const
    {
        auto keyIt = m_keyBindings.find(actionName);
        if (keyIt != m_keyBindings.end())
        {
            return isKeyReleased(keyIt->second);
        }

        auto mouseIt = m_mouseBindings.find(actionName);
        if (mouseIt != m_mouseBindings.end())
        {
            return isMouseButtonReleased(mouseIt->second);
        }

        return false;
    }

    bool InputHandler::isActionHeld(const std::string &actionName) const
    {
        auto keyIt = m_keyBindings.find(actionName);
        if (keyIt != m_keyBindings.end())
        {
            return isKeyHeld(keyIt->second);
        }

        auto mouseIt = m_mouseBindings.find(actionName);
        if (mouseIt != m_mouseBindings.end())
        {
            return isMouseButtonHeld(mouseIt->second);
        }

        return false;
    }

} // namespace Orenji