#include "LanguageManager.h"
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

LanguageManager& LanguageManager::getInstance() {
    static LanguageManager instance;
    return instance;
}

LanguageManager::LanguageManager() {
    reloadLanguages();
    // Charger la langue par défaut (anglais)
    setLanguage("en");
}

bool LanguageManager::setLanguage(const std::string& langCode) {
    // Vérifier si la langue est disponible
    if (availableLanguages.find(langCode) == availableLanguages.end()) {
        std::cerr << "Langue non disponible: " << langCode << std::endl;
        return false;
    }
    
    // Charger la langue
    if (loadLanguageFile(langCode)) {
        currentLanguage = langCode;
        return true;
    }
    return false;
}

std::string LanguageManager::getCurrentLanguage() const {
    return currentLanguage;
}

std::string LanguageManager::get(const std::string& key, const std::string& defaultValue) const {
    auto it = translations.find(key);
    if (it != translations.end()) {
        return it->second;
    }
    // Si la clé n'existe pas, retourner la valeur par défaut ou la clé elle-même
    return defaultValue.empty() ? key : defaultValue;
}

void LanguageManager::reloadLanguages() {
    availableLanguages.clear();
    
    // Chercher tous les fichiers de langue dans le dossier data/lang
    try {
        std::string langPath = "data/lang";
        
        // Vérifier si le dossier existe
        if (!fs::exists(langPath)) {
            std::cerr << "Dossier de langues introuvable: " << langPath << std::endl;
            return;
        }
        
        // Parcourir les fichiers de langues
        for (const auto& entry : fs::directory_iterator(langPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::string langCode = entry.path().stem().string();
                
                // Charger le fichier pour extraire le nom de la langue
                try {
                    json langData = DataSystem::loadJson(entry.path().string());
                    if (langData.contains("language") && langData["language"].is_string()) {
                        availableLanguages[langCode] = langData["language"];
                    } else {
                        availableLanguages[langCode] = langCode; // Utiliser le code comme nom par défaut
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Erreur lors du chargement du fichier de langue " << langCode 
                              << ": " << e.what() << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des langues: " << e.what() << std::endl;
    }
}

std::unordered_map<std::string, std::string> LanguageManager::getAvailableLanguages() const {
    return availableLanguages;
}

bool LanguageManager::loadLanguageFile(const std::string& langCode) {
    try {
        // Vider les traductions actuelles
        translations.clear();
        
        // Charger le fichier JSON
        std::string filePath = "data/lang/" + langCode + ".json";
        json langData = DataSystem::loadJson(filePath);
        
        // Fonction récursive pour extraire les clés de traduction
        std::function<void(const json&, const std::string&)> extractTranslations = 
            [&](const json& data, const std::string& prefix) {
                if (data.is_object()) {
                    for (auto it = data.begin(); it != data.end(); ++it) {
                        std::string key = prefix.empty() ? it.key() : prefix + "." + it.key();
                        
                        if (it.value().is_string()) {
                            translations[key] = it.value();
                        } else if (it.value().is_object()) {
                            extractTranslations(it.value(), key);
                        }
                    }
                }
            };
        
        // Extraire les traductions
        extractTranslations(langData, "");
        
        std::cout << "Langue chargée: " << langCode 
                  << " (" << translations.size() << " traductions)" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement de la langue " << langCode 
                  << ": " << e.what() << std::endl;
        return false;
    }
} 