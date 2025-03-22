#include "../../include/Resources/TiledMapLoader.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include <iostream>
#include <algorithm>
#include "../../lib/tileson/tileson.hpp"

namespace Resources
{

    TiledMapLoader::TiledMapLoader(ResourceManager &resourceManager)
        : m_resourceManager(resourceManager), m_mapSize(0, 0), m_tileSize(0, 0)
    {
        std::cout << "TiledMapLoader created" << std::endl;
    }

    TiledMapLoader::~TiledMapLoader()
    {
        std::cout << "TiledMapLoader destroyed" << std::endl;
    }

    bool TiledMapLoader::loadMap(const std::string &filePath)
    {
        std::cout << "Loading map: " << filePath << std::endl;

        try
        {
            // Créer un loader Tileson
            tson::Tileson parser;

            // Charger la carte (Tileson supporte le format JSON de Tiled)
            m_map = parser.parse(filePath);

            // Vérifier si la carte a été chargée correctement
            if (!m_map || m_map->getStatus() != tson::ParseStatus::OK)
            {
                std::cerr << "Failed to parse map: " << filePath << std::endl;
                return false;
            }

            // Récupérer les dimensions de la carte
            m_tileSize.x = m_map->getTileSize().x;
            m_tileSize.y = m_map->getTileSize().y;
            m_mapSize.x = m_map->getSize().x;
            m_mapSize.y = m_map->getSize().y;

            // Charger tous les tilesets
            for (auto &tileset : m_map->getTilesets())
            {
                const std::string &texturePath = tileset.getImage().u8string();
                if (m_resourceManager.loadTexture(tileset.getName(), texturePath) == nullptr)
                {
                    std::cerr << "Failed to load tileset texture: " << texturePath << std::endl;
                    return false;
                }
            }

            // Analyser toutes les couches
            for (auto &layer : m_map->getLayers())
            {
                if (layer.getType() == tson::LayerType::TileLayer)
                {
                    parseTileLayer(&layer);
                }
                else if (layer.getType() == tson::LayerType::ObjectGroup)
                {
                    parseObjectLayer(&layer);
                }
                // Note: Les autres types de couches comme ImageLayer ou GroupLayer ne sont pas traités ici
            }

            std::cout << "Map loaded successfully: " << m_tileLayers.size()
                      << " tile layers, " << m_objectLayers.size() << " object layers" << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to load map: " << e.what() << std::endl;
            return false;
        }
    }

    void TiledMapLoader::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Dessiner toutes les couches de tuiles
        for (const auto &layer : m_tileLayers)
        {
            if (layer.visible)
            {
                sf::RenderStates layerStates = states;

                // Appliquer l'opacité et le décalage
                sf::Transform transform;
                transform.translate(layer.offset);
                layerStates.transform *= transform;

                for (const auto &sprite : layer.sprites)
                {
                    sf::Sprite s = sprite;
                    s.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(255 * layer.opacity)));
                    target.draw(s, layerStates);
                }
            }
        }
    }

    sf::Vector2f TiledMapLoader::getMapSize() const
    {
        return sf::Vector2f(m_mapSize.x * m_tileSize.x, m_mapSize.y * m_tileSize.y);
    }

    sf::Vector2i TiledMapLoader::getTileSize() const
    {
        return m_tileSize;
    }

    const TileLayer *TiledMapLoader::getLayer(const std::string &name) const
    {
        auto it = std::find_if(m_tileLayers.begin(), m_tileLayers.end(),
                               [&name](const TileLayer &layer)
                               { return layer.name == name; });

        if (it != m_tileLayers.end())
        {
            return &(*it);
        }

        return nullptr;
    }

    std::vector<MapObject> TiledMapLoader::getObjectsInLayer(const std::string &layerName) const
    {
        auto it = m_objectLayers.find(layerName);
        if (it != m_objectLayers.end())
        {
            return it->second;
        }

        return {};
    }

    std::vector<MapObject> TiledMapLoader::getObjectsByType(const std::string &type) const
    {
        std::vector<MapObject> result;

        for (const auto &[layerName, objects] : m_objectLayers)
        {
            for (const auto &obj : objects)
            {
                if (obj.type == type)
                {
                    result.push_back(obj);
                }
            }
        }

        return result;
    }

    std::vector<MapObject> TiledMapLoader::getObjectsByName(const std::string &name) const
    {
        std::vector<MapObject> result;

        for (const auto &[layerName, objects] : m_objectLayers)
        {
            for (const auto &obj : objects)
            {
                if (obj.name == name)
                {
                    result.push_back(obj);
                }
            }
        }

        return result;
    }

    std::vector<MapObject> TiledMapLoader::getCollidableObjects() const
    {
        std::vector<MapObject> result;

        for (const auto &[layerName, objects] : m_objectLayers)
        {
            for (const auto &obj : objects)
            {
                auto it = obj.properties.find("collidable");
                if (it != obj.properties.end() && it->second == "true")
                {
                    result.push_back(obj);
                }
            }
        }

        return result;
    }

    void TiledMapLoader::parseTileLayer(const tson::Layer *layer)
    {
        if (!layer || !m_map)
            return;

        TileLayer tileLayer;
        tileLayer.name = layer->getName();
        tileLayer.visible = layer->isVisible();
        tileLayer.opacity = layer->getOpacity();
        tileLayer.offset = sf::Vector2f(layer->getOffset().x, layer->getOffset().y);

        // Facteurs de parallax (si disponibles)
        const auto &parallax = layer->getParallax();
        tileLayer.parallaxFactor = sf::Vector2f(parallax.x, parallax.y);

        // Parcourir toutes les tuiles de la couche
        int mapWidth = m_mapSize.x;

        if (layer->getType() == tson::LayerType::TileLayer)
        {
            auto &tiles = layer->getTileObjects();

            // Traiter chaque tuile
            for (int y = 0; y < m_mapSize.y; ++y)
            {
                for (int x = 0; x < mapWidth; ++x)
                {
                    // Trouver l'objet tuile à cette position
                    tson::TileObject *tileObj = nullptr;

                    // Rechercher la tuile à cette position spécifique
                    for (auto &tile : tiles)
                    {
                        if (tile.getPositionInTileUnits().x == x && tile.getPositionInTileUnits().y == y)
                        {
                            tileObj = &tile;
                            break;
                        }
                    }

                    // Si pas de tuile ou tuile ID 0, continuer
                    if (!tileObj || tileObj->getTile() == nullptr || tileObj->getTile()->getId() == 0)
                        continue;

                    // Récupérer les informations de la tuile
                    auto tileId = tileObj->getTile()->getId();

                    // Trouver le tileset pour cette tuile
                    tson::Tileset *tileset = nullptr;
                    for (auto &ts : m_map->getTilesets())
                    {
                        if (tileId >= ts.getFirstgid() && tileId < ts.getFirstgid() + ts.getTileCount())
                        {
                            tileset = &ts;
                            break;
                        }
                    }

                    if (!tileset)
                        continue;

                    // Créer et ajouter le sprite
                    sf::Sprite sprite = createSprite(tileset, tileObj->getTile(), x, y);
                    tileLayer.sprites.push_back(sprite);
                }
            }
        }

        m_tileLayers.push_back(tileLayer);
    }

    void TiledMapLoader::parseObjectLayer(const tson::Layer *layer)
    {
        if (!layer || !m_map)
            return;

        std::vector<MapObject> objects;

        // Parcourir tous les objets de la couche - utiliser une copie constante
        const auto &objectsInLayer = layer->getObjects();
        for (const auto &obj : objectsInLayer)
        {
            objects.push_back(createObject(&obj));
        }

        m_objectLayers[layer->getName()] = objects;
    }

    MapObject TiledMapLoader::createObject(const tson::Object *obj)
    {
        MapObject mapObj;

        mapObj.position = sf::Vector2f(obj->getPosition().x, obj->getPosition().y);
        mapObj.size = sf::Vector2f(obj->getSize().x, obj->getSize().y);
        mapObj.bounds = sf::FloatRect(mapObj.position, mapObj.size);
        mapObj.name = obj->getName();
        mapObj.type = obj->getType();

        // Ajouter les propriétés personnalisées - utiliser une copie constante
        const auto &properties = obj->getProperties().getProperties();
        for (const auto &[key, prop] : properties)
        {
            std::string value;

            // Convertir la valeur en string selon son type
            if (prop.getType() == tson::Type::Boolean)
                value = prop.getValue<bool>() ? "true" : "false";
            else if (prop.getType() == tson::Type::Float)
                value = std::to_string(prop.getValue<float>());
            else if (prop.getType() == tson::Type::Int)
                value = std::to_string(prop.getValue<int>());
            else if (prop.getType() == tson::Type::String)
                value = prop.getValue<std::string>();

            mapObj.properties[key] = value;
        }

        return mapObj;
    }

    sf::Sprite TiledMapLoader::createSprite(const tson::Tileset *tileset, const tson::Tile *tile, int x, int y)
    {
        // Créer un sprite avec une texture vide
        sf::Texture emptyTexture;
        sf::Sprite sprite(emptyTexture);

        // Récupérer la texture depuis le ResourceManager
        sf::Texture *texture = m_resourceManager.getTexture(tileset->getName());
        if (texture == nullptr)
        {
            std::cerr << "Texture not found for tileset: " << tileset->getName() << std::endl;
            return sprite;
        }

        // Appliquer la texture au sprite
        sprite.setTexture(*texture);

        // Configurer le rectangle de texture pour la tuile
        sf::IntRect textureRect;
        const auto &drawingRect = tile->getDrawingRect();
        textureRect.position = {drawingRect.x, drawingRect.y};
        textureRect.size = {drawingRect.width, drawingRect.height};
        sprite.setTextureRect(textureRect);

        // Positionner le sprite sur la carte
        sprite.setPosition(sf::Vector2f(x * m_tileSize.x, y * m_tileSize.y));

        return sprite;
    }

} // namespace Resources