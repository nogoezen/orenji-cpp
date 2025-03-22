#include "Core/Entity.hpp"

namespace Orenji
{
    Entity::Entity(const std::string &id)
        : m_id(id), m_active(true)
    {
    }

    Entity::~Entity()
    {
        m_componentsById.clear();
        m_componentsByType.clear();
    }

    void Entity::update(float dt)
    {
        if (!m_active)
            return;

        // Mettre à jour tous les composants
        for (auto &pair : m_componentsById)
        {
            if (pair.second)
            {
                pair.second->update(dt);
            }
        }
    }

    void Entity::initialize()
    {
        // Initialiser tous les composants
        for (auto &pair : m_componentsById)
        {
            if (pair.second)
            {
                pair.second->initialize();
            }
        }
    }

    void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_active)
            return;

        // Combiner la transformation de l'entité avec les états passés
        states.transform *= getTransform();

        // Dessiner tous les composants
        for (const auto &pair : m_componentsById)
        {
            if (pair.second)
            {
                pair.second->draw(target);
            }
        }
    }

    Entity &Entity::addComponent(ComponentPtr component)
    {
        if (!component)
            return *this;

        // Ajouter le composant par ID
        m_componentsById[component->getId()] = component;

        // Ajouter le composant par type
        std::type_index typeIndex = std::type_index(typeid(*component));
        m_componentsByType[typeIndex].push_back(component);

        // Définir l'entité parente du composant
        component->setParentEntity(this);

        return *this;
    }

    ComponentPtr Entity::getComponent(const std::string &id)
    {
        auto it = m_componentsById.find(id);
        if (it != m_componentsById.end())
        {
            return it->second;
        }
        return nullptr;
    }

    bool Entity::removeComponent(const std::string &id)
    {
        auto it = m_componentsById.find(id);
        if (it != m_componentsById.end())
        {
            ComponentPtr component = it->second;

            // Supprimer le composant de la liste par type
            std::type_index typeIndex = std::type_index(typeid(*component));
            auto &typeComponents = m_componentsByType[typeIndex];

            // Rechercher et supprimer de la liste par type
            auto typeIt = std::find(typeComponents.begin(), typeComponents.end(), component);
            if (typeIt != typeComponents.end())
            {
                typeComponents.erase(typeIt);

                // Si la liste est vide, supprimer l'entrée
                if (typeComponents.empty())
                {
                    m_componentsByType.erase(typeIndex);
                }
            }

            // Supprimer le composant de la liste par ID
            m_componentsById.erase(it);

            return true;
        }

        return false;
    }

    // Adaptations pour SFML 3
    void Entity::setPosition(float x, float y)
    {
        sf::Transformable::setPosition(sf::Vector2f(x, y));
    }

    void Entity::setPosition(const sf::Vector2f &position)
    {
        sf::Transformable::setPosition(position);
    }

    void Entity::move(float offsetX, float offsetY)
    {
        sf::Transformable::move(sf::Vector2f(offsetX, offsetY));
    }

    void Entity::move(const sf::Vector2f &offset)
    {
        sf::Transformable::move(offset);
    }

    float Entity::getRotation() const
    {
        return sf::Transformable::getRotation().asDegrees();
    }

    void Entity::setRotation(float angle)
    {
        sf::Transformable::setRotation(sf::degrees(angle));
    }

    void Entity::rotate(float angle)
    {
        sf::Transformable::rotate(sf::degrees(angle));
    }

} // namespace Orenji