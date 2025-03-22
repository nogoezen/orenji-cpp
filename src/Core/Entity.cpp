#include "../../include/Core/Entity.hpp"

namespace Orenji
{

    Entity::Entity(unsigned int id) : m_id(id)
    {
        // Initialize entity with a unique ID
    }

    void Entity::update(float deltaTime)
    {
        if (!m_active)
            return;

        // Update all components
        for (auto &[type, component] : m_components)
        {
            component->update(deltaTime);
        }
    }

    void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_active)
            return;

        // Apply entity's transform to the states
        states.transform *= getTransform();

        // Let components handle drawing if needed
        // This method will be extended by derived classes
    }

} // namespace Orenji