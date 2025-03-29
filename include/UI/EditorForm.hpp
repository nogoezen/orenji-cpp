#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "UI/UIManager.hpp"
#include <functional>
#include <memory>
#include <vector>
#include <string>

namespace UI
{
    /**
     * @brief Property type enum for editor properties
     */
    enum class PropertyType
    {
        INT,
        FLOAT,
        BOOL,
        STRING,
        COLOR,
        POSITION,
        SIZE,
        SELECTION
    };

    /**
     * @brief Property structure for editor properties
     */
    struct Property
    {
        std::string name;
        PropertyType type;
        std::variant<int, float, bool, std::string, sf::Color, sf::Vector2f, std::vector<std::string>> value;
        std::function<void(const Property &)> changeCallback;
    };

    /**
     * @brief Editor Form class for game object editing
     */
    class EditorForm
    {
    public:
        /**
         * @brief Constructor
         * @param uiManager Pointer to the UI manager
         */
        EditorForm(UIManager *uiManager);

        /**
         * @brief Destructor
         */
        ~EditorForm();

        /**
         * @brief Initialize the editor form
         */
        void init();

        /**
         * @brief Show the editor form
         */
        void show();

        /**
         * @brief Hide the editor form
         */
        void hide();

        /**
         * @brief Check if the editor form is visible
         * @return true if visible
         */
        bool isVisible() const;

        /**
         * @brief Add a property to the editor
         * @param property Property to add
         */
        void addProperty(const Property &property);

        /**
         * @brief Update a property value
         * @param name Property name
         * @param value New property value
         */
        template <typename T>
        void updateProperty(const std::string &name, const T &value)
        {
            for (auto &prop : m_properties)
            {
                if (prop.name == name)
                {
                    try
                    {
                        prop.value = value;
                        updatePropertyWidget(prop);
                        break;
                    }
                    catch (const std::bad_variant_access &)
                    {
                        // Type mismatch
                    }
                }
            }
        }

        /**
         * @brief Clear all properties
         */
        void clearProperties();

        /**
         * @brief Set the save callback
         * @param callback Function to call when save is pressed
         */
        void setSaveCallback(const std::function<void()> &callback);

        /**
         * @brief Set the close callback
         * @param callback Function to call when close is pressed
         */
        void setCloseCallback(const std::function<void()> &callback);

    private:
        UIManager *m_uiManager;
        tgui::Panel::Ptr m_panel;
        tgui::ScrollablePanel::Ptr m_propertiesPanel;
        bool m_isVisible;

        std::vector<Property> m_properties;
        std::function<void()> m_saveCallback;
        std::function<void()> m_closeCallback;

        // Helper methods
        void createPropertyWidget(const Property &property, float yPosition);
        void updatePropertyWidget(const Property &property);
        std::string getPropertyValueString(const Property &property);
        void rebuildPropertyWidgets();

        // Default callbacks
        void defaultSaveCallback();
        void defaultCloseCallback();
    };

} // namespace UI