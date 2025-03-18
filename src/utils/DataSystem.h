#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * Système de gestion des données
 */
class DataSystem {
public:
    /**
     * Obtenir l'instance unique du système de données
     * @return Instance unique
     */
    static DataSystem& getInstance();
    
    /**
     * Initialiser le système
     * @return true si l'initialisation a réussi, false sinon
     */
    bool initialize();
    
    /**
     * Obtenir le chemin vers un fichier de données
     * @param relativePath Chemin relatif dans le système de données
     * @return Chemin complet vers le fichier
     */
    std::string getFilePath(const std::string& relativePath) const;
    
    /**
     * Obtenir le chemin vers un répertoire de données
     * @param relativePath Chemin relatif dans le système de données
     * @return Chemin complet vers le répertoire
     */
    std::string getDirectoryPath(const std::string& relativePath) const;
    
    /**
     * Charger un fichier JSON
     * @param filePath Chemin du fichier à charger
     * @return Objet JSON contenant les données
     */
    static nlohmann::json loadJson(const std::string& filePath);
    
    // Destructeur
    ~DataSystem() = default;
    
private:
    DataSystem() = default;
    DataSystem(const DataSystem&) = delete;
    DataSystem& operator=(const DataSystem&) = delete;
    
    // Instance unique
    static std::unique_ptr<DataSystem> s_instance;
    
    // Répertoire de base des données
    std::string baseDataDirectory;
}; 