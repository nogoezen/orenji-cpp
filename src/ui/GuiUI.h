#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

class GuiUI
{
public:
    struct MenuItem
    {
        std::string text;
        std::string description;
        bool enabled = true;
    };

    GuiUI();
    ~GuiUI();

    // Initialisation et configuration
    void initialize(sf::RenderWindow &window);
    void setFont(const std::string &fontPath);

    // Méthodes UI de base
    void clearScreen();
    void displayTitle(const std::string &title);
    void drawLine(int width, char symbol);
    void waitForEnter(const std::string &message);
    int displayMenu(const std::string &title, const std::vector<MenuItem> &items);
    void displayError(const std::string &message);
    void displayNumberedList(const std::vector<std::string> &items, const std::string &title);
    void displayAttributes(const std::map<std::string, std::string> &attributes, const std::string &title);

    // Méthodes d'entrée utilisateur
    template <typename T>
    T getUserInput(const std::string &prompt, T defaultValue);

    // Rendu SFML
    void render(sf::RenderWindow &window);
    void handleEvent(sf::Event &event);
    void update(float deltaTime);

private:
    sf::RenderWindow *m_window;
    sf::Font m_font;
    sf::Text m_text;

    // États UI
    std::string m_currentTitle;
    std::vector<MenuItem> m_currentMenuItems;
    std::map<std::string, std::string> m_currentAttributes;
    std::vector<std::string> m_currentList;

    // État actuel de l'interface
    enum class UIState
    {
        NONE,
        TITLE,
        MENU,
        ERROR,
        LIST,
        ATTRIBUTES,
        WAITING
    };

    UIState m_currentState;
    int m_selectedIndex;
    std::string m_waitMessage;

    // Helpers
    void drawCenteredText(const std::string &text, float y, unsigned int size, sf::Color color);
    void drawMenuItem(const MenuItem &item, int index, bool selected);
    void drawAttribute(const std::string &key, const std::string &value, float y);
};

// Implémentation template
template <typename T>
T GuiUI::getUserInput(const std::string &prompt, T defaultValue)
{
    // Pour l'instant, utiliser une implémentation simple
    // Une vraie implémentation utiliserait un dialogue SFML
    return defaultValue;
}