#include <SFML/Graphics.hpp>
#include "../include/TiledMap/TiledMapLoader.hpp"
#include <iostream>

int main()
{
    // Créer la fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Exemple de carte Tiled");
    window.setFramerateLimit(60);

    // Configurer le répertoire racine pour les cartes
    Orenji::TiledMapLoader::setRootDirectory("assets/maps/");

    // Charger une carte
    auto map = Orenji::TiledMapLoader::load("example.tmx");
    if (!map)
    {
        std::cerr << "Impossible de charger la carte!" << std::endl;
        return 1;
    }

    // Afficher les informations de la carte
    std::cout << "Carte chargée avec succès:" << std::endl;
    std::cout << "Taille: " << map->getWidth() << "x" << map->getHeight() << " tuiles" << std::endl;
    std::cout << "Taille des tuiles: " << map->getTileWidth() << "x" << map->getTileHeight() << " pixels" << std::endl;
    std::cout << "Taille totale: " << map->getWidthPixels() << "x" << map->getHeightPixels() << " pixels" << std::endl;

    // Accéder aux propriétés personnalisées
    std::cout << "Auteur: " << map->getProperty("author", "Inconnu") << std::endl;
    std::cout << "Difficulté: " << map->getPropertyInt("difficulty", 1) << std::endl;
    std::cout << "Est extérieur: " << (map->getPropertyBool("outdoor", false) ? "Oui" : "Non") << std::endl;

    // Créer des textures pour chaque tileset
    std::vector<sf::Texture> tilesetTextures;
    for (const auto &tileset : map->getTilesets())
    {
        sf::Texture texture;
        if (texture.loadFromFile(tileset.imagePath))
        {
            tilesetTextures.push_back(texture);
        }
        else
        {
            std::cerr << "Impossible de charger la texture du tileset: " << tileset.imagePath << std::endl;
        }
    }

    // Vue caméra
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    float zoomLevel = 1.0f;

    // Boucle principale
    while (window.isOpen())
    {
        // Traiter les événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Zoom avec la molette de la souris
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoomLevel *= 1.1f;
                else
                    zoomLevel *= 0.9f;

                view.setSize(800 / zoomLevel, 600 / zoomLevel);
            }

            // Déplacement avec les flèches
            if (event.type == sf::Event::KeyPressed)
            {
                float moveSpeed = 10.0f;
                sf::Vector2f viewCenter = view.getCenter();

                if (event.key.code == sf::Keyboard::Left)
                    viewCenter.x -= moveSpeed;
                if (event.key.code == sf::Keyboard::Right)
                    viewCenter.x += moveSpeed;
                if (event.key.code == sf::Keyboard::Up)
                    viewCenter.y -= moveSpeed;
                if (event.key.code == sf::Keyboard::Down)
                    viewCenter.y += moveSpeed;

                view.setCenter(viewCenter);
            }
        }

        // Appliquer la vue
        window.setView(view);

        // Effacer la fenêtre
        window.clear(sf::Color(50, 50, 50));

        // Rendu simple des couches de tuiles
        // (Dans un vrai jeu, vous auriez une classe TiledMapRenderer dédiée)
        for (const auto &layer : map->getTileLayers())
        {
            // Ignorer les couches invisibles
            if (!layer.visible)
                continue;

            // Dessiner chaque tuile de la couche
            for (int y = 0; y < map->getHeight(); y++)
            {
                for (int x = 0; x < map->getWidth(); x++)
                {
                    int tileIndex = y * map->getWidth() + x;
                    if (tileIndex < static_cast<int>(layer.tiles.size()))
                    {
                        uint32_t gid = layer.tiles[tileIndex];
                        if (gid == 0)
                            continue; // Tuile vide

                        // Trouver le tileset pour cette tuile
                        for (size_t i = 0; i < map->getTilesets().size(); i++)
                        {
                            const auto &tileset = map->getTilesets()[i];
                            if (gid >= tileset.firstGid && gid < tileset.firstGid + tileset.tileCount)
                            {
                                // Calculer l'ID local dans le tileset
                                uint32_t localId = gid - tileset.firstGid;

                                // Calculer la position dans la texture du tileset
                                int tu = localId % tileset.columns;
                                int tv = localId / tileset.columns;

                                // Créer un sprite pour la tuile
                                sf::Sprite tileSprite;
                                tileSprite.setTexture(tilesetTextures[i]);
                                tileSprite.setTextureRect(sf::IntRect(
                                    tu * tileset.tileWidth,
                                    tv * tileset.tileHeight,
                                    tileset.tileWidth,
                                    tileset.tileHeight));

                                // Positionner la tuile dans le monde
                                tileSprite.setPosition(
                                    x * map->getTileWidth() + layer.offsetX,
                                    y * map->getTileHeight() + layer.offsetY);

                                // Appliquer l'opacité de la couche
                                sf::Color color = sf::Color::White;
                                color.a = static_cast<sf::Uint8>(255 * layer.opacity);
                                tileSprite.setColor(color);

                                // Dessiner la tuile
                                window.draw(tileSprite);
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Dessiner les objets
        for (const auto &objLayer : map->getObjectLayers())
        {
            // Ignorer les couches invisibles
            if (!objLayer.visible)
                continue;

            for (const auto &obj : objLayer.objects)
            {
                // Ignorer les objets invisibles
                if (!obj.visible)
                    continue;

                // Dessiner une représentation simple des objets
                sf::RectangleShape shape;
                shape.setSize(sf::Vector2f(obj.width, obj.height));
                shape.setPosition(obj.x, obj.y);
                shape.setRotation(obj.rotation);

                // Couleur basée sur le type d'objet
                sf::Color color = sf::Color::Green;
                if (obj.type == "enemy")
                    color = sf::Color::Red;
                else if (obj.type == "door")
                    color = sf::Color::Blue;
                else if (obj.type == "chest")
                    color = sf::Color::Yellow;

                // Semi-transparent
                color.a = 128;
                shape.setFillColor(color);
                shape.setOutlineColor(sf::Color::White);
                shape.setOutlineThickness(1.0f);

                window.draw(shape);
            }
        }

        // Afficher la fenêtre
        window.display();
    }

    return 0;
}