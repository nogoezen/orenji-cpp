#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>

// Forward declarations pour tmxlite
namespace tmx
{
    class Map;
    class Property;
    class Layer;
    using LayerPtr = std::unique_ptr<Layer>;
}

namespace Orenji
{
    /**
     * @brief Classe pour charger et gérer une carte Tiled.
     */
    class TiledMap
    {
    public:
        /**
         * @brief Structure représentant un point 2D.
         */
        struct Point
        {
            float x; ///< Coordonnée X du point
            float y; ///< Coordonnée Y du point
        };

        /**
         * @brief Structure pour stocker les informations d'un objet.
         */
        struct Object
        {
            enum class Shape
            {
                Rectangle,
                Ellipse,
                Polygon,
                Polyline,
                Text
            };

            uint32_t id = 0;                               ///< ID unique de l'objet
            std::string name;                              ///< Nom de l'objet
            std::string type;                              ///< Type de l'objet
            float x = 0.f;                                 ///< Position X de l'objet
            float y = 0.f;                                 ///< Position Y de l'objet
            float width = 0.f;                             ///< Largeur de l'objet
            float height = 0.f;                            ///< Hauteur de l'objet
            float rotation = 0.f;                          ///< Rotation de l'objet en degrés
            bool visible = true;                           ///< Visibilité de l'objet
            uint32_t gid = 0;                              ///< ID global de la tuile (pour les objets tuile)
            Shape shape = Shape::Rectangle;                ///< Forme de l'objet
            std::vector<Point> points;                     ///< Points pour les polygones et polylignes
            std::string text;                              ///< Texte pour les objets de type texte
            std::map<std::string, std::string> properties; ///< Propriétés supplémentaires
        };

        /**
         * @brief Structure pour stocker les informations d'une couche de tuiles.
         */
        struct TileLayer
        {
            std::string name;                              ///< Nom de la couche
            float opacity = 1.f;                           ///< Opacité de la couche
            bool visible = true;                           ///< Visibilité de la couche
            float offsetX = 0.f;                           ///< Décalage X de la couche
            float offsetY = 0.f;                           ///< Décalage Y de la couche
            std::vector<uint32_t> tiles;                   ///< Tableau des IDs de tuiles
            std::map<std::string, std::string> properties; ///< Propriétés supplémentaires
        };

        /**
         * @brief Structure pour stocker les informations d'une couche d'objets.
         */
        struct ObjectLayer
        {
            std::string name;                              ///< Nom de la couche
            float opacity = 1.f;                           ///< Opacité de la couche
            bool visible = true;                           ///< Visibilité de la couche
            float offsetX = 0.f;                           ///< Décalage X de la couche
            float offsetY = 0.f;                           ///< Décalage Y de la couche
            std::vector<Object> objects;                   ///< Objets dans la couche
            std::map<std::string, std::string> properties; ///< Propriétés supplémentaires
        };

        /**
         * @brief Structure pour stocker les informations d'une couche d'image.
         */
        struct ImageLayer
        {
            std::string name;                              ///< Nom de la couche
            float opacity = 1.f;                           ///< Opacité de la couche
            bool visible = true;                           ///< Visibilité de la couche
            float offsetX = 0.f;                           ///< Décalage X de la couche
            float offsetY = 0.f;                           ///< Décalage Y de la couche
            std::string imagePath;                         ///< Chemin de l'image
            sf::Texture texture;                           ///< Texture de l'image
            std::map<std::string, std::string> properties; ///< Propriétés supplémentaires
        };

        /**
         * @brief Structure pour stocker les informations d'un tileset.
         */
        struct TilesetInfo
        {
            uint32_t firstGid = 0;   ///< Premier ID global
            uint32_t tileCount = 0;  ///< Nombre de tuiles
            uint32_t columns = 0;    ///< Nombre de colonnes
            uint32_t tileWidth = 0;  ///< Largeur d'une tuile
            uint32_t tileHeight = 0; ///< Hauteur d'une tuile
            uint32_t spacing = 0;    ///< Espacement entre les tuiles
            uint32_t margin = 0;     ///< Marge autour des tuiles
            std::string name;        ///< Nom du tileset
            std::string imagePath;   ///< Chemin de l'image
            sf::Texture texture;     ///< Texture du tileset
        };

        /**
         * @brief Structure pour stocker les informations d'une tuile.
         */
        struct TileInfo
        {
            uint32_t id = 0;                               ///< ID global de la tuile
            std::string type;                              ///< Type de la tuile
            std::map<std::string, std::string> properties; ///< Propriétés supplémentaires
        };

        /**
         * @brief Constructeur par défaut.
         */
        TiledMap();

        /**
         * @brief Destructeur.
         */
        ~TiledMap();

        /**
         * @brief Charge une carte à partir d'un fichier TMX.
         *
         * @param filepath Chemin du fichier TMX
         * @return true si le chargement a réussi, false sinon
         */
        bool loadFromFile(const std::string &filepath);

        /**
         * @brief Libère les ressources et réinitialise la carte.
         */
        void clear();

        /**
         * @brief Vérifie si la carte est chargée.
         *
         * @return true si la carte est chargée, false sinon
         */
        bool isLoaded() const;

        /**
         * @brief Obtient le chemin du fichier de la carte.
         *
         * @return Le chemin du fichier
         */
        const std::string &getFilePath() const;

        /**
         * @brief Obtient la largeur de la carte en tuiles.
         *
         * @return La largeur en tuiles
         */
        int getWidth() const;

        /**
         * @brief Obtient la hauteur de la carte en tuiles.
         *
         * @return La hauteur en tuiles
         */
        int getHeight() const;

        /**
         * @brief Obtient la largeur d'une tuile en pixels.
         *
         * @return La largeur d'une tuile
         */
        int getTileWidth() const;

        /**
         * @brief Obtient la hauteur d'une tuile en pixels.
         *
         * @return La hauteur d'une tuile
         */
        int getTileHeight() const;

        /**
         * @brief Obtient la largeur totale de la carte en pixels.
         *
         * @return La largeur totale
         */
        int getWidthPixels() const;

        /**
         * @brief Obtient la hauteur totale de la carte en pixels.
         *
         * @return La hauteur totale
         */
        int getHeightPixels() const;

        /**
         * @brief Obtient toutes les couches de tuiles.
         *
         * @return Les couches de tuiles
         */
        const std::vector<TileLayer> &getTileLayers() const;

        /**
         * @brief Obtient toutes les couches d'objets.
         *
         * @return Les couches d'objets
         */
        const std::vector<ObjectLayer> &getObjectLayers() const;

        /**
         * @brief Obtient toutes les couches d'images.
         *
         * @return Les couches d'images
         */
        const std::vector<ImageLayer> &getImageLayers() const;

        /**
         * @brief Obtient tous les tilesets.
         *
         * @return Les tilesets
         */
        const std::vector<TilesetInfo> &getTilesets() const;

        /**
         * @brief Obtient les informations d'une tuile spécifique.
         *
         * @param gid ID global de la tuile
         * @return Pointeur vers les informations de la tuile, ou nullptr si non trouvée
         */
        const TileInfo *getTileInfo(uint32_t gid) const;

        /**
         * @brief Obtient une propriété de la carte sous forme de chaîne.
         *
         * @param name Nom de la propriété
         * @param defaultValue Valeur par défaut
         * @return La valeur de la propriété ou la valeur par défaut
         */
        std::string getProperty(const std::string &name, const std::string &defaultValue = "") const;

        /**
         * @brief Obtient une propriété de la carte sous forme d'entier.
         *
         * @param name Nom de la propriété
         * @param defaultValue Valeur par défaut
         * @return La valeur de la propriété ou la valeur par défaut
         */
        int getPropertyInt(const std::string &name, int defaultValue = 0) const;

        /**
         * @brief Obtient une propriété de la carte sous forme de nombre flottant.
         *
         * @param name Nom de la propriété
         * @param defaultValue Valeur par défaut
         * @return La valeur de la propriété ou la valeur par défaut
         */
        float getPropertyFloat(const std::string &name, float defaultValue = 0.f) const;

        /**
         * @brief Obtient une propriété de la carte sous forme de booléen.
         *
         * @param name Nom de la propriété
         * @param defaultValue Valeur par défaut
         * @return La valeur de la propriété ou la valeur par défaut
         */
        bool getPropertyBool(const std::string &name, bool defaultValue = false) const;

        /**
         * @brief Met à jour la logique de la carte.
         *
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

    private:
        /**
         * @brief Charge une couche de tuiles.
         *
         * @param layer Pointeur vers la couche tmxlite
         * @param map Référence vers la carte tmxlite
         */
        void loadTileLayer(const tmx::LayerPtr &layer, const tmx::Map &map);

        /**
         * @brief Charge une couche d'objets.
         *
         * @param layer Pointeur vers la couche tmxlite
         */
        void loadObjectLayer(const tmx::LayerPtr &layer);

        /**
         * @brief Charge une couche d'image.
         *
         * @param layer Pointeur vers la couche tmxlite
         */
        void loadImageLayer(const tmx::LayerPtr &layer);

        /**
         * @brief Convertit une propriété tmxlite en chaîne.
         *
         * @param property Propriété à convertir
         * @return La propriété sous forme de chaîne
         */
        std::string getPropertyValueAsString(const tmx::Property &property);

    private:
        bool m_loaded;          ///< Indique si la carte est chargée
        std::string m_filepath; ///< Chemin du fichier de la carte
        int m_width;            ///< Largeur en tuiles
        int m_height;           ///< Hauteur en tuiles
        int m_tileWidth;        ///< Largeur d'une tuile
        int m_tileHeight;       ///< Hauteur d'une tuile

        std::vector<TileLayer> m_tileLayers;     ///< Couches de tuiles
        std::vector<ObjectLayer> m_objectLayers; ///< Couches d'objets
        std::vector<ImageLayer> m_imageLayers;   ///< Couches d'images
        std::vector<TilesetInfo> m_tilesets;     ///< Tilesets

        std::unordered_map<uint32_t, TileInfo> m_tileInfos; ///< Informations sur les tuiles
        std::map<std::string, std::string> m_properties;    ///< Propriétés de la carte
    };

} // namespace Orenji