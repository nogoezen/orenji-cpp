#include "../../include/Core/RenderSystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include <iostream>

namespace Orenji
{

    RenderSystem::RenderSystem()
    {
        // Constructor
        setName("RenderSystem");
    }

    RenderSystem::~RenderSystem()
    {
        // Destructor
    }

    void RenderSystem::initialize()
    {
        std::cout << "RenderSystem initialized" << std::endl;
    }

    void RenderSystem::update(float deltaTime)
    {
        // This method is called every frame
        // We don't need to do rendering here, as it's handled in the render method
    }

    void RenderSystem::render(sf::RenderWindow &window)
    {
        // Get all entities with renderable components
        if (!m_entityManager)
        {
            return;
        }

        // Get all entities
        auto entities = m_entityManager->getAllEntities();

        // For each entity with renderable components, draw them
        // This is a placeholder - in a real implementation, we'd have a proper
        // component-based rendering system
        for (const auto &entity : entities)
        {
            (void)entity; // Évite l'avertissement de variable non utilisée

            // Check if the entity has a sprite or other drawable component
            // and render it if it does

            // Example (assuming we had a SpriteComponent):
            // auto spriteComponent = entity->getComponent<SpriteComponent>();
            // if (spriteComponent) {
            //     window.draw(spriteComponent->getSprite());
            // }
        }
    }

} // namespace Orenji