#include "GuiUI.hpp"
#include <iostream>

GuiUI::GuiUI()
    : m_window(nullptr), m_currentState(UIState::NONE), m_selectedIndex(0)
{
}

GuiUI::~GuiUI()
{
}

void GuiUI::initialize(sf::RenderWindow &window)
{
    m_window = &window;

    // Chargement de la police par défaut
    if (!m_font.loadFromFile("resources/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Erreur: Impossible de charger la police" << std::endl;
    }

    m_text.setFont(m_font);
    m_text.setCharacterSize(20);
    m_text.setFillColor(sf::Color::White);
}

void GuiUI::setFont(const std::string &fontPath)
{
    if (!m_font.loadFromFile(fontPath))
    {
        std::cerr << "Erreur: Impossible de charger la police " << fontPath << std::endl;
    }
    else
    {
        m_text.setFont(m_font);
    }
}

void GuiUI::clearScreen()
{
    m_currentState = UIState::NONE;
    m_currentTitle.clear();
    m_currentMenuItems.clear();
    m_currentAttributes.clear();
    m_currentList.clear();
}

void GuiUI::displayTitle(const std::string &title)
{
    m_currentTitle = title;
    m_currentState = UIState::TITLE;
}

void GuiUI::drawLine(int width, char symbol)
{
    // Cette méthode n'a pas vraiment d'équivalent graphique direct
    // On pourrait dessiner une ligne horizontale dans le rendu
}

void GuiUI::waitForEnter(const std::string &message)
{
    m_waitMessage = message;
    m_currentState = UIState::WAITING;
}

int GuiUI::displayMenu(const std::string &title, const std::vector<MenuItem> &items)
{
    m_currentTitle = title;
    m_currentMenuItems = items;
    m_currentState = UIState::MENU;
    m_selectedIndex = 0;

    // Dans une implémentation réelle, on attendrait une entrée utilisateur ici
    // et on retournerait l'indice sélectionné
    return 0;
}

void GuiUI::displayError(const std::string &message)
{
    m_waitMessage = message;
    m_currentState = UIState::ERROR;
}

void GuiUI::displayNumberedList(const std::vector<std::string> &items, const std::string &title)
{
    m_currentTitle = title;
    m_currentList = items;
    m_currentState = UIState::LIST;
}

void GuiUI::displayAttributes(const std::map<std::string, std::string> &attributes, const std::string &title)
{
    m_currentTitle = title;
    m_currentAttributes = attributes;
    m_currentState = UIState::ATTRIBUTES;
}

void GuiUI::render(sf::RenderWindow &window)
{
    if (!m_window)
    {
        m_window = &window;
    }

    switch (m_currentState)
    {
    case UIState::TITLE:
        drawCenteredText(m_currentTitle, 50.f, 28, sf::Color::Yellow);
        break;

    case UIState::MENU:
        drawCenteredText(m_currentTitle, 30.f, 28, sf::Color::Yellow);
        for (size_t i = 0; i < m_currentMenuItems.size(); ++i)
        {
            drawMenuItem(m_currentMenuItems[i], i, i == m_selectedIndex);
        }
        break;

    case UIState::ERROR:
        drawCenteredText("Erreur", 30.f, 28, sf::Color::Red);
        drawCenteredText(m_waitMessage, 80.f, 20, sf::Color::White);
        drawCenteredText("Appuyez sur Entrée pour continuer...", window.getSize().y - 50.f, 18, sf::Color(200, 200, 200));
        break;

    case UIState::WAITING:
        drawCenteredText(m_waitMessage, window.getSize().y / 2.f, 22, sf::Color::White);
        drawCenteredText("Appuyez sur Entrée pour continuer...", window.getSize().y - 50.f, 18, sf::Color(200, 200, 200));
        break;

    case UIState::LIST:
        drawCenteredText(m_currentTitle, 30.f, 28, sf::Color::Yellow);
        for (size_t i = 0; i < m_currentList.size(); ++i)
        {
            m_text.setString(std::to_string(i + 1) + ". " + m_currentList[i]);
            m_text.setPosition(50.f, 80.f + i * 30.f);
            window.draw(m_text);
        }
        break;

    case UIState::ATTRIBUTES:
        drawCenteredText(m_currentTitle, 30.f, 28, sf::Color::Yellow);
        {
            float y = 80.f;
            for (const auto &attr : m_currentAttributes)
            {
                drawAttribute(attr.first, attr.second, y);
                y += 30.f;
            }
        }
        break;

    case UIState::NONE:
    default:
        break;
    }
}

void GuiUI::handleEvent(sf::Event &event)
{
    if (m_currentState == UIState::MENU && event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Up)
        {
            if (m_selectedIndex > 0)
            {
                m_selectedIndex--;
            }
            else
            {
                m_selectedIndex = m_currentMenuItems.size() - 1;
            }
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            if (m_selectedIndex < m_currentMenuItems.size() - 1)
            {
                m_selectedIndex++;
            }
            else
            {
                m_selectedIndex = 0;
            }
        }
        else if (event.key.code == sf::Keyboard::Return)
        {
            // Retourner l'élément sélectionné
            // Dans une vraie implémentation, on utiliserait un callback
        }
    }
    else if ((m_currentState == UIState::WAITING || m_currentState == UIState::ERROR) &&
             event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
    {
        clearScreen();
    }
}

void GuiUI::update(float deltaTime)
{
    // Animations ou mises à jour de l'interface
}

void GuiUI::drawCenteredText(const std::string &text, float y, unsigned int size, sf::Color color)
{
    m_text.setString(text);
    m_text.setCharacterSize(size);
    m_text.setFillColor(color);

    sf::FloatRect bounds = m_text.getLocalBounds();
    m_text.setPosition((m_window->getSize().x - bounds.width) / 2.f, y);

    m_window->draw(m_text);
}

void GuiUI::drawMenuItem(const MenuItem &item, int index, bool selected)
{
    sf::Color color = item.enabled ? (selected ? sf::Color::Yellow : sf::Color::White) : sf::Color(128, 128, 128);

    m_text.setString(item.text);
    m_text.setCharacterSize(20);
    m_text.setFillColor(color);

    float y = 80.f + index * 30.f;
    m_text.setPosition(50.f, y);

    if (selected)
    {
        m_text.setString("> " + item.text);
    }

    m_window->draw(m_text);

    if (!item.description.empty())
    {
        m_text.setString(item.description);
        m_text.setCharacterSize(16);
        m_text.setFillColor(sf::Color(180, 180, 180));
        m_text.setPosition(m_window->getSize().x - 20.f - m_text.getLocalBounds().width, y);
        m_window->draw(m_text);
    }
}

void GuiUI::drawAttribute(const std::string &key, const std::string &value, float y)
{
    m_text.setCharacterSize(18);

    m_text.setString(key + ": ");
    m_text.setFillColor(sf::Color(200, 200, 200));
    m_text.setPosition(50.f, y);
    m_window->draw(m_text);

    m_text.setString(value);
    m_text.setFillColor(sf::Color::White);
    m_text.setPosition(50.f + 150.f, y);
    m_window->draw(m_text);
}