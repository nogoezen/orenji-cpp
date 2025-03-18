#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

// Structure pour représenter une ville
struct City {
    int id;
    std::string name;
    std::string description;
    std::string region;
    int posX;
    int posY;
    // Facteurs de prix pour les différents biens (goodId -> facteur)
    std::map<int, float> marketFactors;
};

// Structure pour représenter un élément de terrain
struct TerrainElement {
    enum Type {
        WATER,
        LAND,
        MOUNTAIN,
        CITY,
        PORT,
        DANGER
    };
    
    Type type;
    int cityId;  // Si type == CITY, l'ID de la ville
    bool passable;
    
    TerrainElement() : type(WATER), cityId(-1), passable(true) {}
};

class World {
private:
    // Dimensions du monde
    int m_width;
    int m_height;
    
    // Grille de terrain
    std::vector<std::vector<TerrainElement>> m_terrain;
    
    // Villes du monde
    std::vector<City> m_cities;
    
    // Instance singleton
    static std::unique_ptr<World> s_instance;
    
    // Constructeur privé (singleton)
    World(int width = 100, int height = 100) 
        : m_width(width), m_height(height) {
        initializeTerrain();
    }
    
    // Initialisation du terrain
    void initializeTerrain();
    
    // Création des villes
    void createCities();
    
    // Placement des villes sur la carte
    void placeCitiesOnMap();

public:
    // Obtenir l'instance unique (singleton)
    static World& getInstance() {
        if (!s_instance) {
            s_instance = std::unique_ptr<World>(new World());
        }
        return *s_instance;
    }
    
    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    const std::vector<City>& getCities() const { return m_cities; }
    
    // Accès au terrain
    const TerrainElement& getTerrain(int x, int y) const {
        // Vérifier les limites
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            static TerrainElement defaultTerrain;
            defaultTerrain.type = TerrainElement::WATER;
            defaultTerrain.passable = false;
            return defaultTerrain;
        }
        
        return m_terrain[y][x];
    }
    
    // Vérifier si une position est passable
    bool isPassable(int x, int y) const {
        return getTerrain(x, y).passable;
    }
    
    // Obtenir une ville par ID
    const City* getCityById(int id) const {
        for (const auto& city : m_cities) {
            if (city.id == id) {
                return &city;
            }
        }
        return nullptr;
    }
    
    // Obtenir une ville à une position
    const City* getCityAtPosition(int x, int y) const {
        const TerrainElement& element = getTerrain(x, y);
        if (element.type == TerrainElement::CITY && element.cityId >= 0) {
            return getCityById(element.cityId);
        }
        return nullptr;
    }
    
    // Vérifier si une position est un port
    bool isPort(int x, int y) const {
        return getTerrain(x, y).type == TerrainElement::PORT;
    }
}; 