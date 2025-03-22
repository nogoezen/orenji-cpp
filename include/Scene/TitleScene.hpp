#pragma once

#include "Scene/Scene.hpp"
#include "Scene/TextNode.hpp"
#include "Scene/SpriteNode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <functional>

namespace Orenji
{
    /**
     * @class TitleScene
     * @brief Scène du menu titre du jeu
     */
    class TitleScene : public Scene
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom de la scène
         */
        explicit TitleScene(const std::string &name = "TitleScene");

        /**
         * @brief Destructeur
         */
        virtual ~TitleScene();

        /**
         * @brief Initialise la scène
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize() override;

        /**
         * @brief Met à jour la scène
         * @param dt Delta time (temps écoulé depuis la dernière mise à jour)
         */
        virtual void update(float dt) override;

        /**
         * @brief Fonction appelée lorsque la scène est activée
         */
        virtual void onActivate() override;

        /**
         * @brief Fonction appelée lorsque la scène est désactivée
         */
        virtual void onDeactivate() override;

        /**
         * @brief Traite un événement SFML
         * @param event Événement à traiter
         */
        void handleEvent(const sf::Event &event);

        /**
         * @brief Ajoute une option au menu
         * @param text Texte de l'option
         * @param callback Fonction à appeler lorsque l'option est sélectionnée
         */
        void addMenuItem(const std::string &text, std::function<void()> callback);

        /**
         * @brief Définit le titre principal
         * @param title Nouveau titre
         */
        void setMainTitle(const std::string &title);

        /**
         * @brief Définit le sous-titre
         * @param subtitle Nouveau sous-titre
         */
        void setSubtitle(const std::string &subtitle);

    private:
        /**
         * @brief Structure pour stocker une option de menu
         */
        struct MenuItem
        {
            TextNode *textNode;             ///< Node de texte pour l'option
            std::function<void()> callback; ///< Fonction à appeler lorsque l'option est sélectionnée
        };

        /**
         * @brief Crée le fond de la scène
         * @param texturePath Chemin de l'image de fond
         */
        void createBackground(const std::string &texturePath);

        /**
         * @brief Crée les textes du menu
         */
        void createTexts();

        /**
         * @brief Met à jour la disposition des éléments du menu
         */
        void updateLayout();

        /**
         * @brief Sélectionne l'option suivante
         */
        void selectNextItem();

        /**
         * @brief Sélectionne l'option précédente
         */
        void selectPreviousItem();

        /**
         * @brief Exécute l'option sélectionnée
         */
        void activateSelectedItem();

        sf::Font m_font;                   ///< Police pour les textes
        sf::Texture m_backgroundTexture;   ///< Texture du fond
        sf::Music m_music;                 ///< Musique du menu titre
        SpriteNode *m_backgroundSprite;    ///< Sprite du fond
        TextNode *m_titleNode;             ///< Texte du titre principal
        TextNode *m_subtitleNode;          ///< Texte du sous-titre
        std::vector<MenuItem> m_menuItems; ///< Options du menu
        size_t m_selectedItemIndex;        ///< Index de l'option sélectionnée
        float m_textAnimationTimer;        ///< Timer pour l'animation des textes
        bool m_showInstructions;           ///< Indique si les instructions sont affichées
    };

} // namespace Orenji