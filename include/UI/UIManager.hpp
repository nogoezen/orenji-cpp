#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/include/TGUI/TGUI.hpp>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace UI
{
    enum class FormType
    {
        MAIN_MENU,
        PAUSE_MENU,
        OPTIONS_MENU,
        GAME_HUD,
        EDITOR,
        CUSTOM
    };

    /**
     * @brief UI Manager class that handles GUI elements with TGUI
     */
    class UIManager
    {
    public:
        /**
         * @brief Constructor
         * @param window SFML render window
         */
        UIManager(sf::RenderWindow &window);

        /**
         * @brief Destructor
         */
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

        /**
         * @brief Load a GUI form from a file
         * @param type Form type
         * @param filename Name of the file to load
         * @return true if loading was successful
         */
        bool loadForm(FormType type, const std::string &filename);

        /**
         * @brief Create a new GUI form
         * @param type Form type
         * @param name Name for the form
         * @return Pointer to the form's gui container
         */
        tgui::Panel::Ptr createForm(FormType type, const std::string &name);

        /**
         * @brief Show a GUI form
         * @param type Form type to show
         */
        void showForm(FormType type);

        /**
         * @brief Hide a GUI form
         * @param type Form type to hide
         */
        void hideForm(FormType type);

        /**
         * @brief Hide all GUI forms
         */
        void hideAllForms();

        /**
         * @brief Get a form by type
         * @param type Form type
         * @return Pointer to the GUI form (nullptr if not found)
         */
        tgui::Panel::Ptr getForm(FormType type);

        /**
         * @brief Get a widget from a form
         * @param type Form type
         * @param widgetName Name of the widget
         * @return Pointer to the widget (nullptr if not found)
         */
        tgui::Widget::Ptr getWidget(FormType type, const std::string &widgetName);

        /**
         * @brief Set a callback for a widget
         * @param type Form type
         * @param widgetName Name of the widget
         * @param signalName Name of the signal to connect to
         * @param callback Function to call when signal is triggered
         * @return ID of the connected signal
         */
        template <typename F>
        unsigned int setCallback(FormType type, const std::string &widgetName,
                                 const std::string &signalName, F &&callback)
        {
            auto widget = getWidget(type, widgetName);
            if (widget)
            {
                return widget->getSignal(signalName).connect(std::forward<F>(callback));
            }
            return 0;
        }

        /**
         * @brief Load a theme file
         * @param themeName Name to give the theme
         * @param filename Theme file path
         * @return true if loading was successful
         */
        bool loadTheme(const std::string &themeName, const std::string &filename);

        /**
         * @brief Set the default theme for all forms
         * @param themeName Name of the theme to use as default
         */
        void setDefaultTheme(const std::string &themeName);

        /**
         * @brief Apply a theme to a specific form
         * @param type Form type to apply theme to
         * @param themeName Name of the theme to apply
         */
        void applyTheme(FormType type, const std::string &themeName);

    private:
        sf::RenderWindow &m_window;
        tgui::Gui m_gui;
        std::unordered_map<FormType, tgui::Panel::Ptr> m_forms;
        std::unordered_map<std::string, tgui::Theme> m_themes;
        std::string m_defaultTheme;
    };

} // namespace UI