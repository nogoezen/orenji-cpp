#pragma once

#include "State/State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>

namespace Orenji
{
    /**
     * @class MenuState
     * @brief Représente un état de menu dans le jeu
     *
     * Cette classe implémente un menu simple avec des options sélectionnables
     */
    class MenuState : public State
    {
    public:
        /**
         * @struct MenuItem
         * @brief Structure pour un élément de menu
         */
        struct MenuItem
        {
            std::string text;               ///< Texte affiché pour l'option
            std::function<void()> callback; ///< Fonction appelée lorsque l'option est sélectionnée
            sf::Text textObject;            ///< Objet texte SFML
        };

        /**
         * @brief Constructeur
         * @param title Titre du menu
         */
        MenuState(const std::string &title = "Menu");

        /**
         * @brief Destructeur
         */
        virtual ~MenuState() override = default;

        /**
         * @brief Initialise l'état du menu
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize() override;

        /**
         * @brief Met à jour l'état du menu
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Gère les événements
         * @param event L'événement SFML à traiter
         */
        virtual void handleEvent(const sf::Event &event) override;

        /**
         * @brief Dessine le menu
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        virtual void render(sf::RenderWindow &window) override;

        /**
         * @brief Appelé lorsque l'état devient actif
         */
        virtual void onActive() override;

        /**
         * @brief Appelé lorsque l'état devient inactif
         */
        virtual void onInactive() override;

        /**
         * @brief Ajoute une option au menu
         * @param text Texte de l'option
         * @param callback Fonction à appeler lorsque l'option est sélectionnée
         */
        void addMenuItem(const std::string &text, std::function<void()> callback);

        /**
         * @brief Définit le titre du menu
         * @param title Nouveau titre
         */
        void setTitle(const std::string &title);

        /**
         * @brief Définit la couleur de fond
         * @param color Couleur de fond
         */
        void setBackgroundColor(const sf::Color &color);

        /**
         * @brief Définit la couleur du texte normal
         * @param color Couleur du texte
         */
        void setTextColor(const sf::Color &color);

        /**
         * @brief Définit la couleur du texte sélectionné
         * @param color Couleur du texte sélectionné
         */
        void setSelectedTextColor(const sf::Color &color);

        /**
         * @brief Définit la taille du texte
         * @param size Taille du texte
         */
        void setTextSize(unsigned int size);

        /**
         * @brief Définit la taille du texte du titre
         * @param size Taille du texte du titre
         */
        void setTitleTextSize(unsigned int size);

    private:
        /**
         * @brief Calcule les positions des éléments du menu
         */
        void updateLayout();

        /**
         * @brief Sélectionne l'option précédente
         */
        void selectPrevious();

        /**
         * @brief Sélectionne l'option suivante
         */
        void selectNext();

        /**
         * @brief Active l'option sélectionnée
         */
        void activateSelected();

        std::string m_title;             ///< Titre du menu
        sf::Text m_titleText;            ///< Texte SFML pour le titre
        sf::Font m_font;                 ///< Police utilisée
        sf::RectangleShape m_background; ///< Fond du menu

        std::vector<MenuItem> m_menuItems; ///< Options du menu
        size_t m_selectedIndex;            ///< Index de l'option sélectionnée

        sf::Color m_backgroundColor;   ///< Couleur de fond
        sf::Color m_textColor;         ///< Couleur du texte
        sf::Color m_selectedTextColor; ///< Couleur du texte sélectionné

        unsigned int m_textSize;      ///< Taille du texte
        unsigned int m_titleTextSize; ///< Taille du texte du titre

        float m_itemSpacing;     ///< Espacement entre les options
        bool m_needLayoutUpdate; ///< Indique si la mise en page doit être mise à jour
    };

} // namespace Orenji