#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "../utils/TextFormatter.h"
#include "../utils/DataSystem.h"

/**
 * Gestionnaire de langues pour le jeu
 */
class LanguageManager {
public:
    /**
     * Obtenir l'instance unique du gestionnaire de langues
     * @return Instance unique
     */
    static LanguageManager& getInstance();

    /**
     * Définir la langue actuelle
     * @param langCode Code de la langue (ex: "en", "fr")
     * @return true si le changement a réussi, false sinon
     */
    bool setLanguage(const std::string& langCode);
    
    /**
     * Obtenir la langue actuelle
     * @return Code de la langue actuelle
     */
    std::string getCurrentLanguage() const;
    
    /**
     * Obtenir une traduction
     * @param key Clé de traduction
     * @param defaultValue Valeur par défaut si la traduction n'existe pas
     * @return Texte traduit
     */
    std::string get(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * Obtenir une traduction formatée avec des placeholders {}
     * @param key Clé de traduction
     * @param args Arguments à insérer dans la traduction
     * @return Texte traduit et formaté
     */
    template<typename... Args>
    std::string format(const std::string& key, Args... args) const {
        std::string text = get(key);
        if (!text.empty()) {
            return TextFormatter::format(text, args...);
        }
        return key;
    }
    
    /**
     * Obtenir une traduction formatée avec des placeholders numérotés {0}, {1}, etc.
     * @param key Clé de traduction
     * @param args Arguments à insérer dans la traduction
     * @return Texte traduit et formaté
     */
    template<typename... Args>
    std::string formatIndexed(const std::string& key, Args... args) const {
        std::string text = get(key);
        if (!text.empty()) {
            return TextFormatter::formatIndexed(text, args...);
        }
        return key;
    }
    
    /**
     * Recharger toutes les langues disponibles
     */
    void reloadLanguages();
    
    /**
     * Obtenir la liste des langues disponibles
     * @return Map de code de langue -> nom de la langue
     */
    std::unordered_map<std::string, std::string> getAvailableLanguages() const;

private:
    LanguageManager();
    ~LanguageManager() = default;
    LanguageManager(const LanguageManager&) = delete;
    LanguageManager& operator=(const LanguageManager&) = delete;
    
    // Langue actuelle
    std::string currentLanguage = "en";
    
    // Traduductions chargées
    std::unordered_map<std::string, std::string> translations;
    
    // Langues disponibles
    std::unordered_map<std::string, std::string> availableLanguages;
    
    // Charger une langue depuis un fichier
    bool loadLanguageFile(const std::string& langCode);
}; 