#pragma once

#include "../Core/System.hpp"
#include <SFML/Graphics.hpp>

namespace Graphics
{

    class RenderSystem : public Core::System
    {
    public:
        RenderSystem(Core::EntityManager &entityManager, sf::RenderWindow &window);
        virtual ~RenderSystem();

        virtual void init() override;
        virtual void update(float deltaTime) override;

        /**
         * @brief Render all drawable entities
         */
        void render();

    private:
        sf::RenderWindow &m_window;
    };

} // namespace Graphics