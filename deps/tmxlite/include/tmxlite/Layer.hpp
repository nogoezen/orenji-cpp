#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Property.hpp"

namespace tmx
{
    class TileLayer;
    class ObjectGroup;
    class ImageLayer;
    class Tileset;

    /**
     * @brief Stub class for tmxlite's Layer class
     * This is only for compilation purposes and should be replaced with the actual tmxlite library
     */
    class Layer
    {
    public:
        using Ptr = std::unique_ptr<Layer>;

        enum class Type
        {
            Tile,
            Object,
            Image
        };

        virtual ~Layer() = default;

        Type getType() const { return Type::Tile; }
        std::string getName() const { return ""; }
        float getOpacity() const { return 1.f; }
        bool getVisible() const { return true; }

        struct Offset
        {
            float x = 0;
            float y = 0;
        };
        Offset getOffset() const { return {}; }

        const std::vector<Property> &getProperties() const
        {
            static std::vector<Property> props;
            return props;
        }

        template <typename T>
        const T &getLayerAs() const
        {
            static T layer;
            return layer;
        }
    };

    /**
     * @brief Stub class for tmxlite's TileLayer class
     */
    class TileLayer
    {
    public:
        struct Tile
        {
            uint32_t ID = 0;
        };

        const std::vector<Tile> &getTiles() const
        {
            static std::vector<Tile> tiles;
            return tiles;
        }
    };

    /**
     * @brief Stub class for tmxlite's ObjectGroup class
     */
    class ObjectGroup
    {
    public:
        const std::vector<Object> &getObjects() const
        {
            static std::vector<Object> objects;
            return objects;
        }
    };

    /**
     * @brief Stub class for tmxlite's Object class
     */
    class Object
    {
    public:
        enum class Shape
        {
            Rectangle,
            Ellipse,
            Polygon,
            Polyline,
            Text
        };

        uint32_t getUID() const { return 0; }
        std::string getName() const { return ""; }
        std::string getType() const { return ""; }

        struct Position
        {
            float x = 0;
            float y = 0;
        };
        Position getPosition() const { return {}; }

        struct AABB
        {
            float left = 0;
            float top = 0;
            float width = 0;
            float height = 0;
        };
        AABB getAABB() const { return {}; }

        float getRotation() const { return 0.f; }
        bool getVisible() const { return true; }
        uint32_t getTileID() const { return 0; }

        Shape getShape() const { return Shape::Rectangle; }

        struct Point
        {
            float x = 0;
            float y = 0;
        };
        const std::vector<Point> &getPoints() const
        {
            static std::vector<Point> points;
            return points;
        }

        std::string getText() const { return ""; }

        const std::vector<Property> &getProperties() const
        {
            static std::vector<Property> props;
            return props;
        }
    };

    /**
     * @brief Stub class for tmxlite's ImageLayer class
     */
    class ImageLayer
    {
    public:
        std::string getImagePath() const { return ""; }
    };

    /**
     * @brief Stub class for tmxlite's Tileset class
     */
    class Tileset
    {
    public:
        uint32_t getFirstGID() const { return 0; }
        uint32_t getTileCount() const { return 0; }
        uint32_t getColumnCount() const { return 0; }

        struct TileSize
        {
            float x = 0;
            float y = 0;
        };
        TileSize getTileSize() const { return {}; }

        uint32_t getSpacing() const { return 0; }
        uint32_t getMargin() const { return 0; }
        std::string getName() const { return ""; }
        std::string getImagePath() const { return ""; }

        struct Tile
        {
            uint32_t ID = 0;
            std::string type;
            std::vector<Property> properties;
        };

        const std::vector<Tile> &getTiles() const
        {
            static std::vector<Tile> tiles;
            return tiles;
        }
    };
}