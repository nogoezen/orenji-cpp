#include "../../include/UI/UIManager.hpp"
#include <iostream>
#include <variant>
#include <filesystem>

namespace UI
{

    UIManager::UIManager(sf::RenderWindow &window)
        : m_window(window), m_gui(window), m_defaultTheme("Default")
    {
        std::cout << "UIManager created with TGUI integration" << std::endl;
    }

    UIManager::~UIManager()
    {
        std::cout << "UIManager destroyed" << std::endl;
    }

    void UIManager::init()
    {
        std::cout << "Initializing UIManager with TGUI " << TGUI_VERSION_MAJOR << "."
                  << TGUI_VERSION_MINOR << "." << TGUI_VERSION_PATCH << std::endl;

        // Load default themes
        loadTheme("Black", "lib/tgui/themes/Black.txt");
        loadTheme("BabyBlue", "lib/tgui/themes/BabyBlue.txt");
        loadTheme("TransparentGrey", "lib/tgui/themes/TransparentGrey.txt");

        // Set default theme
        setDefaultTheme("Black");
    }

    void UIManager::update(float deltaTime)
    {
        // Nothing specific to update in TGUI (it updates on events and draw)
    }

    void UIManager::render()
    {
        // Draw the GUI
        m_gui.draw();
    }

    bool UIManager::handleEvent(const sf::Event &event)
    {
        // Let TGUI handle the event
        if (m_gui.handleEvent(event))
        {
            return true; // Event was processed by the GUI
        }
        return false;
    }

    bool UIManager::loadForm(FormType type, const std::string &filename)
    {
        try
        {
            // Create a panel to hold the form
            auto panel = tgui::Panel::create();
            panel->setSize("100%", "100%");
            panel->setVisible(false);

            // Try to load widgets from file into the panel
            const std::filesystem::path filePath = filename;
            if (!std::filesystem::exists(filePath))
            {
                std::cerr << "UI form file not found: " << filename << std::endl;
                return false;
            }

            tgui::WidgetLoader loader;
            loader.load(panel, filename);

            // Add to forms map and to the GUI
            m_forms[type] = panel;
            m_gui.add(panel);

            // Apply default theme
            if (!m_defaultTheme.empty() && m_themes.find(m_defaultTheme) != m_themes.end())
            {
                applyTheme(type, m_defaultTheme);
            }

            return true;
        }
        catch (const tgui::Exception &e)
        {
            std::cerr << "Failed to load TGUI form: " << e.what() << std::endl;
            return false;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error loading UI form: " << e.what() << std::endl;
            return false;
        }
    }

    tgui::Panel::Ptr UIManager::createForm(FormType type, const std::string &name)
    {
        // Create a panel to hold the form elements
        auto panel = tgui::Panel::create();
        panel->setSize("100%", "100%");
        panel->setVisible(false);
        panel->setWidgetName(name);

        // Add to forms map and to the GUI
        m_forms[type] = panel;
        m_gui.add(panel);

        // Apply default theme
        if (!m_defaultTheme.empty() && m_themes.find(m_defaultTheme) != m_themes.end())
        {
            applyTheme(type, m_defaultTheme);
        }

        return panel;
    }

    void UIManager::showForm(FormType type)
    {
        // Hide all forms first
        hideAllForms();

        // Show the requested form
        auto it = m_forms.find(type);
        if (it != m_forms.end() && it->second)
        {
            it->second->setVisible(true);
        }
    }

    void UIManager::hideForm(FormType type)
    {
        auto it = m_forms.find(type);
        if (it != m_forms.end() && it->second)
        {
            it->second->setVisible(false);
        }
    }

    void UIManager::hideAllForms()
    {
        for (auto &pair : m_forms)
        {
            if (pair.second)
            {
                pair.second->setVisible(false);
            }
        }
    }

    tgui::Panel::Ptr UIManager::getForm(FormType type)
    {
        auto it = m_forms.find(type);
        if (it != m_forms.end())
        {
            return it->second;
        }
        return nullptr;
    }

    tgui::Widget::Ptr UIManager::getWidget(FormType type, const std::string &widgetName)
    {
        auto form = getForm(type);
        if (form)
        {
            return form->get(widgetName);
        }
        return nullptr;
    }

    bool UIManager::loadTheme(const std::string &themeName, const std::string &filename)
    {
        try
        {
            tgui::Theme theme(filename);
            m_themes[themeName] = theme;
            return true;
        }
        catch (const tgui::Exception &e)
        {
            std::cerr << "Failed to load TGUI theme: " << e.what() << std::endl;
            return false;
        }
    }

    void UIManager::setDefaultTheme(const std::string &themeName)
    {
        if (m_themes.find(themeName) != m_themes.end())
        {
            m_defaultTheme = themeName;

            // Apply to all existing forms
            for (auto &pair : m_forms)
            {
                applyTheme(pair.first, themeName);
            }
        }
        else
        {
            std::cerr << "Theme not found: " << themeName << std::endl;
        }
    }

    void UIManager::applyTheme(FormType type, const std::string &themeName)
    {
        auto it = m_themes.find(themeName);
        if (it == m_themes.end())
        {
            std::cerr << "Theme not found: " << themeName << std::endl;
            return;
        }

        auto form = getForm(type);
        if (!form)
        {
            std::cerr << "Form not found for type: " << static_cast<int>(type) << std::endl;
            return;
        }

        // Apply theme to all widgets in the form
        for (const auto &widget : form->getWidgets())
        {
            try
            {
                it->second.setWidgetTheme(widget);
            }
            catch (const tgui::Exception &e)
            {
                std::cerr << "Failed to apply theme to widget: " << e.what() << std::endl;
            }
        }
    }

} // namespace UI