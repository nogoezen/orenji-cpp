#include "World.h"
#include <iostream>
#include <random>
#include "../utils/JsonLoader.h"

// Initialisation de l'instance unique
std::unique_ptr<World> World::s_instance = nullptr;

// Initialisation du terrain
void World::initializeTerrain() {
    // Redimensionner la grille
    m_terrain.resize(m_height, std::vector<TerrainElement>(m_width));
    
    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> landDist(1, 100);
    
    // Par défaut, tout est de l'eau
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            // 70% d'eau, 30% de terre
            if (landDist(gen) <= 70) {
                m_terrain[y][x].type = TerrainElement::WATER;
                m_terrain[y][x].passable = true;
            } else {
                m_terrain[y][x].type = TerrainElement::LAND;
                m_terrain[y][x].passable = false;
            }
        }
    }
    
    // Créer quelques villes
    createCities();
    
    // Placer les villes sur la carte
    placeCitiesOnMap();
}

// Créer les villes
void World::createCities() {
    // Vider la liste des villes
    m_cities.clear();
    
    // Créer quelques villes de base
    m_cities.push_back({
        1, "Lisbonne", "Capitale du Portugal, point de départ des explorateurs.", 
        "Europe", 20, 15, 
        { {1, 0.8f}, {2, 1.2f}, {3, 1.5f}, {4, 1.8f}, {5, 0.9f} }
    });
    
    m_cities.push_back({
        2, "Séville", "Port espagnol, porte vers le Nouveau Monde.", 
        "Europe", 18, 16,
        { {1, 0.9f}, {2, 1.0f}, {3, 1.3f}, {4, 1.6f}, {5, 1.1f} }
    });
    
    m_cities.push_back({
        3, "Londres", "Centre commercial anglais en plein essor.", 
        "Europe", 22, 10,
        { {1, 1.1f}, {2, 0.8f}, {3, 1.3f}, {4, 1.4f}, {5, 1.2f} }
    });
    
    m_cities.push_back({
        4, "Amsterdam", "Puissant port commercial hollandais.", 
        "Europe", 25, 12,
        { {1, 1.0f}, {2, 0.9f}, {3, 1.2f}, {4, 1.5f}, {5, 1.3f} }
    });
    
    m_cities.push_back({
        5, "Venise", "République marchande prospère en Méditerranée.", 
        "Europe", 30, 18,
        { {1, 0.7f}, {2, 1.3f}, {3, 1.1f}, {4, 1.7f}, {5, 1.0f} }
    });
    
    m_cities.push_back({
        6, "Marrakech", "Carrefour commercial du Maghreb.", 
        "Afrique", 16, 20,
        { {1, 0.6f}, {2, 1.1f}, {3, 1.4f}, {4, 1.3f}, {5, 1.5f} }
    });
    
    m_cities.push_back({
        7, "Istanbul", "Ancienne Constantinople, lien entre l'Europe et l'Asie.", 
        "Europe/Asie", 35, 17,
        { {1, 0.8f}, {2, 1.4f}, {3, 1.0f}, {4, 1.9f}, {5, 1.1f} }
    });
    
    m_cities.push_back({
        8, "La Havane", "Port des Caraïbes riche en ressources exotiques.", 
        "Amériques", 50, 25,
        { {1, 1.2f}, {2, 0.7f}, {3, 0.8f}, {4, 1.1f}, {5, 1.6f} }
    });
    
    m_cities.push_back({
        9, "Goa", "Comptoir portugais en Inde.", 
        "Asie", 60, 30,
        { {1, 0.5f}, {2, 1.0f}, {3, 2.0f}, {4, 1.5f}, {5, 1.4f} }
    });
    
    m_cities.push_back({
        10, "Nagasaki", "Port japonais ouvert au commerce étranger.", 
        "Asie", 80, 20,
        { {1, 0.6f}, {2, 0.9f}, {3, 1.7f}, {4, 2.1f}, {5, 1.2f} }
    });
}

// Placer les villes sur la carte
void World::placeCitiesOnMap() {
    for (const auto& city : m_cities) {
        // Vérifier que les coordonnées sont dans les limites
        if (city.posX >= 0 && city.posX < m_width && city.posY >= 0 && city.posY < m_height) {
            // Créer une zone de terre autour de la ville (pour garantir qu'elle n'est pas en pleine mer)
            for (int y = std::max(0, city.posY - 1); y <= std::min(m_height - 1, city.posY + 1); ++y) {
                for (int x = std::max(0, city.posX - 1); x <= std::min(m_width - 1, city.posX + 1); ++x) {
                    m_terrain[y][x].type = TerrainElement::LAND;
                }
            }
            
            // Placer la ville
            m_terrain[city.posY][city.posX].type = TerrainElement::CITY;
            m_terrain[city.posY][city.posX].cityId = city.id;
            m_terrain[city.posY][city.posX].passable = true;
            
            // Créer un port à côté de la ville
            bool portPlaced = false;
            
            // Essayer de placer un port dans les 4 directions
            std::vector<std::pair<int, int>> directions = {
                {0, 1}, {1, 0}, {0, -1}, {-1, 0}
            };
            
            for (const auto& [dx, dy] : directions) {
                int portX = city.posX + dx;
                int portY = city.posY + dy;
                
                if (portX >= 0 && portX < m_width && portY >= 0 && portY < m_height) {
                    m_terrain[portY][portX].type = TerrainElement::PORT;
                    m_terrain[portY][portX].passable = true;
                    portPlaced = true;
                    break;
                }
            }
        }
    }
} 