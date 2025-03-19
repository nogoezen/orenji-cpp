#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

/**
 * @brief Représente une couche de la carte Tiled
 */
struct TileLayer
{
    std::string name;
    int width;
    int height;
    std::vector<int> data;
    float opacity;
    bool visible;
};

/**
 * @brief Représente un tileset (ensemble de tuiles) utilisé par la carte
 */
struct Tileset
{
    int firstgid;
    std::string name;
    std::string source;
    int tileWidth;
    int tileHeight;
    int tileCount;
    int columns;
    sf::Texture texture;
};

/**
 * @brief Classe qui gère une carte créée avec l'éditeur Tiled
 */
class TiledMap : public sf::Drawable, public sf::Transformable
{
private:
    // Propriétés de la carte
    int m_width;
    int m_height;
    int m_tileWidth;
    int m_tileHeight;

    // Couches et tilesets
    std::vector<TileLayer> m_layers;
    std::vector<Tileset> m_tilesets;

    // Rendu
    sf::VertexArray m_vertices;

    // Méthodes privées
    void load(const std::string &filePath);
    void createVertices();
    void createDefaultTileset(Tileset &tileset);
    Tileset *getTilesetForGid(int gid);

    // Surcharge de la fonction de dessin (héritée de sf::Drawable)
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    /**
     * @brief Constructeur
     * @param filePath Chemin vers le fichier de carte Tiled au format JSON
     */
    TiledMap(const std::string &filePath);

    /**
     * @brief Destructeur
     */
    ~TiledMap();

    /**
     * @brief Retourne la largeur de la carte en nombre de tuiles
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Retourne la hauteur de la carte en nombre de tuiles
     */
    int getHeight() const { return m_height; }

    /**
     * @brief Retourne la largeur d'une tuile en pixels
     */
    int getTileWidth() const { return m_tileWidth; }

    /**
     * @brief Retourne la hauteur d'une tuile en pixels
     */
    int getTileHeight() const { return m_tileHeight; }

    /**
     * @brief Retourne la largeur totale de la carte en pixels
     */
    int getPixelWidth() const { return m_width * m_tileWidth; }

    /**
     * @brief Retourne la hauteur totale de la carte en pixels
     */
    int getPixelHeight() const { return m_height * m_tileHeight; }
};