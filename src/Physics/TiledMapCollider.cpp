#include "../../include/Physics/TiledMapCollider.hpp"
#include "../../include/Resources/TiledMapLoader.hpp"
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

    int TiledMapCollider::createCollisionsFromMap(const Resources::TiledMapLoader &map,
                                                  CollisionCategory category,
                                                  CollisionCategory mask)
    {
        int objectCount = 0;

        // Parcourir toutes les couches d'objets
        for (const auto &layer : map.getObjectLayers())
        {
            // Parcourir tous les objets de la couche
            for (const auto &obj : layer.objects)
            {
                // Créer un corps de collision à partir de l'objet
                b2BodyId body = createCollisionBody(obj, category, mask);
                if (b2Body_IsValid(body))
                {
                    m_collisionBodies.push_back(body);
                    objectCount++;
                }
            }
        }

        return objectCount;
    }

    int TiledMapCollider::createCollisionsFromLayer(const Resources::TiledMapLoader &map, const std::string &layerName,
                                                    CollisionCategory category,
                                                    CollisionCategory mask)
    {
        int objectCount = 0;

        // Chercher la couche par son nom
        for (const auto &layer : map.getObjectLayers())
        {
            if (layer.name == layerName)
            {
                // Parcourir tous les objets de la couche
                for (const auto &obj : layer.objects)
                {
                    // Créer un corps de collision à partir de l'objet
                    b2BodyId body = createCollisionBody(obj, category, mask);
                    if (b2Body_IsValid(body))
                    {
                        m_collisionBodies.push_back(body);
                        objectCount++;
                    }
                }
                break;
            }
        }

        return objectCount;
    }

    int TiledMapCollider::createCollisionsFromType(const Resources::TiledMapLoader &map, const std::string &type,
                                                   CollisionCategory category,
                                                   CollisionCategory mask)
    {
        int objectCount = 0;

        // Parcourir toutes les couches d'objets
        for (const auto &layer : map.getObjectLayers())
        {
            // Parcourir tous les objets de la couche
            for (const auto &obj : layer.objects)
            {
                // Vérifier si l'objet est du type demandé
                if (obj.type == type)
                {
                    // Créer un corps de collision à partir de l'objet
                    b2BodyId body = createCollisionBody(obj, category, mask);
                    if (b2Body_IsValid(body))
                    {
                        m_collisionBodies.push_back(body);
                        objectCount++;
                    }
                }
            }
        }

        return objectCount;
    }

    void TiledMapCollider::clear()
    {
        // Supprimer tous les corps de collision
        for (auto bodyId : m_collisionBodies)
        {
            if (b2Body_IsValid(bodyId))
            {
                m_physics.destroyBody(bodyId);
            }
        }
        m_collisionBodies.clear();
    }

    size_t TiledMapCollider::getCollisionCount() const
    {
        return m_collisionBodies.size();
    }

    b2BodyId TiledMapCollider::createCollisionBody(const Resources::MapObject &obj,
                                                   CollisionCategory category,
                                                   CollisionCategory mask)
    {
        // Créer un corps statique
        b2BodyId bodyId = m_physics.createStaticBody(b2Vec2(obj.x, obj.y));

        if (!b2Body_IsValid(bodyId))
            return {0};

        // Ajouter une forme en fonction du type d'objet
        b2ShapeId shapeId = {0};
        if (obj.isRectangle())
        {
            // Pour un rectangle, on utilise une boîte
            shapeId = m_physics.addBoxFixture(bodyId, obj.width, obj.height);
        }
        else if (obj.isCircle())
        {
            // Pour un cercle, on utilise un cercle
            // Ici, nous supposons que le rayon est stocké dans la propriété "radius" de l'objet
            float radius = 0.0f;
            for (const auto &prop : obj.properties)
            {
                if (prop.name == "radius")
                {
                    radius = std::stof(prop.value);
                    break;
                }
            }
            if (radius <= 0.0f)
            {
                // Si pas de rayon spécifié, utiliser la moyenne de la largeur et de la hauteur
                radius = (obj.width + obj.height) * 0.25f;
            }
            shapeId = m_physics.addCircleFixture(bodyId, radius);
        }
        else if (obj.isPolygon())
        {
            // Pour un polygone, créer un corps composé de plusieurs boîtes
            // Ceci est une simplification; idéalement, nous utiliserions des polygones directement
            // mais Box2D nécessite des polygones convexes, ce qui complique la tâche
            std::cout << "Warning: Polygon objects are not fully supported yet. Using bounding box instead." << std::endl;
            shapeId = m_physics.addBoxFixture(bodyId, obj.width, obj.height);
        }
        else
        {
            // Type d'objet non supporté
            std::cout << "Warning: Unsupported object type. Using bounding box." << std::endl;
            shapeId = m_physics.addBoxFixture(bodyId, obj.width, obj.height);
        }

        // Définir la catégorie et le masque de collision
        if (b2Shape_IsValid(shapeId))
        {
            m_physics.setFilterData(shapeId, category, mask);

            // Vérifier si l'objet est un capteur (trigger)
            bool isSensor = false;
            for (const auto &prop : obj.properties)
            {
                if (prop.name == "isSensor" || prop.name == "sensor" || prop.name == "trigger")
                {
                    isSensor = (prop.value == "true" || prop.value == "1");
                    if (isSensor)
                    {
                        b2Shape_SetSensor(shapeId, true);
                        break;
                    }
                }
            }
        }

        return bodyId;
    }

} // namespace Physics