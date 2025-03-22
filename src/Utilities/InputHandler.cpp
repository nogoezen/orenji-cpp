#include "../../include/Utilities/InputHandler.hpp"

namespace Orenji
{

    InputHandler &InputHandler::getInstance()
    {
        static InputHandler instance;
        return instance;
    }

    void InputHandler::processEvent(const sf::Event &event)
    {
        // Process keyboard events
        if (event.type == sf::Event::KeyPressed)
        {
            for (const auto &[action, key] : m_keyBindings)
            {
                if (key == event.key.code && !m_actionStates[action].isPressed)
                {
                    m_actionStates[action].isPressed = true;
                    m_actionStates[action].heldTime = 0.0f;

                    auto it = m_keyPressedCallbacks.find(action);
                    if (it != m_keyPressedCallbacks.end())
                    {
                        it->second();
                    }
                }
            }
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            for (const auto &[action, key] : m_keyBindings)
            {
                if (key == event.key.code && m_actionStates[action].isPressed)
                {
                    m_actionStates[action].isPressed = false;

                    auto it = m_keyReleasedCallbacks.find(action);
                    if (it != m_keyReleasedCallbacks.end())
                    {
                        it->second();
                    }
                }
            }
        }

        // Process mouse events
        if (event.type == sf::Event::MouseButtonPressed)
        {
            for (const auto &[action, button] : m_mouseBindings)
            {
                if (button == event.mouseButton.button && !m_actionStates[action].isPressed)
                {
                    m_actionStates[action].isPressed = true;

                    auto it = m_mousePressedCallbacks.find(action);
                    if (it != m_mousePressedCallbacks.end())
                    {
                        it->second(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            for (const auto &[action, button] : m_mouseBindings)
            {
                if (button == event.mouseButton.button && m_actionStates[action].isPressed)
                {
                    m_actionStates[action].isPressed = false;

                    auto it = m_mouseReleasedCallbacks.find(action);
                    if (it != m_mouseReleasedCallbacks.end())
                    {
                        it->second(event.mouseButton.x, event.mouseButton.y);
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            if (m_mouseMovedCallback)
            {
                m_mouseMovedCallback(event.mouseMove.x, event.mouseMove.y);
            }
        }
    }

    void InputHandler::update(float deltaTime)
    {
        // Update held times for pressed keys
        for (auto &[action, state] : m_actionStates)
        {
            if (state.isPressed)
            {
                state.heldTime += deltaTime;

                // Trigger key held callbacks
                auto it = m_keyHeldCallbacks.find(action);
                if (it != m_keyHeldCallbacks.end())
                {
                    it->second(state.heldTime);
                }
            }
        }
    }

    void InputHandler::bindKey(const std::string &action, sf::Keyboard::Key key)
    {
        m_keyBindings[action] = key;
        m_actionStates[action] = ActionState();
    }

    void InputHandler::bindMouseButton(const std::string &action, sf::Mouse::Button button)
    {
        m_mouseBindings[action] = button;
        m_actionStates[action] = ActionState();
    }

    void InputHandler::registerKeyPressedCallback(const std::string &action, const std::function<void()> &callback)
    {
        m_keyPressedCallbacks[action] = callback;
    }

    void InputHandler::registerKeyReleasedCallback(const std::string &action, const std::function<void()> &callback)
    {
        m_keyReleasedCallbacks[action] = callback;
    }

    void InputHandler::registerKeyHeldCallback(const std::string &action, const std::function<void(float)> &callback)
    {
        m_keyHeldCallbacks[action] = callback;
    }

    void InputHandler::registerMousePressedCallback(const std::string &action, const std::function<void(int, int)> &callback)
    {
        m_mousePressedCallbacks[action] = callback;
    }

    void InputHandler::registerMouseReleasedCallback(const std::string &action, const std::function<void(int, int)> &callback)
    {
        m_mouseReleasedCallbacks[action] = callback;
    }

    void InputHandler::registerMouseMovedCallback(const std::function<void(int, int)> &callback)
    {
        m_mouseMovedCallback = callback;
    }

    void InputHandler::clear()
    {
        m_keyBindings.clear();
        m_mouseBindings.clear();
        m_actionStates.clear();
        m_keyPressedCallbacks.clear();
        m_keyReleasedCallbacks.clear();
        m_keyHeldCallbacks.clear();
        m_mousePressedCallbacks.clear();
        m_mouseReleasedCallbacks.clear();
        m_mouseMovedCallback = nullptr;
    }

    bool InputHandler::isKeyPressed(const std::string &action) const
    {
        auto it = m_actionStates.find(action);
        if (it != m_actionStates.end())
        {
            return it->second.isPressed;
        }
        return false;
    }

    bool InputHandler::isMouseButtonPressed(const std::string &action) const
    {
        auto it = m_actionStates.find(action);
        if (it != m_actionStates.end())
        {
            return it->second.isPressed;
        }
        return false;
    }

} // namespace Orenji