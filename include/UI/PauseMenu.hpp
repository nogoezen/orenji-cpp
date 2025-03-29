#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "UI/UIManager.hpp"
#include <functional>
#include <memory>

namespace UI
{
    /**
     * @brief Pause Menu class that can be attached to any game scene
     */
    class PauseMenu
    {
    public:
        /**
         * @brief Constructor
         * @param uiManager Pointer to the UI manager
         */
        PauseMenu(UIManager *uiManager);

        /**
         * @brief Destructor
         */
        ~PauseMenu();

        /**
         * @brief Initialize the pause menu
         */
        void init();

        /**
         * @brief Show the pause menu
         */
        void show();

        /**
         * @brief Hide the pause menu
         */
        void hide();

        /**
         * @brief Toggle the visibility of the pause menu
         * @return true if the menu is now visible
         */
        bool toggle();

        /**
         * @brief Check if the pause menu is visible
         * @return true if visible
         */
        bool isVisible() const;

        /**
         * @brief Set the callback for resume button
         * @param callback Function to call
         */
        void setResumeCallback(const std::function<void()> &callback);

        /**
         * @brief Set the callback for settings button
         * @param callback Function to call
         */
        void setSettingsCallback(const std::function<void()> &callback);

        /**
         * @brief Set the callback for main menu button
         * @param callback Function to call
         */
        void setMainMenuCallback(const std::function<void()> &callback);

        /**
         * @brief Set the callback for exit button
         * @param callback Function to call
         */
        void setExitCallback(const std::function<void()> &callback);

    private:
        UIManager *m_uiManager;
        tgui::Panel::Ptr m_panel;
        bool m_isVisible;

        // Buttons
        tgui::Button::Ptr m_resumeButton;
        tgui::Button::Ptr m_settingsButton;
        tgui::Button::Ptr m_mainMenuButton;
        tgui::Button::Ptr m_exitButton;

        // Callbacks
        std::function<void()> m_resumeCallback;
        std::function<void()> m_settingsCallback;
        std::function<void()> m_mainMenuCallback;
        std::function<void()> m_exitCallback;

        // Default callbacks
        void defaultResumeCallback();
        void defaultSettingsCallback();
        void defaultMainMenuCallback();
        void defaultExitCallback();
    };

} // namespace UI