#include "DataSystem.h"
#include <filesystem>
#include <fstream>

// Initialisation de l'instance unique
std::unique_ptr<DataSystem> DataSystem::s_instance = nullptr;

// Obtenir l'instance unique
DataSystem& DataSystem::getInstance() {
    if (!s_instance) {
        s_instance = std::unique_ptr<DataSystem>(new DataSystem());
    }
    return *s_instance;
}

// Initialiser le système
bool DataSystem::initialize() {
    // Déterminer le répertoire de base des données
    baseDataDirectory = "data";
    
    // Vérifier que le répertoire existe
    if (!std::filesystem::exists(baseDataDirectory)) {
        std::filesystem::create_directory(baseDataDirectory);
    }
    
    return true;
}

// Obtenir le chemin vers un fichier de données
std::string DataSystem::getFilePath(const std::string& relativePath) const {
    return baseDataDirectory + "/" + relativePath;
}

// Obtenir le chemin vers un répertoire de données
std::string DataSystem::getDirectoryPath(const std::string& relativePath) const {
    return baseDataDirectory + "/" + relativePath;
}

// Charger un fichier JSON
nlohmann::json DataSystem::loadJson(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier JSON: " + filePath);
    }
    
    try {
        return nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("Erreur de parsing JSON dans le fichier: " + filePath + "\n" + e.what());
    }
} 