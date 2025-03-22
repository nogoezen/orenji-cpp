#pragma once

#include "Scene/SceneNode.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace Orenji
{
    /**
     * @class TextNode
     * @brief Node spécialisé pour l'affichage de texte
     */
    class TextNode : public SceneNode
    {
    public:
        /**
         * @brief Constructeur par défaut
         * @param name Nom du node
         */
        explicit TextNode(const std::string &name = "TextNode");

        /**
         * @brief Constructeur avec texte
         * @param font Police à utiliser
         * @param text Texte à afficher
         * @param characterSize Taille des caractères
         * @param name Nom du node
         */
        TextNode(const sf::Font &font, const std::string &text, unsigned int characterSize = 30, const std::string &name = "TextNode");

        /**
         * @brief Définit le texte
         * @param text Nouveau texte
         */
        void setText(const std::string &text);

        /**
         * @brief Définit la police
         * @param font Nouvelle police
         */
        void setFont(const sf::Font &font);

        /**
         * @brief Définit la taille des caractères
         * @param size Nouvelle taille
         */
        void setCharacterSize(unsigned int size);

        /**
         * @brief Définit la couleur de remplissage
         * @param color Nouvelle couleur
         */
        void setFillColor(const sf::Color &color);

        /**
         * @brief Définit la couleur du contour
         * @param color Nouvelle couleur
         */
        void setOutlineColor(const sf::Color &color);

        /**
         * @brief Définit l'épaisseur du contour
         * @param thickness Nouvelle épaisseur
         */
        void setOutlineThickness(float thickness);

        /**
         * @brief Définit le style du texte
         * @param style Nouveau style
         */
        void setStyle(sf::Uint32 style);

        /**
         * @brief Récupère l'objet texte
         * @return Référence vers l'objet texte
         */
        sf::Text &getText();

        /**
         * @brief Récupère l'objet texte (const)
         * @return Référence const vers l'objet texte
         */
        const sf::Text &getText() const;

        /**
         * @brief Centre l'origine du texte
         */
        void centerOrigin();

    protected:
        /**
         * @brief Dessine le texte
         * @param target Cible de rendu
         * @param states États de rendu
         */
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Text m_text; ///< Objet texte SFML
    };

} // namespace Orenji