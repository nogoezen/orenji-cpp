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
        std::sort(entities.begin(), entities.end(), [this](Core::Entity *a, Core::Entity *b)
                  {
            auto* spriteA = a->getComponent<Components::SpriteComponent>();
            auto* spriteB = b->getComponent<Components::SpriteComponent>();
            return spriteA->getLayer() < spriteB->getLayer(); });

        // View culling - Get current view
        sf::View view = m_window.getView();
        sf::FloatRect viewBounds(
            sf::Vector2f(view.getCenter().x - view.getSize().x / 2.f,
                         view.getCenter().y - view.getSize().y / 2.f),
            sf::Vector2f(view.getSize().x, view.getSize().y));

        // Batching - Group by texture
        std::map<const sf::Texture *, std::vector<Components::SpriteComponent *>> batchMap;

        // Collect all visible sprites within view
        for (const auto &entity : entities)
        {
            auto *spriteComponent = entity->getComponent<Components::SpriteComponent>();
            if (!spriteComponent)
            {
                continue;
            }

            // Skip invisible sprites
            if (!spriteComponent->isVisible())
            {
                continue;
            }

            // Get sprite global bounds for culling
            sf::FloatRect spriteBounds = spriteComponent->getSprite().getGlobalBounds();

            // View culling - Skip sprites outside view
            // SFML 3: check if any corner of the sprite bounds is within the view
            if (!viewBounds.contains(spriteBounds.position) &&
                !viewBounds.contains(sf::Vector2f(spriteBounds.position.x + spriteBounds.size.x, spriteBounds.position.y)) &&
                !viewBounds.contains(sf::Vector2f(spriteBounds.position.x, spriteBounds.position.y + spriteBounds.size.y)) &&
                !viewBounds.contains(sf::Vector2f(spriteBounds.position.x + spriteBounds.size.x, spriteBounds.position.y + spriteBounds.size.y)))
            {
                continue;
            }

            // SFML 3: getTexture() renvoie une référence et non un pointeur
            // On l'ajoute directement au batchMap en utilisant son adresse comme clé
            const sf::Texture &texture = spriteComponent->getSprite().getTexture();
            batchMap[&texture].push_back(spriteComponent);
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