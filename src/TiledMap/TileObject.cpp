#include "../../include/TiledMap/TileObject.hpp"
#include "../../include/Core/Entity.hpp"

namespace Orenji
{

    TileObject::TileObject()
        : m_objectId(-1), m_map(nullptr)
    {
    }

    TileObject::TileObject(int objectId)
        : m_objectId(objectId), m_map(nullptr)
    {
    }

    TileObject::TileObject(const std::string &objectName)
        : m_map(nullptr), m_objectId(-1), m_objectName(objectName), m_object(nullptr)
    {
    }

    void TileObject::initialize()
    {
        // Rien à initialiser spécifiquement
    }

    void TileObject::update(float deltaTime)
    {
        // Rien à mettre à jour automatiquement
    }

    void TileObject::setMap(TiledMap *map)
    {
        m_map = map;
    }

    TiledMap *TileObject::getMap() const
    {
        return m_map;
    }

    void TileObject::setObjectId(int objectId)
    {
        m_objectId = objectId;
    }

    int TileObject::getObjectId() const
    {
        return m_objectId;
    }

    void TileObject::setObjectName(const std::string &objectName)
    {
        m_objectName = objectName;
        m_objectId = -1; // Réinitialiser l'ID car nous utilisons le nom

        // Réinitialiser l'objet
        m_object = nullptr;

        // Si nous avons une carte, essayer de trouver l'objet
        if (m_map)
        {
            m_object = m_map->getObjectByName(m_objectName);

            // Si l'objet est trouvé et que nous avons une entité, appliquer sa position
            if (m_object && hasEntity())
            {
                applyToEntity();
            }
        }
    }

    Object *TileObject::getObject() const
    {
        if (!m_map || m_objectId < 0)
        {
            return nullptr;
        }

        return m_map->getObjectById(m_objectId);
    }

    const std::string &TileObject::getObjectName() const
    {
        if (m_object)
        {
            return m_object->name;
        }
        return m_objectName;
    }

    std::string TileObject::getObjectType() const
    {
        if (m_object)
        {
            return m_object->type;
        }
        return "";
    }

    bool TileObject::isVisible() const
    {
        if (m_object)
        {
            return m_object->visible;
        }
        return false;
    }

    void TileObject::setVisible(bool visible)
    {
        if (m_object)
        {
            m_object->visible = visible;
        }
    }

    bool TileObject::hasProperty(const std::string &name) const
    {
        if (Object *obj = getObject())
        {
            return obj->properties.contains(name);
        }
        return false;
    }

    std::string TileObject::getStringProperty(const std::string &name, const std::string &defaultValue) const
    {
        if (Object *obj = getObject())
        {
            if (obj->properties.contains(name))
            {
                const auto &prop = obj->properties[name];
                if (prop.is_string())
                {
                    return prop.get<std::string>();
                }
            }
        }
        return defaultValue;
    }

    int TileObject::getIntProperty(const std::string &name, int defaultValue) const
    {
        if (Object *obj = getObject())
        {
            if (obj->properties.contains(name))
            {
                const auto &prop = obj->properties[name];
                if (prop.is_number_integer())
                {
                    return prop.get<int>();
                }
                else if (prop.is_number_float())
                {
                    return static_cast<int>(prop.get<float>());
                }
                else if (prop.is_string())
                {
                    try
                    {
                        return std::stoi(prop.get<std::string>());
                    }
                    catch (...)
                    {
                        // Conversion échouée, retourner la valeur par défaut
                    }
                }
            }
        }
        return defaultValue;
    }

    float TileObject::getFloatProperty(const std::string &name, float defaultValue) const
    {
        if (Object *obj = getObject())
        {
            if (obj->properties.contains(name))
            {
                const auto &prop = obj->properties[name];
                if (prop.is_number_float())
                {
                    return prop.get<float>();
                }
                else if (prop.is_number_integer())
                {
                    return static_cast<float>(prop.get<int>());
                }
                else if (prop.is_string())
                {
                    try
                    {
                        return std::stof(prop.get<std::string>());
                    }
                    catch (...)
                    {
                        // Conversion échouée, retourner la valeur par défaut
                    }
                }
            }
        }
        return defaultValue;
    }

    bool TileObject::getBoolProperty(const std::string &name, bool defaultValue) const
    {
        if (Object *obj = getObject())
        {
            if (obj->properties.contains(name))
            {
                const auto &prop = obj->properties[name];
                if (prop.is_boolean())
                {
                    return prop.get<bool>();
                }
                else if (prop.is_number_integer())
                {
                    return prop.get<int>() != 0;
                }
                else if (prop.is_string())
                {
                    const auto &str = prop.get<std::string>();
                    return str == "true" || str == "1" || str == "yes";
                }
            }
        }
        return defaultValue;
    }

    sf::FloatRect TileObject::getBounds() const
    {
        if (m_object)
        {
            return m_object->bounds;
        }
        return sf::FloatRect();
    }

    sf::Vector2f TileObject::getPosition() const
    {
        if (m_object)
        {
            return m_object->position;
        }
        return sf::Vector2f();
    }

    void TileObject::setPosition(const sf::Vector2f &position)
    {
        if (m_object)
        {
            float diffX = position.x - m_object->position.x;
            float diffY = position.y - m_object->position.y;

            // Mettre à jour la position
            m_object->position = position;

            // Mettre à jour le rectangle englobant
            m_object->bounds.left += diffX;
            m_object->bounds.top += diffY;

            // Mettre à jour la forme si elle existe
            if (m_object->shape)
            {
                m_object->shape->setPosition(position);
            }

            // Mettre à jour le corps physique si nécessaire
            if (m_object->body)
            {
                b2Vec2 physicsPos(position.x / PhysicsWorld::PIXELS_PER_METER,
                                  position.y / PhysicsWorld::PIXELS_PER_METER);
                m_object->body->SetTransform(physicsPos, m_object->body->GetAngle());
            }
        }
    }

    void TileObject::applyToEntity()
    {
        if (m_object && hasEntity())
        {
            // Appliquer la position de l'objet à l'entité
            getEntity()->setPosition(m_object->position);

            // Si l'objet a une propriété "rotation", l'appliquer aussi
            if (m_object->properties.contains("rotation"))
            {
                const auto &prop = m_object->properties["rotation"];
                if (prop.is_number_float())
                {
                    getEntity()->setRotation(prop.get<float>());
                }
                else if (prop.is_number_integer())
                {
                    getEntity()->setRotation(static_cast<float>(prop.get<int>()));
                }
            }
        }
    }

    void TileObject::applyFromEntity()
    {
        if (m_object && hasEntity())
        {
            // Appliquer la position de l'entité à l'objet
            setPosition(getEntity()->getPosition());

            // Enregistrer la rotation comme propriété
            m_object->properties["rotation"] = getEntity()->getRotation();
        }
    }

    void TileObject::syncPositionFromObject()
    {
        if (Object *obj = getObject())
        {
            if (Entity *entity = getEntity())
            {
                // Utiliser le centre de l'objet comme position de l'entité
                sf::Vector2f center(
                    obj->bounds.left + obj->bounds.width / 2.0f,
                    obj->bounds.top + obj->bounds.height / 2.0f);
                entity->setPosition(center);

                // Si l'objet a une rotation, l'appliquer à l'entité
                if (obj->properties.contains("rotation"))
                {
                    const auto &rotProp = obj->properties["rotation"];
                    if (rotProp.is_number())
                    {
                        entity->setRotation(rotProp.get<float>());
                    }
                }
            }
        }
    }

    void TileObject::syncPositionToObject()
    {
        if (Object *obj = getObject())
        {
            if (Entity *entity = getEntity())
            {
                // Calculer le nouveau rectangle englobant de l'objet
                const sf::Vector2f &entityPos = entity->getPosition();
                float width = obj->bounds.width;
                float height = obj->bounds.height;

                // Mettre à jour les coordonnées de l'objet pour que son centre corresponde à la position de l'entité
                obj->bounds.left = entityPos.x - width / 2.0f;
                obj->bounds.top = entityPos.y - height / 2.0f;

                // Si l'entité a une rotation, mettre à jour la propriété de rotation de l'objet
                obj->properties["rotation"] = entity->getRotation();
            }
        }
    }

} // namespace Orenji