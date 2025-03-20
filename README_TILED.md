# Système de cartes Tiled pour Orenji Engine

Ce module permet d'intégrer facilement des cartes créées avec [Tiled Map Editor](https://www.mapeditor.org/) dans vos jeux utilisant Orenji Engine.

## Caractéristiques

- Chargement efficace des cartes TMX avec mise en cache
- Rendu des cartes avec prise en charge des calques et des tilesets
- Système de collision basé sur les calques
- Création d'entités à partir des objets définis dans la carte
- Scène de base prête à l'emploi pour les jeux basés sur des cartes

## Comment utiliser

### Chargement simple d'une carte

```cpp
// Configurer le répertoire racine (optionnel)
TiledMapLoader::setRootDirectory("assets/maps/");

// Charger une carte (elle sera mise en cache)
auto map = TiledMapLoader::load("level1.tmx");

// Vérifier si une carte est chargée
bool isLoaded = TiledMapLoader::isLoaded("level1.tmx");

// Libérer une carte de la mémoire
TiledMapLoader::unload("level1.tmx");

// Libérer toutes les cartes
TiledMapLoader::unloadAll();
```

### Utilisation avec TiledMapScene

Pour une intégration complète, il est recommandé d'utiliser ou d'étendre la classe `TiledMapScene` :

```cpp
class GameScene : public TiledMapScene
{
public:
    void initialize() override
    {
        // Charger la carte
        loadMap("level1.tmx");
        
        // Initialiser la scène (crée les entités, caméra, etc.)
        TiledMapScene::initialize();
        
        // Configurer les calques de collision
        setCollisionLayers({"Collision", "Obstacles"});
    }
    
    void registerEntityTypes() override
    {
        // Enregistrer les types d'entités personnalisés
        TiledMapFactory* factory = getFactory();
        if (factory)
        {
            factory->registerType("player", [this](Object* obj) -> Entity* {
                return createPlayer(obj);
            });
            
            factory->registerType("enemy", [this](Object* obj) -> Entity* {
                return createEnemy(obj);
            });
        }
    }
    
private:
    Entity* createPlayer(Object* obj) {
        // Créer et configurer l'entité joueur
        // ...
    }
    
    Entity* createEnemy(Object* obj) {
        // Créer et configurer l'entité ennemi
        // ...
    }
};
```

## Structure de la carte Tiled

Pour que votre carte fonctionne correctement avec ce système :

1. **Calques de tuiles** : Utilisés pour le rendu et les collisions
   - Nommez vos calques de collision de manière cohérente (ex: "Collision", "Obstacles")

2. **Calques d'objets** : Utilisés pour placer des entités
   - Définissez le type d'objet dans Tiled (ex: "player", "enemy", "item")
   - Utilisez les propriétés personnalisées pour définir des attributs supplémentaires

3. **Propriétés de carte** : Peuvent être utilisées pour des paramètres globaux
   - Accès via `map->getProperty("nomPropriété")`

## Optimisation

- Les cartes sont automatiquement mises en cache pour éviter les chargements redondants
- Utilisez `setVisibleLayers()` pour limiter le rendu aux calques nécessaires
- Configurez uniquement les calques de collision requis avec `setCollisionLayers()`

## Exemple complet

Voir les fichiers d'exemple fournis :
- `examples/TiledMapExample.cpp` : Exemple d'utilisation basique
- `examples/Player.hpp` : Exemple de composant joueur avec collision 