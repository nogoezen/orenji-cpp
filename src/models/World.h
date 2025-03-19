#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

/**
 * @brief Classe représentant le monde du jeu
 *
 * Cette classe contient les informations sur le monde,
 * comme les villes, les routes commerciales, etc.
 */
class World
{
private:
    // Données du monde
    bool m_initialized = false;

    // Structure pour les villes
    struct City
    {
        int id;
        std::string name;
        std::string kingdom;
        int posX;
        int posY;
        std::vector<int> connections;
    };

    // Liste des villes du monde
    std::vector<City> m_cities;

public:
    /**
     * @brief Constructeur par défaut
     */
    World();

    /**
     * @brief Destructeur
     */
    ~World() = default;

    /**
     * @brief Initialise le monde
     * @return true si l'initialisation a réussi, false sinon
     */
    bool initialize();

    /**
     * @brief Convertit les données du monde en JSON pour la sauvegarde
     * @return Objet JSON représentant le monde
     */
    nlohmann::json toJson() const;

    /**
     * @brief Charge les données du monde depuis un objet JSON
     * @param data Objet JSON contenant les données du monde
     * @return true si le chargement a réussi, false sinon
     */
    bool fromJson(const nlohmann::json &data);

    /**
     * @brief Vérifie si le monde est initialisé
     * @return true si le monde est initialisé, false sinon
     */
    inline bool isInitialized() const { return m_initialized; }

    // Méthodes liées aux villes

    /**
     * @brief Obtient une ville par son ID
     * @param cityId ID de la ville
     * @return Référence vers la ville ou nullptr si non trouvée
     */
    const City *getCity(int cityId) const;

    /**
     * @brief Obtient une ville par sa position
     * @param x Coordonnée X
     * @param y Coordonnée Y
     * @return Référence vers la ville ou nullptr si non trouvée
     */
    const City *getCityAtPosition(int x, int y) const;

    /**
     * @brief Obtient la liste de toutes les villes
     * @return Liste des villes
     */
    const std::vector<City> &getCities() const { return m_cities; }

    /**
     * @brief Obtient la liste des villes d'un royaume spécifique
     * @param kingdom Nom du royaume
     * @return Liste des villes du royaume
     */
    std::vector<const City *> getCitiesByKingdom(const std::string &kingdom) const;
};