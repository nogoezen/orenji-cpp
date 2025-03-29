#include "UI/EditorForm.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace UI
{
    EditorForm::EditorForm(UIManager *uiManager)
        : m_uiManager(uiManager), m_isVisible(false)
    {
        // Set default callbacks
        m_saveCallback = [this]()
        { this->defaultSaveCallback(); };
        m_closeCallback = [this]()
        { this->defaultCloseCallback(); };
    }

    EditorForm::~EditorForm()
    {
        // Clean up if needed
    }

    void EditorForm::init()
    {
        // Create the editor form if it doesn't exist
        if (!m_uiManager->getForm(FormType::EDITOR))
        {
            m_panel = m_uiManager->createForm(FormType::EDITOR, "EditorForm");

            // Create main panel
            auto mainPanel = tgui::Panel::create();
            mainPanel->setSize("100%", "100%");
            mainPanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40, 230));
            m_panel->add(mainPanel);

            // Add title
            auto titleLabel = tgui::Label::create("Object Editor");
            titleLabel->setPosition(20.f, 20.f);
            titleLabel->setTextSize(24);
            titleLabel->getRenderer()->setTextColor(tgui::Color(255, 165, 0));
            mainPanel->add(titleLabel);

            // Add close button
            auto closeButton = tgui::Button::create("×");
            closeButton->setSize(30.f, 30.f);
            closeButton->setPosition("95%", 20.f);
            closeButton->setOrigin(0.5f, 0.5f);
            closeButton->onPress([this]()
                                 { m_closeCallback(); });
            mainPanel->add(closeButton);

            // Add properties panel
            m_propertiesPanel = tgui::ScrollablePanel::create();
            m_propertiesPanel->setSize("95%", "70%");
            m_propertiesPanel->setPosition("50%", "45%");
            m_propertiesPanel->setOrigin(0.5f, 0.5f);
            m_propertiesPanel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50));
            m_propertiesPanel->getRenderer()->setBorderColor(tgui::Color(100, 100, 100));
            m_propertiesPanel->getRenderer()->setBorders({1, 1, 1, 1});
            mainPanel->add(m_propertiesPanel);

            // Add action buttons
            auto saveButton = tgui::Button::create("Save");
            saveButton->setSize(100.f, 40.f);
            saveButton->setPosition("40%", "90%");
            saveButton->setOrigin(0.5f, 0.5f);
            saveButton->onPress([this]()
                                { m_saveCallback(); });
            mainPanel->add(saveButton);

            auto cancelButton = tgui::Button::create("Cancel");
            cancelButton->setSize(100.f, 40.f);
            cancelButton->setPosition("60%", "90%");
            cancelButton->setOrigin(0.5f, 0.5f);
            cancelButton->onPress([this]()
                                  { m_closeCallback(); });
            mainPanel->add(cancelButton);
        }
        else
        {
            // Get the existing form
            m_panel = m_uiManager->getForm(FormType::EDITOR);

            // Find the properties panel
            auto mainPanel = m_panel->get("EditorForm");
            if (mainPanel)
            {
                m_propertiesPanel = mainPanel->get("PropertiesPanel")->cast<tgui::ScrollablePanel>();
            }
        }

        // Ensure the editor starts hidden
        hide();
    }

    void EditorForm::show()
    {
        if (m_panel)
        {
            m_uiManager->showForm(FormType::EDITOR);
            m_isVisible = true;
        }
    }

    void EditorForm::hide()
    {
        if (m_panel)
        {
            m_uiManager->hideForm(FormType::EDITOR);
            m_isVisible = false;
        }
    }

    bool EditorForm::isVisible() const
    {
        return m_isVisible;
    }

    void EditorForm::addProperty(const Property &property)
    {
        // Add property to list
        m_properties.push_back(property);

        // Rebuild the property widgets
        rebuildPropertyWidgets();
    }

    void EditorForm::clearProperties()
    {
        m_properties.clear();

        // Clear all property widgets
        if (m_propertiesPanel)
        {
            m_propertiesPanel->removeAllWidgets();
        }
    }

    void EditorForm::setSaveCallback(const std::function<void()> &callback)
    {
        m_saveCallback = callback;
    }

    void EditorForm::setCloseCallback(const std::function<void()> &callback)
    {
        m_closeCallback = callback;
    }

    void EditorForm::rebuildPropertyWidgets()
    {
        if (!m_propertiesPanel)
            return;

        // Clear existing widgets
        m_propertiesPanel->removeAllWidgets();

        // Create property widgets
        float yPosition = 10.f;
        const float rowHeight = 30.f;
        const float margin = 10.f;

        for (const auto &property : m_properties)
        {
            createPropertyWidget(property, yPosition);
            yPosition += rowHeight + margin;
        }

        // Update panel content size
        m_propertiesPanel->setContentSize(m_propertiesPanel->getSize().x, yPosition);
    }

    void EditorForm::createPropertyWidget(const Property &property, float yPosition)
    {
        if (!m_propertiesPanel)
            return;

        // Create property label
        auto label = tgui::Label::create(property.name);
        label->setPosition(10.f, yPosition);
        label->setTextSize(14);
        label->setSize(150.f, 30.f);
        m_propertiesPanel->add(label);

        // Create property input based on type
        switch (property.type)
        {
        case PropertyType::INT:
        {
            auto editBox = tgui::EditBox::create();
            editBox->setPosition(170.f, yPosition);
            editBox->setSize(200.f, 30.f);
            editBox->setText(getPropertyValueString(property));
            editBox->setInputValidator(tgui::EditBox::Validator::Int);

            editBox->onTextChange([this, property](const tgui::String &text)
                                  {
                    // Update property value
                    Property updatedProp = property;
                    try {
                        updatedProp.value = std::stoi(text.toStdString());
                        if (updatedProp.changeCallback)
                            updatedProp.changeCallback(updatedProp);
                    }
                    catch (...) {
                        // Invalid conversion
                    } });

            m_propertiesPanel->add(editBox);
            break;
        }
        case PropertyType::FLOAT:
        {
            auto editBox = tgui::EditBox::create();
            editBox->setPosition(170.f, yPosition);
            editBox->setSize(200.f, 30.f);
            editBox->setText(getPropertyValueString(property));
            editBox->setInputValidator(tgui::EditBox::Validator::Float);

            editBox->onTextChange([this, property](const tgui::String &text)
                                  {
                    // Update property value
                    Property updatedProp = property;
                    try {
                        updatedProp.value = std::stof(text.toStdString());
                        if (updatedProp.changeCallback)
                            updatedProp.changeCallback(updatedProp);
                    }
                    catch (...) {
                        // Invalid conversion
                    } });

            m_propertiesPanel->add(editBox);
            break;
        }
        case PropertyType::BOOL:
        {
            auto checkbox = tgui::CheckBox::create();
            checkbox->setPosition(170.f, yPosition);
            checkbox->setSize(30.f, 30.f);

            try
            {
                checkbox->setChecked(std::get<bool>(property.value));
            }
            catch (...)
            {
                checkbox->setChecked(false);
            }

            checkbox->onChange([this, property](bool checked)
                               {
                    // Update property value
                    Property updatedProp = property;
                    updatedProp.value = checked;
                    if (updatedProp.changeCallback)
                        updatedProp.changeCallback(updatedProp); });

            m_propertiesPanel->add(checkbox);
            break;
        }
        case PropertyType::STRING:
        {
            auto editBox = tgui::EditBox::create();
            editBox->setPosition(170.f, yPosition);
            editBox->setSize(200.f, 30.f);
            editBox->setText(getPropertyValueString(property));

            editBox->onTextChange([this, property](const tgui::String &text)
                                  {
                    // Update property value
                    Property updatedProp = property;
                    updatedProp.value = text.toStdString();
                    if (updatedProp.changeCallback)
                        updatedProp.changeCallback(updatedProp); });

            m_propertiesPanel->add(editBox);
            break;
        }
        case PropertyType::COLOR:
        {
            auto colorButton = tgui::Button::create();
            colorButton->setPosition(170.f, yPosition);
            colorButton->setSize(200.f, 30.f);

            try
            {
                sf::Color color = std::get<sf::Color>(property.value);
                colorButton->getRenderer()->setBackgroundColor(tgui::Color(color.r, color.g, color.b, color.a));
                colorButton->setText("#" + getPropertyValueString(property));
            }
            catch (...)
            {
                colorButton->getRenderer()->setBackgroundColor(tgui::Color::White);
                colorButton->setText("#FFFFFF");
            }

            colorButton->onPress([this, property]()
                                 {
                    // In a real implementation, we would display a color picker here
                    std::cout << "Color picker would be shown here" << std::endl; });

            m_propertiesPanel->add(colorButton);
            break;
        }
        case PropertyType::POSITION:
        case PropertyType::SIZE:
        {
            // X value
            auto xBox = tgui::EditBox::create();
            xBox->setPosition(170.f, yPosition);
            xBox->setSize(95.f, 30.f);
            xBox->setInputValidator(tgui::EditBox::Validator::Float);

            // Y value
            auto yBox = tgui::EditBox::create();
            yBox->setPosition(275.f, yPosition);
            yBox->setSize(95.f, 30.f);
            yBox->setInputValidator(tgui::EditBox::Validator::Float);

            try
            {
                sf::Vector2f vec = std::get<sf::Vector2f>(property.value);
                xBox->setText(std::to_string(vec.x));
                yBox->setText(std::to_string(vec.y));
            }
            catch (...)
            {
                xBox->setText("0");
                yBox->setText("0");
            }

            auto updateVectorProperty = [this, property](tgui::EditBox::Ptr xBox, tgui::EditBox::Ptr yBox)
            {
                try
                {
                    float x = std::stof(xBox->getText().toStdString());
                    float y = std::stof(yBox->getText().toStdString());

                    Property updatedProp = property;
                    updatedProp.value = sf::Vector2f(x, y);
                    if (updatedProp.changeCallback)
                        updatedProp.changeCallback(updatedProp);
                }
                catch (...)
                {
                    // Invalid conversion
                }
            };

            xBox->onTextChange([=](const tgui::String &)
                               { updateVectorProperty(xBox, yBox); });

            yBox->onTextChange([=](const tgui::String &)
                               { updateVectorProperty(xBox, yBox); });

            m_propertiesPanel->add(xBox);
            m_propertiesPanel->add(yBox);
            break;
        }
        case PropertyType::SELECTION:
        {
            auto comboBox = tgui::ComboBox::create();
            comboBox->setPosition(170.f, yPosition);
            comboBox->setSize(200.f, 30.f);

            try
            {
                auto options = std::get<std::vector<std::string>>(property.value);
                for (const auto &option : options)
                {
                    comboBox->addItem(option);
                }

                if (!options.empty())
                {
                    comboBox->setSelectedItem(options[0]);
                }
            }
            catch (...)
            {
                // No options
            }

            comboBox->onItemSelect([this, property](const tgui::String &selectedItem)
                                   {
                    // Update property value (we keep the options list but change the selected item to first)
                    Property updatedProp = property;
                    try {
                        auto options = std::get<std::vector<std::string>>(property.value);
                        if (!options.empty()) {
                            std::vector<std::string> newOptions = options;
                            
                            // Move selected item to front
                            auto selection = selectedItem.toStdString();
                            auto it = std::find(newOptions.begin(), newOptions.end(), selection);
                            if (it != newOptions.end()) {
                                std::rotate(newOptions.begin(), it, it + 1);
                            }
                            
                            updatedProp.value = newOptions;
                            if (updatedProp.changeCallback)
                                updatedProp.changeCallback(updatedProp);
                        }
                    } catch (...) {
                        // Invalid conversion
                    } });

            m_propertiesPanel->add(comboBox);
            break;
        }
        }
    }

    void EditorForm::updatePropertyWidget(const Property &property)
    {
        // For simplicity, we just rebuild all widgets
        rebuildPropertyWidgets();
    }

    std::string EditorForm::getPropertyValueString(const Property &property)
    {
        std::stringstream ss;

        try
        {
            switch (property.type)
            {
            case PropertyType::INT:
                ss << std::get<int>(property.value);
                break;
            case PropertyType::FLOAT:
                ss << std::fixed << std::setprecision(2) << std::get<float>(property.value);
                break;
            case PropertyType::BOOL:
                ss << (std::get<bool>(property.value) ? "true" : "false");
                break;
            case PropertyType::STRING:
                ss << std::get<std::string>(property.value);
                break;
            case PropertyType::COLOR:
            {
                sf::Color color = std::get<sf::Color>(property.value);
                ss << std::hex << std::setfill('0')
                   << std::setw(2) << static_cast<int>(color.r)
                   << std::setw(2) << static_cast<int>(color.g)
                   << std::setw(2) << static_cast<int>(color.b);
                break;
            }
            case PropertyType::POSITION:
            case PropertyType::SIZE:
            {
                sf::Vector2f vec = std::get<sf::Vector2f>(property.value);
                ss << vec.x << ", " << vec.y;
                break;
            }
            case PropertyType::SELECTION:
            {
                auto options = std::get<std::vector<std::string>>(property.value);
                if (!options.empty())
                    ss << options[0]; // Show the first (selected) option
                break;
            }
            }
        }
        catch (...)
        {
            ss << "Invalid value";
        }

        return ss.str();
    }

    void EditorForm::defaultSaveCallback()
    {
        std::cout << "Save properties (default action)" << std::endl;

        // Print all properties for debugging
        for (const auto &prop : m_properties)
        {
            std::cout << "Property: " << prop.name << " = " << getPropertyValueString(prop) << std::endl;
        }

        hide();
    }

    void EditorForm::defaultCloseCallback()
    {
        std::cout << "Close editor (default action)" << std::endl;
        hide();
    }

} // namespace UI