# Module TiledMap pour Orenji

Ce module permet de charger et de gérer des cartes créées avec l'éditeur de cartes [Tiled](https://www.mapeditor.org/).

## Prérequis

- [TMXLite](https://github.com/fallahn/tmxlite) : Bibliothèque pour parser les fichiers TMX
- [SFML](https://www.sfml-dev.org/) : Bibliothèque pour le rendu graphique

## Installation de TMXLite

### Windows avec MSYS2

```bash
pacman -S mingw-w64-ucrt-x86_64-tmxlite
```

### Linux

```bash
# Compiler depuis les sources
git clone https://github.com/fallahn/tmxlite.git
cd tmxlite/tmxlite
cmake -DCMAKE_BUILD_TYPE=Release .
make
sudo make install
```

## Concepts principaux

### TiledMap

La classe `TiledMap` représente une carte Tiled chargée en mémoire. Elle permet d'accéder aux différentes couches, tuiles et objets de la carte.

Principales fonctionnalités :
- Chargement d'une carte TMX
- Accès aux propriétés de la carte (taille, propriétés personnalisées)
- Accès aux couches de tuiles, d'objets et d'images
- Accès aux tilesets et aux informations des tuiles

### TiledMapLoader

La classe `TiledMapLoader` fournit des méthodes statiques pour charger et gérer les cartes Tiled. Elle utilise un cache pour éviter de charger plusieurs fois la même carte.

Principales fonctionnalités :
- Chargement et mise en cache des cartes
- Gestion d'un répertoire racine pour les chemins relatifs
- Déchargement de cartes spécifiques ou de toutes les cartes

## Utilisation

### Chargement d'une carte

```cpp
// Configurer le répertoire racine (optionnel)
Orenji::TiledMapLoader::setRootDirectory("resources/maps/");

// Charger une carte
auto map = Orenji::TiledMapLoader::load("level1.tmx");
if (map)
{
    // La carte a été chargée avec succès
}
```

### Accès aux propriétés de la carte

```cpp
// Dimensions de la carte
int width = map->getWidth();        // Largeur en tuiles
int height = map->getHeight();      // Hauteur en tuiles
int tileWidth = map->getTileWidth(); // Largeur d'une tuile en pixels
int tileHeight = map->getTileHeight(); // Hauteur d'une tuile en pixels

// Propriétés personnalisées
std::string author = map->getProperty("author", "Unknown");
int difficulty = map->getPropertyInt("difficulty", 1);
bool isOutdoor = map->getPropertyBool("outdoor", false);
```

### Accès aux couches et tuiles

```cpp
// Accéder aux couches de tuiles
const auto& tileLayers = map->getTileLayers();
for (const auto& layer : tileLayers)
{
    // Traiter chaque couche de tuiles
    std::string layerName = layer.name;
    bool layerVisible = layer.visible;
    float layerOpacity = layer.opacity;
    
    // Accéder aux tuiles de la couche
    for (int y = 0; y < map->getHeight(); y++)
    {
        for (int x = 0; x < map->getWidth(); x++)
        {
            int index = y * map->getWidth() + x;
            uint32_t tileId = layer.tiles[index];
            
            // tileId == 0 signifie une tuile vide
            if (tileId != 0)
            {
                // Traiter la tuile
            }
        }
    }
}
```

### Accès aux objets

```cpp
// Accéder aux couches d'objets
const auto& objectLayers = map->getObjectLayers();
for (const auto& layer : objectLayers)
{
    // Traiter chaque couche d'objets
    std::string layerName = layer.name;
    
    // Accéder aux objets de la couche
    for (const auto& object : layer.objects)
    {
        // Traiter chaque objet
        uint32_t id = object.id;
        std::string name = object.name;
        std::string type = object.type;
        float x = object.x;
        float y = object.y;
        float width = object.width;
        float height = object.height;
        
        // Accéder aux propriétés personnalisées de l'objet
        auto it = object.properties.find("key");
        if (it != object.properties.end())
        {
            std::string value = it->second;
            // Traiter la propriété
        }
    }
}
```

## Exemple complet

Voir le fichier [examples/TiledMapExample.cpp](examples/TiledMapExample.cpp) pour un exemple complet montrant comment charger et afficher une carte Tiled. 