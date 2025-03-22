#include "Core/Component.hpp"

namespace Orenji
{

    Component::Component(const std::string &name)
        : m_name(name), m_active(true), m_owner(nullptr)
    {
    }

    Component::~Component()
    {
    }

    bool Component::initialize()
    {
        return true;
    }

    void Component::update(float deltaTime)
    {
        // À surcharger dans les classes dérivées
    }

    void Component::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // À surcharger dans les classes dérivées
    }

} // namespace Orenji