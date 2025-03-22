#include "../../include/Graphics/RenderSystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include <iostream>

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
        // La mise à jour du système de rendu peut inclure des animations,
        // la mise à jour des sprites, etc.
    }

    void RenderSystem::render()
    {
        // Dans une vraie implémentation, nous itérerions à travers toutes les entités
        // avec des composants rendables et les dessinerions

        // Exemples :
        // for (auto entity : m_entityManager.findEntitiesWithComponents<SpriteComponent>()) {
        //     auto* spriteComponent = entity->getComponent<SpriteComponent>();
        //     m_window.draw(spriteComponent->getSprite());
        // }
    }

} // namespace Graphics