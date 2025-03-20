#pragma once
#include "System.hpp"
#include <SFML/Graphics.hpp>

namespace Orenji
{

    class RenderSystem : public System
    {
    public:
        RenderSystem();
        virtual ~RenderSystem();

        // System interface implementation
        virtual void initialize() override;
        virtual void update(float deltaTime) override;

        // Render all entities with renderable components
        void render(sf::RenderWindow &window);

    private:
        // Any render system specific data
    };

} // namespace Orenji