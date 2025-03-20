#include "../../include/TiledMap/TiledMap.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/ImageLayer.hpp>
#include <tmxlite/Property.hpp>

namespace Orenji
{

    TiledMap::TiledMap()
        : m_loaded(false), m_width(0), m_height(0), m_tileWidth(0), m_tileHeight(0)
    {
    }

    TiledMap::~TiledMap()
    {
        clear();
    }

    bool TiledMap::loadFromFile(const std::string &filepath)
    {
        // Nettoyer les données existantes
        clear();

        // Charger le fichier TMX avec tmxlite
        tmx::Map tmxMap;
        if (!tmxMap.load(filepath))
        {
            std::cerr << "Erreur lors du chargement de la carte " << filepath << std::endl;
            return false;
        }

        // Récupérer les propriétés de base de la carte
        m_width = tmxMap.getTileCount().x;
        m_height = tmxMap.getTileCount().y;
        m_tileWidth = tmxMap.getTileSize().x;
        m_tileHeight = tmxMap.getTileSize().y;
        m_filepath = filepath;

        // Récupérer les propriétés personnalisées de la carte
        for (const auto &property : tmxMap.getProperties())
        {
            m_properties[property.getName()] = getPropertyValueAsString(property);
        }

        // Récupérer tous les tilesets
        const auto &tmxTilesets = tmxMap.getTilesets();
        for (const auto &tileset : tmxTilesets)
        {
            TilesetInfo tilesetInfo;
            tilesetInfo.firstGid = tileset.getFirstGID();
            tilesetInfo.tileCount = tileset.getTileCount();
            tilesetInfo.columns = tileset.getColumnCount();
            tilesetInfo.tileWidth = tileset.getTileSize().x;
            tilesetInfo.tileHeight = tileset.getTileSize().y;
            tilesetInfo.spacing = tileset.getSpacing();
            tilesetInfo.margin = tileset.getMargin();
            tilesetInfo.imagePath = tileset.getImagePath();
            tilesetInfo.name = tileset.getName();

            // Charger l'image du tileset
            if (!tilesetInfo.texture.loadFromFile(tileset.getImagePath()))
            {
                std::cerr << "Impossible de charger l'image du tileset: " << tileset.getImagePath() << std::endl;
                continue;
            }

            // Récupérer les propriétés des tuiles individuelles
            for (const auto &tile : tileset.getTiles())
            {
                TileInfo tileInfo;
                tileInfo.id = tile.ID + tileset.getFirstGID(); // ID global = local ID + first GID
                tileInfo.type = tile.type;

                // Récupérer les propriétés de la tuile
                for (const auto &property : tile.properties)
                {
                    tileInfo.properties[property.getName()] = getPropertyValueAsString(property);
                }

                // Ajouter la tuile à notre dictionnaire
                m_tileInfos[tileInfo.id] = tileInfo;
            }

            // Ajouter le tileset à notre liste
            m_tilesets.push_back(tilesetInfo);
        }

        // Charger chaque couche
        const auto &tmxLayers = tmxMap.getLayers();
        for (const auto &layer : tmxLayers)
        {
            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                loadTileLayer(layer, tmxMap);
            }
            else if (layer->getType() == tmx::Layer::Type::Object)
            {
                loadObjectLayer(layer);
            }
            else if (layer->getType() == tmx::Layer::Type::Image)
            {
                loadImageLayer(layer);
            }
        }

        m_loaded = true;
        return true;
    }

    void TiledMap::clear()
    {
        m_tileLayers.clear();
        m_objectLayers.clear();
        m_imageLayers.clear();
        m_tilesets.clear();
        m_tileInfos.clear();
        m_properties.clear();
        m_loaded = false;
    }

    bool TiledMap::isLoaded() const
    {
        return m_loaded;
    }

    const std::string &TiledMap::getFilePath() const
    {
        return m_filepath;
    }

    int TiledMap::getWidth() const
    {
        return m_width;
    }

    int TiledMap::getHeight() const
    {
        return m_height;
    }

    int TiledMap::getTileWidth() const
    {
        return m_tileWidth;
    }

    int TiledMap::getTileHeight() const
    {
        return m_tileHeight;
    }

    int TiledMap::getWidthPixels() const
    {
        return m_width * m_tileWidth;
    }

    int TiledMap::getHeightPixels() const
    {
        return m_height * m_tileHeight;
    }

    const std::vector<TiledMap::TileLayer> &TiledMap::getTileLayers() const
    {
        return m_tileLayers;
    }

    const std::vector<TiledMap::ObjectLayer> &TiledMap::getObjectLayers() const
    {
        return m_objectLayers;
    }

    const std::vector<TiledMap::ImageLayer> &TiledMap::getImageLayers() const
    {
        return m_imageLayers;
    }

    const std::vector<TiledMap::TilesetInfo> &TiledMap::getTilesets() const
    {
        return m_tilesets;
    }

    const TiledMap::TileInfo *TiledMap::getTileInfo(uint32_t gid) const
    {
        auto it = m_tileInfos.find(gid);
        if (it != m_tileInfos.end())
        {
            return &it->second;
        }
        return nullptr;
    }

    std::string TiledMap::getProperty(const std::string &name, const std::string &defaultValue) const
    {
        auto it = m_properties.find(name);
        if (it != m_properties.end())
        {
            return it->second;
        }
        return defaultValue;
    }

    int TiledMap::getPropertyInt(const std::string &name, int defaultValue) const
    {
        auto it = m_properties.find(name);
        if (it != m_properties.end())
        {
            try
            {
                return std::stoi(it->second);
            }
            catch (...)
            {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    float TiledMap::getPropertyFloat(const std::string &name, float defaultValue) const
    {
        auto it = m_properties.find(name);
        if (it != m_properties.end())
        {
            try
            {
                return std::stof(it->second);
            }
            catch (...)
            {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    bool TiledMap::getPropertyBool(const std::string &name, bool defaultValue) const
    {
        auto it = m_properties.find(name);
        if (it != m_properties.end())
        {
            std::string value = it->second;
            std::transform(value.begin(), value.end(), value.begin(),
                           [](unsigned char c)
                           { return std::tolower(c); });

            if (value == "true" || value == "1" || value == "yes")
                return true;
            else if (value == "false" || value == "0" || value == "no")
                return false;
        }
        return defaultValue;
    }

    void TiledMap::loadTileLayer(const tmx::Layer::Ptr &layer, const tmx::Map &map)
    {
        TileLayer tileLayer;
        tileLayer.name = layer->getName();
        tileLayer.opacity = layer->getOpacity();
        tileLayer.visible = layer->getVisible();
        tileLayer.offsetX = layer->getOffset().x;
        tileLayer.offsetY = layer->getOffset().y;

        // Récupérer les propriétés de la couche
        for (const auto &property : layer->getProperties())
        {
            tileLayer.properties[property.getName()] = getPropertyValueAsString(property);
        }

        // Récupérer les tuiles
        auto tileLayerPtr = layer->getLayerAs<tmx::TileLayer>();
        const auto &tiles = tileLayerPtr.getTiles();

        // Allouer l'espace pour les tuiles
        tileLayer.tiles.resize(m_width * m_height, 0);

        // Remplir le tableau de tuiles
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width; ++x)
            {
                int index = y * m_width + x;
                if (index < static_cast<int>(tiles.size()))
                {
                    tileLayer.tiles[index] = tiles[index].ID;
                }
            }
        }

        m_tileLayers.push_back(tileLayer);
    }

    void TiledMap::loadObjectLayer(const tmx::Layer::Ptr &layer)
    {
        ObjectLayer objectLayer;
        objectLayer.name = layer->getName();
        objectLayer.opacity = layer->getOpacity();
        objectLayer.visible = layer->getVisible();
        objectLayer.offsetX = layer->getOffset().x;
        objectLayer.offsetY = layer->getOffset().y;

        // Récupérer les propriétés de la couche
        for (const auto &property : layer->getProperties())
        {
            objectLayer.properties[property.getName()] = getPropertyValueAsString(property);
        }

        // Récupérer les objets
        auto objectGroupPtr = layer->getLayerAs<tmx::ObjectGroup>();
        const auto &objects = objectGroupPtr.getObjects();

        for (const auto &tmxObject : objects)
        {
            Object object;
            object.id = tmxObject.getUID();
            object.name = tmxObject.getName();
            object.type = tmxObject.getType();
            object.x = tmxObject.getPosition().x;
            object.y = tmxObject.getPosition().y;
            object.width = tmxObject.getAABB().width;
            object.height = tmxObject.getAABB().height;
            object.rotation = tmxObject.getRotation();
            object.visible = tmxObject.getVisible();
            object.gid = tmxObject.getTileID();

            // Récupérer les propriétés de l'objet
            for (const auto &property : tmxObject.getProperties())
            {
                object.properties[property.getName()] = getPropertyValueAsString(property);
            }

            // Récupérer les points pour les polygones/polylignes
            if (tmxObject.getShape() == tmx::Object::Shape::Polygon ||
                tmxObject.getShape() == tmx::Object::Shape::Polyline)
            {
                const auto &points = tmxObject.getPoints();
                for (const auto &point : points)
                {
                    object.points.push_back({point.x, point.y});
                }
            }

            // Déterminer le type d'objet en fonction de sa forme
            switch (tmxObject.getShape())
            {
            case tmx::Object::Shape::Rectangle:
                object.shape = Object::Shape::Rectangle;
                break;
            case tmx::Object::Shape::Ellipse:
                object.shape = Object::Shape::Ellipse;
                break;
            case tmx::Object::Shape::Polygon:
                object.shape = Object::Shape::Polygon;
                break;
            case tmx::Object::Shape::Polyline:
                object.shape = Object::Shape::Polyline;
                break;
            case tmx::Object::Shape::Text:
                object.shape = Object::Shape::Text;
                object.text = tmxObject.getText();
                break;
            default:
                object.shape = Object::Shape::Rectangle;
                break;
            }

            objectLayer.objects.push_back(object);
        }

        m_objectLayers.push_back(objectLayer);
    }

    void TiledMap::loadImageLayer(const tmx::Layer::Ptr &layer)
    {
        ImageLayer imageLayer;
        imageLayer.name = layer->getName();
        imageLayer.opacity = layer->getOpacity();
        imageLayer.visible = layer->getVisible();
        imageLayer.offsetX = layer->getOffset().x;
        imageLayer.offsetY = layer->getOffset().y;

        // Récupérer les propriétés de la couche
        for (const auto &property : layer->getProperties())
        {
            imageLayer.properties[property.getName()] = getPropertyValueAsString(property);
        }

        // Récupérer les informations de l'image
        auto imageLayerPtr = layer->getLayerAs<tmx::ImageLayer>();
        imageLayer.imagePath = imageLayerPtr.getImagePath();

        // Charger l'image
        if (!imageLayer.texture.loadFromFile(imageLayer.imagePath))
        {
            std::cerr << "Impossible de charger l'image de la couche: " << imageLayer.imagePath << std::endl;
        }

        m_imageLayers.push_back(imageLayer);
    }

    std::string TiledMap::getPropertyValueAsString(const tmx::Property &property)
    {
        switch (property.getType())
        {
        case tmx::Property::Type::Boolean:
            return property.getBoolValue() ? "true" : "false";
        case tmx::Property::Type::Float:
            return std::to_string(property.getFloatValue());
        case tmx::Property::Type::Int:
            return std::to_string(property.getIntValue());
        case tmx::Property::Type::String:
            return property.getStringValue();
        case tmx::Property::Type::Colour:
        {
            auto color = property.getColourValue();
            return std::to_string(color.r) + "," +
                   std::to_string(color.g) + "," +
                   std::to_string(color.b) + "," +
                   std::to_string(color.a);
        }
        default:
            return "";
        }
    }

    void TiledMap::update(float deltaTime)
    {
        // Mettre à jour la logique de la carte si nécessaire
        // Pour l'instant vide
    }

} // namespace Orenji