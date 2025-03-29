#include "UI/PauseMenu.hpp"
#include <iostream>

namespace UI
{
    PauseMenu::PauseMenu(UIManager *uiManager)
        : m_uiManager(uiManager), m_isVisible(false)
    {
        // Set default callbacks
        m_resumeCallback = [this]()
        { this->defaultResumeCallback(); };
        m_settingsCallback = [this]()
        { this->defaultSettingsCallback(); };
        m_mainMenuCallback = [this]()
        { this->defaultMainMenuCallback(); };
        m_exitCallback = [this]()
        { this->defaultExitCallback(); };
    }

    PauseMenu::~PauseMenu()
    {
        // Clean up if needed
    }

    void PauseMenu::init()
    {
        // Create the pause menu form if it doesn't exist
        if (!m_uiManager->getForm(FormType::PAUSE_MENU))
        {
            m_panel = m_uiManager->createForm(FormType::PAUSE_MENU, "PauseMenu");

            // Create semi-transparent background
            auto background = tgui::Panel::create();
            background->setSize("100%", "100%");
            background->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 175));
            m_panel->add(background);

            // Create centered panel for menu
            auto menuPanel = tgui::Panel::create();
            menuPanel->setSize(400.f, 500.f);
            menuPanel->setPosition("50%", "50%");
            menuPanel->setOrigin(0.5f, 0.5f);
            menuPanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40, 230));
            menuPanel->getRenderer()->setBorderColor(tgui::Color(255, 165, 0));
            menuPanel->getRenderer()->setBorders({2, 2, 2, 2});
            menuPanel->getRenderer()->setRoundedBorderRadius(5.f);
            background->add(menuPanel);

            // Title
            auto title = tgui::Label::create("PAUSE");
            title->setPosition("50%", "10%");
            title->setOrigin(0.5f, 0.5f);
            title->setTextSize(36);
            title->getRenderer()->setTextColor(tgui::Color(255, 165, 0));
            title->getRenderer()->setTextOutlineColor(tgui::Color::Black);
            title->getRenderer()->setTextOutlineThickness(1.5f);
            menuPanel->add(title);

            // Resume button
            m_resumeButton = tgui::Button::create("Resume Game");
            m_resumeButton->setSize(250.f, 50.f);
            m_resumeButton->setPosition("50%", "30%");
            m_resumeButton->setOrigin(0.5f, 0.5f);
            menuPanel->add(m_resumeButton);

            // Settings button
            m_settingsButton = tgui::Button::create("Settings");
            m_settingsButton->setSize(250.f, 50.f);
            m_settingsButton->setPosition("50%", "45%");
            m_settingsButton->setOrigin(0.5f, 0.5f);
            menuPanel->add(m_settingsButton);

            // Main menu button
            m_mainMenuButton = tgui::Button::create("Main Menu");
            m_mainMenuButton->setSize(250.f, 50.f);
            m_mainMenuButton->setPosition("50%", "60%");
            m_mainMenuButton->setOrigin(0.5f, 0.5f);
            menuPanel->add(m_mainMenuButton);

            // Exit button
            m_exitButton = tgui::Button::create("Exit Game");
            m_exitButton->setSize(250.f, 50.f);
            m_exitButton->setPosition("50%", "75%");
            m_exitButton->setOrigin(0.5f, 0.5f);
            menuPanel->add(m_exitButton);

            // Set up button callbacks
            m_resumeButton->onPress([this]()
                                    { m_resumeCallback(); });

            m_settingsButton->onPress([this]()
                                      { m_settingsCallback(); });

            m_mainMenuButton->onPress([this]()
                                      { m_mainMenuCallback(); });

            m_exitButton->onPress([this]()
                                  { m_exitCallback(); });
        }
        else
        {
            // Get the existing form
            m_panel = m_uiManager->getForm(FormType::PAUSE_MENU);

            // Retrieve button references
            if (m_panel)
            {
                auto menuPanel = m_panel->get("PauseMenu")->cast<tgui::Panel>();
                if (menuPanel)
                {
                    m_resumeButton = menuPanel->get("ResumeGame")->cast<tgui::Button>();
                    m_settingsButton = menuPanel->get("Settings")->cast<tgui::Button>();
                    m_mainMenuButton = menuPanel->get("MainMenu")->cast<tgui::Button>();
                    m_exitButton = menuPanel->get("ExitGame")->cast<tgui::Button>();
                }
            }
        }

        // Ensure the pause menu starts hidden
        hide();
    }

    void PauseMenu::show()
    {
        if (m_panel)
        {
            m_uiManager->showForm(FormType::PAUSE_MENU);
            m_isVisible = true;
        }
    }

    void PauseMenu::hide()
    {
        if (m_panel)
        {
            m_uiManager->hideForm(FormType::PAUSE_MENU);
            m_isVisible = false;
        }
    }

    bool PauseMenu::toggle()
    {
        if (m_isVisible)
        {
            hide();
        }
        else
        {
            show();
        }
        return m_isVisible;
    }

    bool PauseMenu::isVisible() const
    {
        return m_isVisible;
    }

    void PauseMenu::setResumeCallback(const std::function<void()> &callback)
    {
        m_resumeCallback = callback;
    }

    void PauseMenu::setSettingsCallback(const std::function<void()> &callback)
    {
        m_settingsCallback = callback;
    }

    void PauseMenu::setMainMenuCallback(const std::function<void()> &callback)
    {
        m_mainMenuCallback = callback;
    }

    void PauseMenu::setExitCallback(const std::function<void()> &callback)
    {
        m_exitCallback = callback;
    }

    void PauseMenu::defaultResumeCallback()
    {
        std::cout << "Resume game (default action)" << std::endl;
        hide();
    }

    void PauseMenu::defaultSettingsCallback()
    {
        std::cout << "Open settings (default action)" << std::endl;
        m_uiManager->showForm(FormType::OPTIONS_MENU);
    }

    void PauseMenu::defaultMainMenuCallback()
    {
        std::cout << "Return to main menu (default action)" << std::endl;
        hide();
        m_uiManager->showForm(FormType::MAIN_MENU);
    }

    void PauseMenu::defaultExitCallback()
    {
        std::cout << "Exit game (default action)" << std::endl;
        // Normally would close the window or trigger game exit
    }

} // namespace UI