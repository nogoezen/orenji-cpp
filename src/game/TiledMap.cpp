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
    // Déterminer le type de fichier (TMX ou JSON)
    bool isTmxFile = filePath.substr(filePath.find_last_of(".") + 1) == "tmx";

    if (isTmxFile)
    {
        loadTMX(filePath);
    }
    else
    {
        loadJSON(filePath);
    }
}

void TiledMap::loadJSON(const std::string &filePath)
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

void TiledMap::loadTMX(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier TMX: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Extraire les informations de base de la carte
    std::string widthStr = extractAttribute(content, "width=\"", "\"");
    std::string heightStr = extractAttribute(content, "height=\"", "\"");
    std::string tileWidthStr = extractAttribute(content, "tilewidth=\"", "\"");
    std::string tileHeightStr = extractAttribute(content, "tileheight=\"", "\"");

    m_width = std::stoi(widthStr);
    m_height = std::stoi(heightStr);
    m_tileWidth = std::stoi(tileWidthStr);
    m_tileHeight = std::stoi(tileHeightStr);

    std::cout << "Dimensions de la carte: " << m_width << "x" << m_height
              << " tuiles de " << m_tileWidth << "x" << m_tileHeight << " pixels" << std::endl;

    // Extraire les informations des tilesets
    size_t tilesetPos = 0;
    while ((tilesetPos = content.find("<tileset ", tilesetPos)) != std::string::npos)
    {
        size_t endPos = content.find(">", tilesetPos);
        std::string tilesetTag = content.substr(tilesetPos, endPos - tilesetPos);

        Tileset tileset;
        std::string firstgidStr = extractAttribute(tilesetTag, "firstgid=\"", "\"");
        tileset.firstgid = std::stoi(firstgidStr);

        std::string nameStr = extractAttribute(tilesetTag, "name=\"", "\"");
        tileset.name = nameStr;

        std::string sourceStr = extractAttribute(tilesetTag, "source=\"", "\"");
        if (!sourceStr.empty())
        {
            // Tileset externe
            tileset.source = sourceStr;

            // Extraire le chemin du dossier du fichier de carte
            std::string folderPath = filePath.substr(0, filePath.find_last_of("/\\") + 1);
            std::string tilesetPath = folderPath + tileset.source;

            std::cout << "Chargement du tileset externe: " << tilesetPath << std::endl;

            // Ouvrir le fichier TSX
            std::ifstream tsxFile(tilesetPath);
            if (!tsxFile.is_open())
            {
                std::cerr << "Erreur: Impossible d'ouvrir le fichier de tileset: " << tilesetPath << std::endl;
                tilesetPos = endPos;
                continue;
            }

            std::string tsxContent((std::istreambuf_iterator<char>(tsxFile)), std::istreambuf_iterator<char>());

            // Extraire les propriétés du tileset
            std::string tileWidthStr = extractAttribute(tsxContent, "tilewidth=\"", "\"");
            std::string tileHeightStr = extractAttribute(tsxContent, "tileheight=\"", "\"");
            std::string tileCountStr = extractAttribute(tsxContent, "tilecount=\"", "\"");
            std::string columnsStr = extractAttribute(tsxContent, "columns=\"", "\"");

            tileset.tileWidth = std::stoi(tileWidthStr);
            tileset.tileHeight = std::stoi(tileHeightStr);
            tileset.tileCount = std::stoi(tileCountStr);
            tileset.columns = std::stoi(columnsStr);

            std::cout << "Propriétés du tileset: " << tileset.tileWidth << "x" << tileset.tileHeight
                      << ", " << tileset.tileCount << " tuiles, " << tileset.columns << " colonnes" << std::endl;

            // Extraire le chemin de l'image
            std::string imageTag = extractTag(tsxContent, "<image ", "/>");
            std::string imagePath = extractAttribute(imageTag, "source=\"", "\"");

            // Charger l'image du tileset
            std::string fullImagePath = folderPath + imagePath;
            std::cout << "Chargement de l'image du tileset: " << fullImagePath << std::endl;

            if (!tileset.texture.loadFromFile(fullImagePath))
            {
                std::cerr << "Erreur: Impossible de charger l'image du tileset: " << fullImagePath << std::endl;
                createDefaultTileset(tileset);
            }
            else
            {
                std::cout << "Image du tileset chargée avec succès" << std::endl;
            }
        }
        else
        {
            // Tileset embarqué (non supporté pour l'instant)
            std::cerr << "Erreur: Tilesets embarqués non supportés pour les fichiers TMX." << std::endl;
            tilesetPos = endPos;
            continue;
        }

        m_tilesets.push_back(tileset);
        tilesetPos = endPos;
    }

    std::cout << "Nombre de tilesets chargés: " << m_tilesets.size() << std::endl;

    // Extraire les couches
    extractLayers(content);

    // Extraire les groupes (qui contiennent des couches)
    extractGroups(content);

    std::cout << "Nombre total de couches chargées: " << m_layers.size() << std::endl;
}

void TiledMap::extractLayers(const std::string &content)
{
    size_t layerPos = 0;
    while ((layerPos = content.find("<layer ", layerPos)) != std::string::npos)
    {
        // Vérifier si cette couche est à l'intérieur d'un groupe
        size_t groupStartPos = content.rfind("<group ", layerPos);
        size_t groupEndPos = content.rfind("</group>", layerPos);
        if (groupStartPos != std::string::npos && (groupEndPos == std::string::npos || groupEndPos < groupStartPos))
        {
            // Cette couche est à l'intérieur d'un groupe, on la traitera dans extractGroups
            layerPos += 7; // Longueur de "<layer "
            continue;
        }

        size_t endLayerPos = content.find("</layer>", layerPos);
        if (endLayerPos == std::string::npos)
            break;

        std::string layerContent = content.substr(layerPos, endLayerPos - layerPos + 8); // +8 pour "</layer>"

        TileLayer layer;
        layer.name = extractAttribute(layerContent, "name=\"", "\"");
        std::string widthStr = extractAttribute(layerContent, "width=\"", "\"");
        std::string heightStr = extractAttribute(layerContent, "height=\"", "\"");
        layer.width = std::stoi(widthStr);
        layer.height = std::stoi(heightStr);

        // Vérifier si la couche est visible
        std::string visibleStr = extractAttribute(layerContent, "visible=\"", "\"");
        layer.visible = visibleStr.empty() || visibleStr == "1";

        // Vérifier l'opacité de la couche
        std::string opacityStr = extractAttribute(layerContent, "opacity=\"", "\"");
        layer.opacity = opacityStr.empty() ? 1.0f : std::stof(opacityStr);

        std::cout << "Couche trouvée: " << layer.name << ", " << layer.width << "x" << layer.height
                  << ", visible: " << layer.visible << ", opacité: " << layer.opacity << std::endl;

        // Extraire les données de la couche
        std::string dataTag = extractTag(layerContent, "<data ", "</data>");
        std::string encodingStr = extractAttribute(dataTag, "encoding=\"", "\"");

        if (encodingStr == "csv")
        {
            // Extraire les données CSV
            size_t startData = dataTag.find(">") + 1;
            std::string csvData = dataTag.substr(startData);

            // Nettoyer les données (enlever les espaces et les retours à la ligne)
            csvData.erase(std::remove(csvData.begin(), csvData.end(), '\n'), csvData.end());
            csvData.erase(std::remove(csvData.begin(), csvData.end(), '\r'), csvData.end());
            csvData.erase(std::remove(csvData.begin(), csvData.end(), ' '), csvData.end());

            // Diviser la chaîne en nombres
            size_t pos = 0;
            while (pos < csvData.length())
            {
                size_t commaPos = csvData.find(",", pos);
                if (commaPos == std::string::npos)
                    commaPos = csvData.length();

                std::string numStr = csvData.substr(pos, commaPos - pos);
                if (!numStr.empty())
                {
                    try
                    {
                        int tileId = std::stoi(numStr);
                        layer.data.push_back(tileId);
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << "Erreur de conversion: " << e.what() << " pour '" << numStr << "'" << std::endl;
                    }
                }

                pos = commaPos + 1;
            }

            std::cout << "Données chargées: " << layer.data.size() << " tuiles" << std::endl;
        }
        else
        {
            std::cerr << "Erreur: Format d'encodage non supporté: " << encodingStr << std::endl;
        }

        m_layers.push_back(layer);
        layerPos = endLayerPos + 8; // +8 pour "</layer>"
    }
}

void TiledMap::extractGroups(const std::string &content)
{
    size_t groupPos = 0;
    while ((groupPos = content.find("<group ", groupPos)) != std::string::npos)
    {
        size_t endGroupPos = content.find("</group>", groupPos);
        if (endGroupPos == std::string::npos)
            break;

        std::string groupContent = content.substr(groupPos, endGroupPos - groupPos + 8); // +8 pour "</group>"
        std::string groupName = extractAttribute(groupContent, "name=\"", "\"");

        std::cout << "Groupe trouvé: " << groupName << std::endl;

        // Extraire les couches dans ce groupe
        size_t layerPos = 0;
        while ((layerPos = groupContent.find("<layer ", layerPos)) != std::string::npos)
        {
            size_t endLayerPos = groupContent.find("</layer>", layerPos);
            if (endLayerPos == std::string::npos)
                break;

            std::string layerContent = groupContent.substr(layerPos, endLayerPos - layerPos + 8); // +8 pour "</layer>"

            TileLayer layer;
            layer.name = groupName + ":" + extractAttribute(layerContent, "name=\"", "\"");
            std::string widthStr = extractAttribute(layerContent, "width=\"", "\"");
            std::string heightStr = extractAttribute(layerContent, "height=\"", "\"");
            layer.width = std::stoi(widthStr);
            layer.height = std::stoi(heightStr);

            // Vérifier si la couche est visible
            std::string visibleStr = extractAttribute(layerContent, "visible=\"", "\"");
            layer.visible = visibleStr.empty() || visibleStr == "1";

            // Vérifier l'opacité de la couche
            std::string opacityStr = extractAttribute(layerContent, "opacity=\"", "\"");
            layer.opacity = opacityStr.empty() ? 1.0f : std::stof(opacityStr);

            std::cout << "  Couche trouvée dans le groupe: " << layer.name << ", " << layer.width << "x" << layer.height
                      << ", visible: " << layer.visible << ", opacité: " << layer.opacity << std::endl;

            // Extraire les données de la couche
            std::string dataTag = extractTag(layerContent, "<data ", "</data>");
            std::string encodingStr = extractAttribute(dataTag, "encoding=\"", "\"");

            if (encodingStr == "csv")
            {
                // Extraire les données CSV
                size_t startData = dataTag.find(">") + 1;
                std::string csvData = dataTag.substr(startData);

                // Nettoyer les données (enlever les espaces et les retours à la ligne)
                csvData.erase(std::remove(csvData.begin(), csvData.end(), '\n'), csvData.end());
                csvData.erase(std::remove(csvData.begin(), csvData.end(), '\r'), csvData.end());
                csvData.erase(std::remove(csvData.begin(), csvData.end(), ' '), csvData.end());

                // Diviser la chaîne en nombres
                size_t pos = 0;
                while (pos < csvData.length())
                {
                    size_t commaPos = csvData.find(",", pos);
                    if (commaPos == std::string::npos)
                        commaPos = csvData.length();

                    std::string numStr = csvData.substr(pos, commaPos - pos);
                    if (!numStr.empty())
                    {
                        try
                        {
                            int tileId = std::stoi(numStr);
                            layer.data.push_back(tileId);
                        }
                        catch (const std::exception &e)
                        {
                            std::cerr << "Erreur de conversion: " << e.what() << " pour '" << numStr << "'" << std::endl;
                        }
                    }

                    pos = commaPos + 1;
                }

                std::cout << "  Données chargées: " << layer.data.size() << " tuiles" << std::endl;
            }
            else
            {
                std::cerr << "Erreur: Format d'encodage non supporté: " << encodingStr << std::endl;
            }

            m_layers.push_back(layer);
            layerPos = endLayerPos + 8; // +8 pour "</layer>"
        }

        groupPos = endGroupPos + 8; // +8 pour "</group>"
    }
}

std::string TiledMap::extractAttribute(const std::string &content, const std::string &attributeStart, const std::string &attributeEnd)
{
    size_t startPos = content.find(attributeStart);
    if (startPos == std::string::npos)
        return "";

    startPos += attributeStart.length();
    size_t endPos = content.find(attributeEnd, startPos);
    if (endPos == std::string::npos)
        return "";

    return content.substr(startPos, endPos - startPos);
}

std::string TiledMap::extractTag(const std::string &content, const std::string &tagStart, const std::string &tagEnd)
{
    size_t startPos = content.find(tagStart);
    if (startPos == std::string::npos)
        return "";

    size_t endPos = content.find(tagEnd, startPos + tagStart.length());
    if (endPos == std::string::npos)
        return "";

    return content.substr(startPos, endPos + tagEnd.length() - startPos);
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
    // Cette méthode n'est plus nécessaire avec notre approche de rendu directe
    // mais on la garde vide pour maintenir la compatibilité
}

Tileset *TiledMap::getTilesetForGid(int gid) const
{
    // Trouver le bon tileset pour cet indice global
    for (int i = m_tilesets.size() - 1; i >= 0; --i)
    {
        if (m_tilesets[i].firstgid <= gid)
        {
            return const_cast<Tileset *>(&m_tilesets[i]);
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

        // Dessiner la couche avec le tileset approprié
        sf::Vertex vertices[4];
        for (int y = 0; y < layer.height; ++y)
        {
            for (int x = 0; x < layer.width; ++x)
            {
                // Calculer l'indice de la tuile courante
                int tileIndex = x + y * layer.width;

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

                // Définir les positions des sommets (rectangle)
                vertices[0].position = sf::Vector2f(x * m_tileWidth, y * m_tileHeight);
                vertices[1].position = sf::Vector2f((x + 1) * m_tileWidth, y * m_tileHeight);
                vertices[2].position = sf::Vector2f((x + 1) * m_tileWidth, (y + 1) * m_tileHeight);
                vertices[3].position = sf::Vector2f(x * m_tileWidth, (y + 1) * m_tileHeight);

                // Définir les coordonnées de texture des sommets
                vertices[0].texCoords = sf::Vector2f(tu * tileset->tileWidth, tv * tileset->tileHeight);
                vertices[1].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, tv * tileset->tileHeight);
                vertices[2].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, (tv + 1) * tileset->tileHeight);
                vertices[3].texCoords = sf::Vector2f(tu * tileset->tileWidth, (tv + 1) * tileset->tileHeight);

                // Appliquer l'opacité de la couche
                sf::Color color(255, 255, 255, static_cast<sf::Uint8>(layer.opacity * 255));
                vertices[0].color = color;
                vertices[1].color = color;
                vertices[2].color = color;
                vertices[3].color = color;

                // Configurer les états avec la texture du tileset
                states.texture = &tileset->texture;

                // Dessiner le quad
                target.draw(vertices, 4, sf::Quads, states);
            }
        }
    }
}