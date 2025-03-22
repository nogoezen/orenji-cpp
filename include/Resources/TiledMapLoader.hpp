#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace Resources
{

    class ResourceManager;

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
         * @brief Load a TMX map file
         * @param filePath Path to the TMX file
         * @return true if the map was loaded successfully
         */
        bool loadMap(const std::string &filePath);

    private:
        ResourceManager &m_resourceManager;
    };

} // namespace Resources