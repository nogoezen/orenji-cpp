#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Property.hpp"
#include "Layer.hpp"

namespace tmx
{
    /**
     * @brief Stub class for tmxlite's Map class
     * This is only for compilation purposes and should be replaced with the actual tmxlite library
     */
    class Map
    {
    public:
        bool load(const std::string &path) { return false; }

        struct TileCount
        {
            int x = 0;
            int y = 0;
        };
        TileCount getTileCount() const { return {}; }

        struct TileSize
        {
            int x = 0;
            int y = 0;
        };
        TileSize getTileSize() const { return {}; }

        const std::vector<Property> &getProperties() const
        {
            static std::vector<Property> props;
            return props;
        }

        const std::vector<Tileset> &getTilesets() const
        {
            static std::vector<Tileset> tilesets;
            return tilesets;
        }

        const std::vector<Layer::Ptr> &getLayers() const
        {
            static std::vector<Layer::Ptr> layers;
            return layers;
        }
    };
}