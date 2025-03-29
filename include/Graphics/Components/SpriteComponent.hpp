#pragma once

#include <SFML/Graphics.hpp>
#include "../../Core/Component.hpp"
#include <string>
#include <memory>

namespace Graphics
{
    namespace Components
    {

        /**
         * @brief Component for sprite rendering
         */
        class SpriteComponent : public Core::Component
        {
        public:
            /**
             * @brief Constructor
             */
            SpriteComponent();

            /**
             * @brief Constructor with texture initialization
             * @param texture Shared pointer to the texture
             */
            SpriteComponent(std::shared_ptr<sf::Texture> texture);

            /**
             * @brief Destructor
             */
            virtual ~SpriteComponent() = default;

            /**
             * @brief Set the texture
             * @param texture Shared pointer to the texture
             */
            void setTexture(std::shared_ptr<sf::Texture> texture);

            /**
             * @brief Get the sprite
             * @return Reference to the sprite
             */
            sf::Sprite &getSprite();

            /**
             * @brief Set the texture rectangle for sprite animation
             * @param rect Rectangle in the texture
             */
            void setTextureRect(const sf::IntRect &rect);

            /**
             * @brief Set the sprite's origin
             * @param x X coordinate of origin
             * @param y Y coordinate of origin
             */
            void setOrigin(float x, float y);

            /**
             * @brief Set the sprite's color
             * @param color Color to apply
             */
            void setColor(const sf::Color &color);

            /**
             * @brief Set render layer (higher values render on top)
             * @param layer Layer value
             */
            void setLayer(int layer);

            /**
             * @brief Get render layer
             * @return Layer value
             */
            int getLayer() const;

            /**
             * @brief Set visibility
             * @param visible Whether the sprite should be visible
             */
            void setVisible(bool visible);

            /**
             * @brief Check if visible
             * @return True if visible
             */
            bool isVisible() const;

        private:
            sf::Sprite m_sprite;
            int m_layer;
            bool m_visible;
        };

    }
} // namespace Graphics::Components