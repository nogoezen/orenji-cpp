#include "TiledMap.h"
#include <iostream>
#include <fstream>

TiledMap::TiledMap(const std::string &filePath)
    : m_width(0), m_height(0), m_tileWidth(0), m_tileHeight(0)
{
    load(filePath);
    createVertices();
}

TiledMap::~TiledMap()
{
    // Pas besoin de nettoyer explicitement, les destructeurs des membres s'en chargeront
}

void TiledMap::load(const std::string &filePath)
{
    // Charger le fichier JSON
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier de carte: " << filePath << std::endl;
        return;
    }

    // Parser le JSON
    nlohmann::json mapData;
    try
    {
        file >> mapData;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur: Impossible de parser le JSON: " << e.what() << std::endl;
        return;
    }

    // Récupérer les propriétés de base de la carte
    m_width = mapData["width"];
    m_height = mapData["height"];
    m_tileWidth = mapData["tilewidth"];
    m_tileHeight = mapData["tileheight"];

    // Charger les tilesets
    for (const auto &tilesetJson : mapData["tilesets"])
    {
        Tileset tileset;
        tileset.firstgid = tilesetJson["firstgid"];
        tileset.name = tilesetJson["name"];

        // Gérer les tilesets embarqués ou externes
        if (tilesetJson.find("source") != tilesetJson.end())
        {
            // Tileset externe
            tileset.source = tilesetJson["source"];

            // Extraire le chemin du dossier du fichier de carte
            std::string folderPath = filePath.substr(0, filePath.find_last_of("/\\") + 1);
            std::string tilesetPath = folderPath + tileset.source;

            // Charger le fichier de tileset
            std::ifstream tilesetFile(tilesetPath);
            if (!tilesetFile.is_open())
            {
                std::cerr << "Erreur: Impossible d'ouvrir le fichier de tileset: " << tilesetPath << std::endl;
                continue;
            }

            nlohmann::json tilesetData;
            try
            {
                tilesetFile >> tilesetData;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Erreur: Impossible de parser le JSON du tileset: " << e.what() << std::endl;
                continue;
            }

            tileset.tileWidth = tilesetData["tilewidth"];
            tileset.tileHeight = tilesetData["tileheight"];
            tileset.tileCount = tilesetData["tilecount"];
            tileset.columns = tilesetData["columns"];

            // Charger l'image du tileset
            std::string imagePath = folderPath + tilesetData["image"].get<std::string>();
            if (!tileset.texture.loadFromFile(imagePath))
            {
                std::cerr << "Erreur: Impossible de charger l'image du tileset: " << imagePath << std::endl;
                createDefaultTileset(tileset);
            }
        }
        else
        {
            // Tileset embarqué
            tileset.tileWidth = tilesetJson["tilewidth"];
            tileset.tileHeight = tilesetJson["tileheight"];
            tileset.tileCount = tilesetJson["tilecount"];
            tileset.columns = tilesetJson["columns"];

            // Extraire le chemin du dossier du fichier de carte
            std::string folderPath = filePath.substr(0, filePath.find_last_of("/\\") + 1);
            std::string imagePath = folderPath + tilesetJson["image"].get<std::string>();

            if (!tileset.texture.loadFromFile(imagePath))
            {
                std::cerr << "Erreur: Impossible de charger l'image du tileset: " << imagePath << std::endl;
                std::cerr << "Création d'un tileset par défaut." << std::endl;
                createDefaultTileset(tileset);
            }
        }

        m_tilesets.push_back(tileset);
    }

    // Charger les couches
    for (const auto &layerJson : mapData["layers"])
    {
        // Ne traiter que les couches de type "tilelayer"
        if (layerJson["type"] != "tilelayer")
        {
            continue;
        }

        TileLayer layer;
        layer.name = layerJson["name"];
        layer.width = layerJson["width"];
        layer.height = layerJson["height"];
        layer.opacity = layerJson.contains("opacity") ? layerJson["opacity"].get<float>() : 1.0f;
        layer.visible = layerJson.contains("visible") ? layerJson["visible"].get<bool>() : true;

        // Récupérer les données de la couche (indices des tuiles)
        layer.data = layerJson["data"].get<std::vector<int>>();

        m_layers.push_back(layer);
    }
}

void TiledMap::createDefaultTileset(Tileset &tileset)
{
    // Créer une texture par défaut pour le tileset
    sf::Image image;
    image.create(tileset.columns * tileset.tileWidth, (tileset.tileCount / tileset.columns + 1) * tileset.tileHeight);

    // Remplir l'image avec des couleurs différentes pour chaque tuile
    sf::Color colors[] = {
        sf::Color(200, 200, 200), // Gris clair
        sf::Color(120, 180, 80),  // Vert
        sf::Color(180, 80, 80),   // Rouge
        sf::Color(80, 80, 180),   // Bleu
        sf::Color(180, 180, 80)   // Jaune
    };

    for (int i = 0; i < tileset.tileCount; ++i)
    {
        int x = (i % tileset.columns) * tileset.tileWidth;
        int y = (i / tileset.columns) * tileset.tileHeight;

        sf::Color tileColor = colors[i % 5];

        // Remplir la tuile avec la couleur
        for (int ty = 0; ty < tileset.tileHeight; ++ty)
        {
            for (int tx = 0; tx < tileset.tileWidth; ++tx)
            {
                image.setPixel(x + tx, y + ty, tileColor);
            }
        }

        // Ajouter une bordure
        sf::Color borderColor = sf::Color::Black;
        for (int tx = 0; tx < tileset.tileWidth; ++tx)
        {
            image.setPixel(x + tx, y, borderColor);
            image.setPixel(x + tx, y + tileset.tileHeight - 1, borderColor);
        }
        for (int ty = 0; ty < tileset.tileHeight; ++ty)
        {
            image.setPixel(x, y + ty, borderColor);
            image.setPixel(x + tileset.tileWidth - 1, y + ty, borderColor);
        }
    }

    // Charger la texture à partir de l'image
    tileset.texture.loadFromImage(image);
}

void TiledMap::createVertices()
{
    // Créer le tableau de vertices pour le rendu
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(m_width * m_height * 4 * m_layers.size());

    // Pour chaque couche
    for (size_t layerIndex = 0; layerIndex < m_layers.size(); ++layerIndex)
    {
        const TileLayer &layer = m_layers[layerIndex];

        // Pour chaque tuile dans la couche
        for (int y = 0; y < m_height; ++y)
        {
            for (int x = 0; x < m_width; ++x)
            {
                // Calculer l'indice de la tuile courante
                int tileIndex = x + y * m_width;

                // Récupérer l'indice global de la tuile
                int gid = layer.data[tileIndex];

                // Si l'indice est 0, il n'y a pas de tuile à cet emplacement
                if (gid == 0)
                {
                    continue;
                }

                // Trouver le tileset correspondant à cet indice
                Tileset *tileset = getTilesetForGid(gid);
                if (!tileset)
                {
                    continue;
                }

                // Convertir l'indice global en indice local pour le tileset
                int localId = gid - tileset->firstgid;

                // Calculer la position de la tuile dans la texture
                int tu = localId % tileset->columns;
                int tv = localId / tileset->columns;

                // Calculer l'indice du quad dans le tableau de vertices
                int quadIndex = (tileIndex + layerIndex * m_width * m_height) * 4;

                // Définir les coordonnées des 4 sommets du quad
                sf::Vertex *quad = &m_vertices[quadIndex];

                // Définir les positions des sommets (rectangle)
                quad[0].position = sf::Vector2f(x * m_tileWidth, y * m_tileHeight);
                quad[1].position = sf::Vector2f((x + 1) * m_tileWidth, y * m_tileHeight);
                quad[2].position = sf::Vector2f((x + 1) * m_tileWidth, (y + 1) * m_tileHeight);
                quad[3].position = sf::Vector2f(x * m_tileWidth, (y + 1) * m_tileHeight);

                // Définir les coordonnées de texture des sommets
                quad[0].texCoords = sf::Vector2f(tu * tileset->tileWidth, tv * tileset->tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, tv * tileset->tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, (tv + 1) * tileset->tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * tileset->tileWidth, (tv + 1) * tileset->tileHeight);

                // Appliquer l'opacité de la couche
                sf::Color color(255, 255, 255, static_cast<sf::Uint8>(layer.opacity * 255));
                quad[0].color = color;
                quad[1].color = color;
                quad[2].color = color;
                quad[3].color = color;
            }
        }
    }
}

Tileset *TiledMap::getTilesetForGid(int gid)
{
    // Trouver le bon tileset pour cet indice global
    for (int i = m_tilesets.size() - 1; i >= 0; --i)
    {
        if (m_tilesets[i].firstgid <= gid)
        {
            return &m_tilesets[i];
        }
    }

    return nullptr;
}

void TiledMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Appliquer la transformation
    states.transform *= getTransform();

    // Pour chaque couche
    for (size_t layerIndex = 0; layerIndex < m_layers.size(); ++layerIndex)
    {
        const TileLayer &layer = m_layers[layerIndex];

        // Ne pas dessiner les couches invisibles
        if (!layer.visible)
        {
            continue;
        }

        // Pour chaque tileset
        for (const auto &tileset : m_tilesets)
        {
            // Préparer les états de rendu avec la texture du tileset
            states.texture = &tileset.texture;

            // Calculer les indices de début et de fin pour ce tileset dans cette couche
            int firstTileIndex = layerIndex * m_width * m_height * 4;
            int lastTileIndex = (layerIndex + 1) * m_width * m_height * 4;

            // Dessiner seulement les tuiles de ce tileset
            target.draw(&m_vertices[firstTileIndex], lastTileIndex - firstTileIndex, sf::Quads, states);
        }
    }
}