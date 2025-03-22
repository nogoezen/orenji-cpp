#pragma once

#include "States/State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace Orenji
{
    /**
     * @brief État du menu principal
     *
     * Cet état gère l'affichage et l'interaction avec le menu principal du jeu.
     */
    class MainMenuState : public State
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param machine Référence vers la machine à états
         */
        MainMenuState(StateMachine &machine);

        /**
         * @brief Destructeur
         */
        ~MainMenuState() = default;

        /**
         * @brief Initialise le menu
         */
        void onEnter() override;

        /**
         * @brief Nettoie le menu
         */
        void onExit() override;

        /**
         * @brief Met à jour le menu
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        void update(float dt) override;

        /**
         * @brief Gère les événements du menu
         *
         * @param event Événement SFML à traiter
         */
        void handleEvent(const sf::Event &event) override;

        /**
         * @brief Dessine le menu
         *
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target) override;

        /**
         * @brief Récupère l'identifiant de l'état
         *
         * @return Identifiant de l'état
         */
        std::string getStateID() const override { return "MainMenu"; }

    private:
        /**
         * @brief Centrer un texte horizontalement
         *
         * @param text Texte à centrer
         * @param y Position Y du texte
         * @param windowWidth Largeur de la fenêtre
         */
        void centerText(sf::Text &text, float y, float windowWidth);

        /**
         * @brief Mettre à jour la sélection des options du menu
         *
         * @param direction Direction (-1 = haut, 1 = bas)
         */
        void updateMenuSelection(int direction);

        /**
         * @brief Exécuter l'action associée à l'option sélectionnée
         */
        void executeSelectedAction();

    private:
        sf::Font m_font;                   ///< Police utilisée pour le menu
        sf::Text m_titleText;              ///< Texte du titre
        std::vector<sf::Text> m_menuItems; ///< Textes des options du menu
        int m_selectedItemIndex;           ///< Index de l'option sélectionnée

        sf::Texture m_backgroundTexture; ///< Texture de fond
        sf::Sprite m_backgroundSprite;   ///< Sprite de fond
    };

} // namespace Orenji