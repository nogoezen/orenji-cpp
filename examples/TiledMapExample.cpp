#include "../include/Core/Engine.hpp"
#include "../include/Core/Scene.hpp"
#include "../include/TiledMap/TiledMapScene.hpp"
#include "../include/TiledMap/TiledMapLoader.hpp"
#include "../include/TiledMap/TiledMap.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace Orenji;

class GameScene : public TiledMapScene
{
public:
    GameScene() : TiledMapScene() {}

    void initialize() override
    {
        // Configurer le répertoire des cartes
        TiledMapLoader::setRootDirectory("assets/maps/");

        // Charger la carte
        if (!loadMap("example_map.tmx"))
        {
            std::cerr << "Impossible de charger la carte !" << std::endl;
            return;
        }

        // Initialiser la scène de base (crée les entités, la caméra, etc.)
        TiledMapScene::initialize();

        // Configurer les calques de collision
        setCollisionLayers({"Collision", "Obstacles"});

        // Masquer les objets (optionnel)
        setObjectsVisible(false);

        // Créer un joueur manuellement si aucun n'est défini dans la carte
        createPlayerIfNeeded();
    }

    void registerEntityTypes() override
    {
        // Enregistrer les types d'entités personnalisés
        TiledMapFactory *factory = getFactory();
        if (factory)
        {
            factory->registerType("player", [this](Object *obj) -> Entity *
                                  { return createPlayer(obj); });
        }
    }

    void update(float deltaTime) override
    {
        // Mettre à jour la scène de base
        TiledMapScene::update(deltaTime);

        // Suivre le joueur avec la caméra
        if (m_playerEntity && m_cameraEntity)
        {
            // Obtenir la position du joueur
            sf::Vector2f playerPos = m_playerEntity->getPosition();

            // Positionner la caméra sur le joueur
            m_cameraEntity->setPosition(playerPos);
        }
    }

private:
    Entity *m_playerEntity = nullptr;

    void createPlayerIfNeeded()
    {
        // Vérifier si un joueur a déjà été créé par la factory
        auto entities = getEntityManager()->getEntities();
        for (auto *entity : entities)
        {
            if (entity->getName() == "Player")
            {
                m_playerEntity = entity;
                return;
            }
        }

        // Aucun joueur trouvé, en créer un manuellement
        Object dummyObj;
        dummyObj.name = "Player";
        dummyObj.type = "player";
        dummyObj.x = getMap()->getWidth() * getMap()->getTileWidth() / 2.0f;
        dummyObj.y = getMap()->getHeight() * getMap()->getTileHeight() / 2.0f;

        m_playerEntity = createPlayer(&dummyObj);
    }

    // Méthode pour créer une entité joueur
    Entity *createPlayer(Object *obj)
    {
        if (!obj)
            return nullptr;

        Entity *player = getEntityManager()->createEntity();
        player->setName("Player");
        player->setPosition(obj->x, obj->y);

        // Ajouter le composant joueur
        player->addComponent<PlayerComponent>();

        // Stocker l'entité joueur
        m_playerEntity = player;

        return player;
    }
};

/**
 * @brief Example demonstrating how to load and display a Tiled map
 *
 * This example shows how to:
 * 1. Configure the TiledMapLoader with a root directory
 * 2. Load a Tiled map from a .tmx file
 * 3. Access different types of layers (tile, object, image)
 * 4. Access map properties
 * 5. Simple rendering of tile layers
 */
int main()
{
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tiled Map Example");
    window.setFramerateLimit(60);

    // Set the root directory for maps
    Orenji::TiledMapLoader::setRootDirectory("assets/maps/");

    // Load a map
    auto map = Orenji::TiledMapLoader::load("example.tmx");
    if (!map)
    {
        std::cerr << "Failed to load map!" << std::endl;
        return 1;
    }

    // Print map information
    std::cout << "Map loaded successfully:" << std::endl;
    std::cout << "Size: " << map->getWidth() << "x" << map->getHeight() << " tiles" << std::endl;
    std::cout << "Tile size: " << map->getTileWidth() << "x" << map->getTileHeight() << " pixels" << std::endl;
    std::cout << "Total size: " << map->getWidthPixels() << "x" << map->getHeightPixels() << " pixels" << std::endl;

    // Access custom properties if defined in the Tiled map
    std::cout << "Author: " << map->getProperty("author", "Unknown") << std::endl;
    std::cout << "Difficulty: " << map->getPropertyInt("difficulty", 1) << std::endl;
    std::cout << "Is Outdoor: " << (map->getPropertyBool("outdoor", false) ? "Yes" : "No") << std::endl;

    // Create textures for each tileset
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
            std::cerr << "Failed to load tileset texture: " << tileset.imagePath << std::endl;
        }
    }

    // Camera view
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    float zoomLevel = 1.0f;

    // Main game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Camera zoom with mouse wheel
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    zoomLevel *= 1.1f;
                else
                    zoomLevel *= 0.9f;

                view.setSize(800 / zoomLevel, 600 / zoomLevel);
            }

            // Camera movement with arrow keys
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

        // Set the view
        window.setView(view);

        // Clear the window
        window.clear(sf::Color(50, 50, 50));

        // Simple manual rendering of the tile layers
        // (In a real game, you would use a dedicated TiledMapRenderer class)
        for (const auto &layer : map->getTileLayers())
        {
            // Skip invisible layers
            if (!layer.visible)
                continue;

            // Draw each tile in the layer
            for (int y = 0; y < map->getHeight(); y++)
            {
                for (int x = 0; x < map->getWidth(); x++)
                {
                    int tileIndex = y * map->getWidth() + x;
                    if (tileIndex < static_cast<int>(layer.tiles.size()))
                    {
                        uint32_t gid = layer.tiles[tileIndex];
                        if (gid == 0)
                            continue; // Empty tile

                        // Find the tileset for this tile
                        for (size_t i = 0; i < map->getTilesets().size(); i++)
                        {
                            const auto &tileset = map->getTilesets()[i];
                            if (gid >= tileset.firstGid && gid < tileset.firstGid + tileset.tileCount)
                            {
                                // Calculate the local tile ID within the tileset
                                uint32_t localId = gid - tileset.firstGid;

                                // Calculate the position in the tileset texture
                                int tu = localId % tileset.columns;
                                int tv = localId / tileset.columns;

                                // Create a sprite for the tile
                                sf::Sprite tileSprite;
                                tileSprite.setTexture(tilesetTextures[i]);
                                tileSprite.setTextureRect(sf::IntRect(
                                    tu * tileset.tileWidth,
                                    tv * tileset.tileHeight,
                                    tileset.tileWidth,
                                    tileset.tileHeight));

                                // Position the tile in the world
                                tileSprite.setPosition(
                                    x * map->getTileWidth() + layer.offsetX,
                                    y * map->getTileHeight() + layer.offsetY);

                                // Apply layer opacity
                                sf::Color color = sf::Color::White;
                                color.a = static_cast<sf::Uint8>(255 * layer.opacity);
                                tileSprite.setColor(color);

                                // Draw the tile
                                window.draw(tileSprite);
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Display the window
        window.display();
    }

    return 0;
}