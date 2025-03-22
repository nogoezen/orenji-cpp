#include "State/MenuState.hpp"
#include <iostream>

namespace Orenji
{
    MenuState::MenuState(const std::string &title)
        : m_title(title),
          m_selectedIndex(0),
          m_backgroundColor(sf::Color(20, 20, 40, 220)),
          m_textColor(sf::Color::White),
          m_selectedTextColor(sf::Color::Yellow),
          m_textSize(24),
          m_titleTextSize(36),
          m_itemSpacing(50.0f),
          m_needLayoutUpdate(true)
    {
    }

    bool MenuState::initialize()
    {
        // Charge la police par défaut (ou une police personnalisée)
        if (!m_font.loadFromFile("resources/fonts/arial.ttf"))
        {
            std::cerr << "Erreur lors du chargement de la police\n";
            // Créer une police par défaut pour les exemples si le chargement échoue
            std::vector<uint8_t> fontData(1000, 0);
            if (!m_font.loadFromMemory(fontData.data(), fontData.size()))
            {
                return false;
            }
        }

        // Configure le texte du titre
        m_titleText.setFont(m_font);
        m_titleText.setString(m_title);
        m_titleText.setCharacterSize(m_titleTextSize);
        m_titleText.setFillColor(m_textColor);
        m_titleText.setStyle(sf::Text::Bold);

        // Configure le fond
        m_background.setFillColor(m_backgroundColor);

        return true;
    }

    void MenuState::update(float deltaTime)
    {
        // Met à jour la disposition si nécessaire
        if (m_needLayoutUpdate)
        {
            updateLayout();
            m_needLayoutUpdate = false;
        }
    }

    void MenuState::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::Type::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::Up)
            {
                selectPrevious();
            }
            else if (event.key.code == sf::Keyboard::Key::Down)
            {
                selectNext();
            }
            else if (event.key.code == sf::Keyboard::Key::Return ||
                     event.key.code == sf::Keyboard::Key::Space)
            {
                activateSelected();
            }
            else if (event.key.code == sf::Keyboard::Key::Escape)
            {
                m_shouldRemove = true; // Quitte le menu
            }
        }
    }

    void MenuState::render(sf::RenderWindow &window)
    {
        // Dessine le fond
        window.draw(m_background);

        // Dessine le titre
        window.draw(m_titleText);

        // Dessine les options de menu
        for (const auto &item : m_menuItems)
        {
            window.draw(item.textObject);
        }
    }

    void MenuState::onActive()
    {
        m_needLayoutUpdate = true;
    }

    void MenuState::onInactive()
    {
    }

    void MenuState::addMenuItem(const std::string &text, std::function<void()> callback)
    {
        MenuItem item;
        item.text = text;
        item.callback = callback;

        // Configure le texte SFML
        item.textObject.setFont(m_font);
        item.textObject.setString(text);
        item.textObject.setCharacterSize(m_textSize);
        item.textObject.setFillColor(m_menuItems.size() == m_selectedIndex ? m_selectedTextColor : m_textColor);

        m_menuItems.push_back(item);
        m_needLayoutUpdate = true;
    }

    void MenuState::setTitle(const std::string &title)
    {
        m_title = title;
        m_titleText.setString(title);
        m_needLayoutUpdate = true;
    }

    void MenuState::setBackgroundColor(const sf::Color &color)
    {
        m_backgroundColor = color;
        m_background.setFillColor(color);
    }

    void MenuState::setTextColor(const sf::Color &color)
    {
        m_textColor = color;

        // Met à jour la couleur du texte des options non sélectionnées
        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            if (i != m_selectedIndex)
            {
                m_menuItems[i].textObject.setFillColor(color);
            }
        }

        // Met à jour la couleur du titre
        m_titleText.setFillColor(color);
    }

    void MenuState::setSelectedTextColor(const sf::Color &color)
    {
        m_selectedTextColor = color;

        // Met à jour la couleur du texte de l'option sélectionnée
        if (!m_menuItems.empty() && m_selectedIndex < m_menuItems.size())
        {
            m_menuItems[m_selectedIndex].textObject.setFillColor(color);
        }
    }

    void MenuState::setTextSize(unsigned int size)
    {
        m_textSize = size;

        // Met à jour la taille du texte des options
        for (auto &item : m_menuItems)
        {
            item.textObject.setCharacterSize(size);
        }

        m_needLayoutUpdate = true;
    }

    void MenuState::setTitleTextSize(unsigned int size)
    {
        m_titleTextSize = size;
        m_titleText.setCharacterSize(size);
        m_needLayoutUpdate = true;
    }

    void MenuState::updateLayout()
    {
        // Adapte la taille du fond à la fenêtre
        sf::Vector2f windowSize(800.0f, 600.0f); // Taille par défaut
        if (auto window = dynamic_cast<sf::RenderWindow *>(nullptr))
        {
            windowSize = sf::Vector2f(window->getSize());
        }

        m_background.setSize(windowSize);
        m_background.setPosition(0, 0);

        // Centre le titre
        sf::FloatRect titleBounds = m_titleText.getLocalBounds();
        m_titleText.setOrigin(titleBounds.width / 2.0f, 0);
        m_titleText.setPosition(windowSize.x / 2.0f, 50.0f);

        // Positionne les options de menu
        float yPos = 150.0f + m_titleTextSize; // Position initiale après le titre

        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            auto &item = m_menuItems[i];
            sf::FloatRect itemBounds = item.textObject.getLocalBounds();

            item.textObject.setOrigin(itemBounds.width / 2.0f, 0);
            item.textObject.setPosition(windowSize.x / 2.0f, yPos);

            // Couleur différente pour l'option sélectionnée
            item.textObject.setFillColor(i == m_selectedIndex ? m_selectedTextColor : m_textColor);

            yPos += m_itemSpacing;
        }
    }

    void MenuState::selectPrevious()
    {
        if (m_menuItems.empty())
        {
            return;
        }

        // Change la couleur de l'élément actuellement sélectionné
        m_menuItems[m_selectedIndex].textObject.setFillColor(m_textColor);

        // Sélectionne l'élément précédent
        if (m_selectedIndex > 0)
        {
            m_selectedIndex--;
        }
        else
        {
            m_selectedIndex = m_menuItems.size() - 1; // Boucle vers le dernier élément
        }

        // Change la couleur du nouvel élément sélectionné
        m_menuItems[m_selectedIndex].textObject.setFillColor(m_selectedTextColor);
    }

    void MenuState::selectNext()
    {
        if (m_menuItems.empty())
        {
            return;
        }

        // Change la couleur de l'élément actuellement sélectionné
        m_menuItems[m_selectedIndex].textObject.setFillColor(m_textColor);

        // Sélectionne l'élément suivant
        m_selectedIndex = (m_selectedIndex + 1) % m_menuItems.size();

        // Change la couleur du nouvel élément sélectionné
        m_menuItems[m_selectedIndex].textObject.setFillColor(m_selectedTextColor);
    }

    void MenuState::activateSelected()
    {
        if (!m_menuItems.empty() && m_selectedIndex < m_menuItems.size())
        {
            // Appelle la fonction callback de l'élément sélectionné
            if (m_menuItems[m_selectedIndex].callback)
            {
                m_menuItems[m_selectedIndex].callback();
            }
        }
    }

} // namespace Orenji