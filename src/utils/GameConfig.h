#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include "FileUtils.h"
#include "JsonLoader.h"

/**
 * Classe pour gérer les configurations et les paramètres du jeu
 */
class GameConfig {
private:
    // Instance singleton
    static std::unique_ptr<GameConfig> s_instance;
    
    // Configurations
    nlohmann::json m_config;
    
    // Textes localisés
    std::unordered_map<std::string, std::string> m_texts;
    
    // Langue actuelle
    std::string m_currentLanguage;
    
    // Constructeur privé
    GameConfig() : m_currentLanguage("fr") {
        loadConfig();
        loadTexts(m_currentLanguage);
    }
    
    // Charger la configuration
    void loadConfig() {
        try {
            std::string configPath = FileUtils::joinPaths({"data", "config.json"});
            if (FileUtils::fileExists(configPath)) {
                m_config = JsonLoader::loadJsonFile(configPath);
            } else {
                // Configuration par défaut
                m_config = {
                    {"language", "fr"},
                    {"fullscreen", false},
                    {"sound", true},
                    {"music", true},
                    {"volume", 0.8},
                    {"difficulty", "normal"}
                };
                
                // Sauvegarder la configuration par défaut
                saveConfig();
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement de la configuration: " << e.what() << std::endl;
            
            // Configuration par défaut en cas d'erreur
            m_config = {
                {"language", "fr"},
                {"fullscreen", false},
                {"sound", true},
                {"music", true},
                {"volume", 0.8},
                {"difficulty", "normal"}
            };
        }
    }
    
    // Charger les textes localisés
    void loadTexts(const std::string& language) {
        m_texts.clear();
        
        try {
            std::string textsPath = FileUtils::joinPaths({"data", "lang", language + ".json"});
            if (FileUtils::fileExists(textsPath)) {
                nlohmann::json texts = JsonLoader::loadJsonFile(textsPath);
                
                // Traiter tous les textes
                for (auto it = texts.begin(); it != texts.end(); ++it) {
                    m_texts[it.key()] = it.value();
                }
                
                m_currentLanguage = language;
            } else {
                std::cerr << "Fichier de langue non trouvé: " << textsPath << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement des textes: " << e.what() << std::endl;
        }
    }

public:
    // Obtenir l'instance unique
    static GameConfig& getInstance() {
        if (!s_instance) {
            s_instance = std::unique_ptr<GameConfig>(new GameConfig());
        }
        return *s_instance;
    }
    
    // Sauvegarder la configuration
    bool saveConfig() {
        try {
            std::string configPath = FileUtils::joinPaths({"data", "config.json"});
            return FileUtils::writeTextFile(configPath, m_config.dump(4));
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors de la sauvegarde de la configuration: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Définir la langue
    void setLanguage(const std::string& language) {
        if (language != m_currentLanguage) {
            loadTexts(language);
            m_config["language"] = language;
            saveConfig();
        }
    }
    
    // Obtenir un texte localisé
    std::string getText(const std::string& key, const std::string& defaultValue = "") const {
        auto it = m_texts.find(key);
        if (it != m_texts.end()) {
            return it->second;
        }
        return defaultValue.empty() ? key : defaultValue;
    }
    
    // Obtenir une valeur de configuration
    template<typename T>
    T getValue(const std::string& key, const T& defaultValue) const {
        try {
            if (m_config.contains(key)) {
                return m_config[key].get<T>();
            }
        } catch (...) {
            // Ignorer les erreurs de conversion
        }
        return defaultValue;
    }
    
    // Définir une valeur de configuration
    template<typename T>
    void setValue(const std::string& key, const T& value) {
        m_config[key] = value;
    }
    
    // Obtenir la langue actuelle
    const std::string& getCurrentLanguage() const {
        return m_currentLanguage;
    }
    
    // Vérifier si une valeur existe
    bool hasValue(const std::string& key) const {
        return m_config.contains(key);
    }
    
    // Recharger la configuration
    void reload() {
        loadConfig();
        loadTexts(m_currentLanguage);
    }
}; 