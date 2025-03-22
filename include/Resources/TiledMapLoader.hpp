#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>

// Forward déclarations
namespace tson
{
    class Map;
    class Layer;
    class Tileset;
    class Object;
    class Tile;
}

namespace Resources
{

    class ResourceManager;

    /**
     * @brief Structure pour représenter une couche de tuiles (layer)
     */
    struct TileLayer
    {
        std::vector<sf::Sprite> sprites;
        std::string name;
        bool visible = true;
        float opacity = 1.0f;
        sf::Vector2f offset = {0.0f, 0.0f};
        sf::Vector2f parallaxFactor = {1.0f, 1.0f};
    };

    /**
     * @brief Structure pour représenter un objet
     */
    struct MapObject
    {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::FloatRect bounds;
        std::string name;
        std::string type;
        std::unordered_map<std::string, std::string> properties;
    };

    /**
     * @brief Class for loading and parsing Tiled TMX map files
     */
    class TiledMapLoader
    {
    public:
        /**
         * @brief Constructor
         * @param resourceManager Reference to resource manager
         */
        TiledMapLoader(ResourceManager &resourceManager);

        /**
         * @brief Destructor
         */
        ~TiledMapLoader();

        /**
         * @brief Load a TMX map file (JSON format)
         * @param filePath Path to the TMX file
         * @return true if the map was loaded successfully
         */
        bool loadMap(const std::string &filePath);

        /**
         * @brief Draw the map
         * @param target Target to draw to
         * @param states Render states
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;

        /**
         * @brief Get the size of the map in pixels
         * @return Size of the map in pixels
         */
        sf::Vector2f getMapSize() const;

        /**
         * @brief Get the tile size in pixels
         * @return Size of a tile in pixels
         */
        sf::Vector2i getTileSize() const;

        /**
         * @brief Get the layer with the specified name
         * @param name Name of the layer
         * @return Pointer to the layer or nullptr if not found
         */
        const TileLayer *getLayer(const std::string &name) const;

        /**
         * @brief Get all objects from a specific object layer
         * @param layerName Name of the object layer
         * @return Vector of objects in the layer
         */
        std::vector<MapObject> getObjectsInLayer(const std::string &layerName) const;

        /**
         * @brief Get all objects with a specific type
         * @param type Type of objects to get
         * @return Vector of objects with the specified type
         */
        std::vector<MapObject> getObjectsByType(const std::string &type) const;

        /**
         * @brief Get all objects with a specific name
         * @param name Name of objects to get
         * @return Vector of objects with the specified name
         */
        std::vector<MapObject> getObjectsByName(const std::string &name) const;

        /**
         * @brief Get all collidable objects (with "collidable" property set to true)
         * @return Vector of collidable objects
         */
        std::vector<MapObject> getCollidableObjects() const;

    private:
        ResourceManager &m_resourceManager;
        std::unique_ptr<tson::Map> m_map;

        // Couches de tuiles
        std::vector<TileLayer> m_tileLayers;

        // Objets
        std::unordered_map<std::string, std::vector<MapObject>> m_objectLayers;

        // Dimensions de la carte
        sf::Vector2i m_mapSize;
        sf::Vector2i m_tileSize;

        // Méthodes privées pour analyser les différentes parties de la carte
        void parseTileLayer(const tson::Layer *layer);
        void parseObjectLayer(const tson::Layer *layer);
        MapObject createObject(const tson::Object *obj);
        sf::Sprite createSprite(const tson::Tileset *tileset, const tson::Tile *tile, int x, int y);
    };

} // namespace Resources