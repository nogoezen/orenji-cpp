#pragma once
#include <SFML/Window.hpp>
#include <memory>
#include <map>
#include <functional>

namespace Orenji {

class InputHandler {
public:
    enum class InputType {
        Pressed,
        Released,
        Held
    };

    InputHandler();
    ~InputHandler();

    // Singleton pattern
    static InputHandler& getInstance();

    // Event handling
    void processEvent(const sf::Event& event);
    void update();

    // Keyboard input
    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isKeyReleased(sf::Keyboard::Key key) const;
    bool isKeyHeld(sf::Keyboard::Key key) const;

    // Mouse input
    bool isMouseButtonPressed(sf::Mouse::Button button) const;
    bool isMouseButtonReleased(sf::Mouse::Button button) const;
    bool isMouseButtonHeld(sf::Mouse::Button button) const;
    
    sf::Vector2i getMousePosition() const;
    sf::Vector2f getMouseWorldPosition(const sf::View& view) const;

    // Input mapping
    void bindKeyAction(sf::Keyboard::Key key, const std::string& actionName);
    void bindMouseAction(sf::Mouse::Button button, const std::string& actionName);
    
    bool isActionPressed(const std::string& actionName) const;
    bool isActionReleased(const std::string& actionName) const;
    bool isActionHeld(const std::string& actionName) const;

private:
    std::map<sf::Keyboard::Key, bool> m_keyCurrentState;
    std::map<sf::Keyboard::Key, bool> m_keyPreviousState;
    
    std::map<sf::Mouse::Button, bool> m_mouseCurrentState;
    std::map<sf::Mouse::Button, bool> m_mousePreviousState;
    
    std::map<std::string, sf::Keyboard::Key> m_keyBindings;
    std::map<std::string, sf::Mouse::Button> m_mouseBindings;
    
    sf::Vector2i m_mousePosition;
};

} // namespace Orenji 