#pragma once

#include "Scene/SceneNode.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace Orenji
{
    /**
     * @class SpriteNode
     * @brief Node spécialisé pour l'affichage d'un sprite
     */
    class SpriteNode : public SceneNode
    {
    public:
        /**
         * @brief Constructeur par défaut
         * @param name Nom du node
         */
        explicit SpriteNode(const std::string &name = "SpriteNode");

        /**
         * @brief Constructeur avec texture
         * @param texture Texture à utiliser
         * @param name Nom du node
         */
        SpriteNode(const sf::Texture &texture, const std::string &name = "SpriteNode");

        /**
         * @brief Constructeur avec texture et rectangle
         * @param texture Texture à utiliser
         * @param textureRect Rectangle définissant la partie de la texture à utiliser
         * @param name Nom du node
         */
        SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect, const std::string &name = "SpriteNode");

        /**
         * @brief Définit la texture
         * @param texture Nouvelle texture
         */
        void setTexture(const sf::Texture &texture);

        /**
         * @brief Définit le rectangle de texture
         * @param rect Nouveau rectangle
         */
        void setTextureRect(const sf::IntRect &rect);

        /**
         * @brief Définit la couleur
         * @param color Nouvelle couleur
         */
        void setColor(const sf::Color &color);

        /**
         * @brief Récupère l'objet sprite
         * @return Référence vers l'objet sprite
         */
        sf::Sprite &getSprite();

        /**
         * @brief Récupère l'objet sprite (const)
         * @return Référence const vers l'objet sprite
         */
        const sf::Sprite &getSprite() const;

        /**
         * @brief Centre l'origine du sprite
         */
        void centerOrigin();

    protected:
        /**
         * @brief Dessine le sprite
         * @param target Cible de rendu
         * @param states États de rendu
         */
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::Sprite m_sprite; ///< Objet sprite SFML
    };

} // namespace Orenji