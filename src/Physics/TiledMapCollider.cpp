#include "../../include/Physics/TiledMapCollider.hpp"
#include <iostream>

namespace Physics
{
    TiledMapCollider::TiledMapCollider(Box2DWrapper &physics)
        : m_physics(physics)
    {
    }

    TiledMapCollider::~TiledMapCollider()
    {
        clear();
    }

    int TiledMapCollider::createCollisionsFromMap(const Resources::TiledMapLoader &map)
    {
        int count = 0;

        // Récupérer tous les objets marqués comme collidables
        auto objects = map.getCollidableObjects();

        for (const auto &obj : objects)
        {
            if (createCollisionBody(obj))
            {
                count++;
            }
        }

        std::cout << "Created " << count << " collision objects from map" << std::endl;
        return count;
    }

    int TiledMapCollider::createCollisionsFromLayer(const Resources::TiledMapLoader &map, const std::string &layerName)
    {
        int count = 0;

        // Récupérer tous les objets de la couche spécifiée
        auto objects = map.getObjectsInLayer(layerName);

        for (const auto &obj : objects)
        {
            if (createCollisionBody(obj))
            {
                count++;
            }
        }

        std::cout << "Created " << count << " collision objects from layer '" << layerName << "'" << std::endl;
        return count;
    }

    int TiledMapCollider::createCollisionsFromType(const Resources::TiledMapLoader &map, const std::string &type)
    {
        int count = 0;

        // Récupérer tous les objets du type spécifié
        auto objects = map.getObjectsByType(type);

        for (const auto &obj : objects)
        {
            if (createCollisionBody(obj))
            {
                count++;
            }
        }

        std::cout << "Created " << count << " collision objects of type '" << type << "'" << std::endl;
        return count;
    }

    void TiledMapCollider::clear()
    {
        // Détruire tous les corps de collision
        for (auto body : m_collisionBodies)
        {
            if (body)
            {
                m_physics.destroyBody(body);
            }
        }

        m_collisionBodies.clear();
    }

    size_t TiledMapCollider::getCollisionCount() const
    {
        return m_collisionBodies.size();
    }

    b2Body *TiledMapCollider::createCollisionBody(const Resources::MapObject &obj)
    {
        // Déterminer la forme de l'objet (pour l'instant, uniquement des boîtes rectangulaires)
        // Dans une implémentation plus complète, on pourrait gérer d'autres formes comme les cercles ou les polygones

        // Créer un corps statique à la position de l'objet
        b2Body *body = m_physics.createStaticBody(b2Vec2(obj.position.x + obj.size.x / 2, obj.position.y + obj.size.y / 2));

        if (!body)
        {
            std::cerr << "Failed to create collision body for object: " << obj.name << std::endl;
            return nullptr;
        }

        // Ajouter une fixture rectangulaire au corps
        m_physics.addBoxFixture(body, obj.size.x, obj.size.y);

        // Stocker le corps créé
        m_collisionBodies.push_back(body);

        return body;
    }

} // namespace Physics