#include "../../include/Resources/TiledMapLoader.hpp"
#include "../../include/Resources/ResourceManager.hpp"
#include <iostream>

namespace Resources
{

    TiledMapLoader::TiledMapLoader(ResourceManager &resourceManager)
        : m_resourceManager(resourceManager)
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

        // Dans une vraie implémentation, nous utiliserions une bibliothèque comme
        // TMXLite pour charger et analyser les fichiers TMX

        // Exemple simplifié :
        try
        {
            // 1. Charger le fichier TMX
            // 2. Analyser les calques, tuiles, objets, etc.
            // 3. Créer les sprites/entités correspondants

            std::cout << "Map loaded successfully" << std::endl;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to load map: " << e.what() << std::endl;
            return false;
        }
    }

} // namespace Resources