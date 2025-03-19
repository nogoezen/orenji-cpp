#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../models/TradeGood.h"

// Structure pour représenter une ville
struct City
{
    int id;
    std::string name;
    int x, y; // Coordonnées sur la carte
    std::string region;
    std::string description;
    std::vector<int> availableGoods; // IDs des biens disponibles
    std::vector<int> availableShips; // IDs des navires disponibles
    int population = 5000;           // Population (pour le calcul des facteurs économiques)

    nlohmann::json toJson() const;
    static City fromJson(const nlohmann::json &json);
    void updateFromJson(const nlohmann::json &json);
};

// Structure pour représenter une région
struct Region
{
    int id;
    std::string name;
    std::string description;
    std::vector<int> cities; // IDs des villes dans cette région

    nlohmann::json toJson() const;
    static Region fromJson(const nlohmann::json &json);
};

/**
 * @brief Classe qui gère le monde du jeu
 *
 * Cette classe représente l'environnement du jeu, avec les villes,
 * régions, et autres éléments géographiques.
 */
class CoreWorld
{
private:
    // Villes par ID
    std::unordered_map<int, City> m_cities;

    // Régions par ID
    std::unordered_map<int, Region> m_regions;

    // Position actuelle du joueur
    int m_currentCityId = -1;

    // Chargement des données du monde
    void loadCities();
    void loadRegions();

public:
    /**
     * Constructeur
     */
    CoreWorld();

    /**
     * Destructeur
     */
    ~CoreWorld();

    /**
     * Initialise le monde en chargeant les données
     * @return true si l'initialisation a réussi, false sinon
     */
    bool initialize();

    /**
     * Initialise le monde à partir de données JSON
     * @param data Données JSON du monde
     * @return true si l'initialisation a réussi, false sinon
     */
    bool initialize(const nlohmann::json &data);

    /**
     * Convertit le monde en JSON
     * @return JSON représentant le monde
     */
    nlohmann::json toJson() const;

    /**
     * Obtient une ville par son ID
     * @param cityId ID de la ville
     * @return Pointeur vers la ville ou nullptr si non trouvée
     */
    const City *getCityById(int cityId) const;

    /**
     * Obtient toutes les villes
     * @return Map des villes indexées par ID
     */
    const std::unordered_map<int, City> &getAllCities() const { return m_cities; }

    /**
     * Obtient une région par son ID
     * @param regionId ID de la région
     * @return Pointeur vers la région ou nullptr si non trouvée
     */
    const Region *getRegionById(int regionId) const;

    /**
     * Obtient toutes les régions
     * @return Map des régions indexées par ID
     */
    const std::unordered_map<int, Region> &getAllRegions() const { return m_regions; }

    /**
     * Obtient tous les biens commerciaux
     * @return Map des biens commerciaux indexés par ID
     */
    const std::unordered_map<int, std::shared_ptr<TradeGood>> getAllTradeGoods() const;

    /**
     * Obtient la ville actuelle du joueur
     * @return Pointeur vers la ville actuelle ou nullptr si le joueur n'est pas dans une ville
     */
    const City *getCurrentCity() const;

    /**
     * Définit la ville actuelle du joueur
     * @param cityId ID de la nouvelle ville
     * @return true si le changement a réussi
     */
    bool setCurrentCity(int cityId);

    /**
     * Calcule la distance entre deux villes
     * @param fromCityId ID de la ville de départ
     * @param toCityId ID de la ville d'arrivée
     * @return Distance entre les deux villes (en unités de jeu)
     */
    float calculateDistance(int fromCityId, int toCityId) const;
};

#endif // CORE_WORLD_H