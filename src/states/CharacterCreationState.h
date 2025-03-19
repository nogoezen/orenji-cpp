#pragma once

#include "../core/State.h"
#include "../models/Player.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>
#include <memory>

class CharacterCreationState : public State
{
public:
    CharacterCreationState();
    ~CharacterCreationState() override = default;

    void enter() override;
    void exit() override;
    void pause() override;
    void resume() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

private:
    // Ressources graphiques
    std::shared_ptr<sf::Font> m_font;
    std::shared_ptr<sf::Sprite> m_background;
    std::vector<std::shared_ptr<sf::Text>> m_menuItems;

    // État de création
    int m_selectedItem;
    std::string m_characterName;
    std::string m_currentInput;
    bool m_isEnteringName;

    // Méthodes utilitaires
    void initializeGraphics();
    void updateMenuSelection();
    void handleMenuSelection();
    void handleNameInput();
    void createCharacter();
};