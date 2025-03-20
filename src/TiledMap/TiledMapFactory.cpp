#include "../../include/TiledMap/TiledMapFactory.hpp"
#include "../../include/TiledMap/TileObject.hpp"

namespace Orenji
{

    TiledMapFactory::TiledMapFactory()
    {
        // Constructeur vide
    }

    void TiledMapFactory::registerType(const std::string &objectType, EntityCreationFunction creationFunction)
    {
        m_typeCreators[objectType] = creationFunction;
    }

    void TiledMapFactory::registerNamePrefix(const std::string &namePrefix, EntityCreationFunction creationFunction)
    {
        m_namePrefixCreators[namePrefix] = creationFunction;
    }

    void TiledMapFactory::registerProperty(const std::string &propertyName, const std::string &propertyValue, EntityCreationFunction creationFunction)
    {
        m_propertyCreators[propertyName][propertyValue] = creationFunction;
    }

    int TiledMapFactory::createEntities(TiledMap &map, EntityManager &manager)
    {
        int count = 0;

        // Parcourir tous les objets de la carte
        for (const auto &object : map.getObjects())
        {
            Entity *entity = createEntity(object, manager);
            if (entity)
            {
                // Ajouter un composant TileObject à l'entité pour faciliter la référence à l'objet Tiled
                TileObject *tileObject = new TileObject(object.id);
                tileObject->setMap(&map);
                entity->addComponent(tileObject);

                count++;
            }
        }

        return count;
    }

    Entity *TiledMapFactory::createEntity(const Object &object, EntityManager &manager)
    {
        // Vérifier d'abord les créateurs par type
        if (!object.type.empty())
        {
            auto typeIt = m_typeCreators.find(object.type);
            if (typeIt != m_typeCreators.end())
            {
                return typeIt->second(object, manager);
            }
        }

        // Ensuite, vérifier les créateurs par préfixe de nom
        if (!object.name.empty())
        {
            for (const auto &[prefix, creator] : m_namePrefixCreators)
            {
                if (object.name.find(prefix) == 0)
                {
                    return creator(object, manager);
                }
            }
        }

        // Enfin, vérifier les créateurs par propriété
        for (const auto &[propName, valueMap] : m_propertyCreators)
        {
            // Vérifier si l'objet a cette propriété
            if (object.properties.contains(propName))
            {
                const auto &prop = object.properties[propName];
                std::string propValue;

                // Convertir la propriété en chaîne pour la comparaison
                if (prop.is_string())
                {
                    propValue = prop.get<std::string>();
                }
                else if (prop.is_boolean())
                {
                    propValue = prop.get<bool>() ? "true" : "false";
                }
                else if (prop.is_number_integer())
                {
                    propValue = std::to_string(prop.get<int>());
                }
                else if (prop.is_number_float())
                {
                    propValue = std::to_string(prop.get<float>());
                }

                // Chercher un créateur pour cette valeur de propriété
                auto valueIt = valueMap.find(propValue);
                if (valueIt != valueMap.end())
                {
                    return valueIt->second(object, manager);
                }
            }
        }

        // Aucun créateur trouvé pour cet objet
        return nullptr;
    }

} // namespace Orenji