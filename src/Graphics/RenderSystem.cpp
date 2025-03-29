#include "../../include/Graphics/RenderSystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/Graphics/Components/SpriteComponent.hpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

namespace Graphics
{

    RenderSystem::RenderSystem(Core::EntityManager &entityManager, sf::RenderWindow &window)
        : Core::System(entityManager), m_window(window)
    {
        std::cout << "RenderSystem created" << std::endl;
    }

    RenderSystem::~RenderSystem()
    {
        std::cout << "RenderSystem destroyed" << std::endl;
    }

    void RenderSystem::init()
    {
        std::cout << "RenderSystem initialized" << std::endl;
    }

    void RenderSystem::update(float deltaTime)
    {
        // Empty - we don't need to update anything in the render system
        // Actual rendering happens in the render() method
    }

    void RenderSystem::render()
    {
        // Get all entities with sprite components
        auto entities = m_entityManager.getEntitiesWithComponent<Components::SpriteComponent>();

        // Optimization: Skip if no sprite entities
        if (entities.empty())
        {
            return;
        }

        // Sort entities by layer (higher layers drawn on top)
        std::sort(entities.begin(), entities.end(), [this](Core::EntityId a, Core::EntityId b)
                  {
            auto& spriteA = m_entityManager.getComponent<Components::SpriteComponent>(a);
            auto& spriteB = m_entityManager.getComponent<Components::SpriteComponent>(b);
            return spriteA.getLayer() < spriteB.getLayer(); });

        // View culling - Get current view
        sf::View view = m_window.getView();
        sf::FloatRect viewBounds(
            view.getCenter().x - view.getSize().x / 2.f,
            view.getCenter().y - view.getSize().y / 2.f,
            view.getSize().x,
            view.getSize().y);

        // Batching - Group by texture
        std::map<const sf::Texture *, std::vector<Components::SpriteComponent *>> batchMap;

        // Collect all visible sprites within view
        for (const auto &entityId : entities)
        {
            auto &spriteComponent = m_entityManager.getComponent<Components::SpriteComponent>(entityId);

            // Skip invisible sprites
            if (!spriteComponent.isVisible())
            {
                continue;
            }

            // Get sprite global bounds for culling
            sf::FloatRect spriteBounds = spriteComponent.getSprite().getGlobalBounds();

            // View culling - Skip sprites outside view
            if (!viewBounds.intersects(spriteBounds))
            {
                continue;
            }

            // Group by texture for batching
            const sf::Texture *texture = spriteComponent.getSprite().getTexture();
            if (texture)
            {
                batchMap[texture].push_back(&spriteComponent);
            }
        }

        // Render batches
        for (const auto &[texture, sprites] : batchMap)
        {
            for (const auto &sprite : sprites)
            {
                m_window.draw(sprite->getSprite());
            }
        }
    }

} // namespace Graphics