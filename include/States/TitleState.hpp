#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include <vector>
#include <memory>

namespace Orenji
{
    /**
     * @brief État du menu titre
     */
    class TitleState : public State
    {
    public:
        /**
         * @brief Constructeur
         */
        TitleState();

        /**
         * @brief Destructeur
         */
        ~TitleState() override;

        /**
         * @brief Initialisation de l'état
         */
        bool initialize() override;

        /**
         * @brief Mise à jour de l'état
         * @param deltaTime Temps écoulé depuis la dernière mise à jour en secondes
         */
        bool update(float deltaTime) override;

        /**
         * @brief Gestion des événements
         * @param event Événement à traiter
         */
        void handleEvent(sf::Event &event);

        /**
         * @brief Rendu de l'état
         */
        bool render() override;

        /**
         * @brief Rendu de l'état avec accès à la fenêtre
         * @param window Fenêtre de rendu
         */
        void render(sf::RenderWindow &window) override;

        /**
         * @brief Gestion des entrées utilisateur
         */
        bool handleInput() override;

    private:
        /**
         * @brief Structure pour un élément de menu
         */
        struct MenuItem
        {
            sf::Text text;                  ///< Texte de l'élément
            bool isSelected;                ///< Indique si l'élément est sélectionné
            std::function<void()> callback; ///< Fonction à appeler quand l'élément est activé
        };

        sf::Texture m_backgroundTexture;   ///< Texture de l'arrière-plan
        sf::Sprite m_backgroundSprite;     ///< Sprite de l'arrière-plan
        sf::Font m_font;                   ///< Police de caractères
        sf::Text m_titleText;              ///< Texte du titre
        sf::Music m_music;                 ///< Musique de fond
        std::vector<MenuItem> m_menuItems; ///< Éléments du menu
        int m_selectedItemIndex;           ///< Index de l'élément sélectionné
        float m_fadeAlpha;                 ///< Alpha pour l'effet de fondu
        bool m_isExiting;                  ///< Indique si on quitte l'état
        float m_titleY;                    ///< Position Y du titre pour l'animation
        float m_titleYSpeed;               ///< Vitesse de l'animation du titre
        sf::RectangleShape m_fadeRect;     ///< Rectangle pour l'effet de fondu

        /**
         * @brief Sélectionne l'élément de menu suivant
         */
        void selectNextItem();

        /**
         * @brief Sélectionne l'élément de menu précédent
         */
        void selectPreviousItem();

        /**
         * @brief Active l'élément de menu sélectionné
         */
        void activateItem();

        /**
         * @brief Démarre un nouvel état de jeu
         */
        void startGame();

        /**
         * @brief Affiche les options
         */
        void showOptions();

        /**
         * @brief Quitte le jeu
         */
        void exitGame();
    };

} // namespace Orenji