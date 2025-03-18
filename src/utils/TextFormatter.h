#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <stdexcept>

/**
 * Classe utilitaire pour le formatage de texte
 */
class TextFormatter {
public:
    /**
     * Formater un texte en remplaçant les placeholders {} par les valeurs
     * @param format Format avec placeholders {}
     * @param args Arguments à insérer
     * @return Texte formaté
     */
    template<typename... Args>
    static std::string format(const std::string& format, Args... args) {
        return formatInternal(format, 0, args...);
    }
    
    /**
     * Formater un texte en remplaçant les placeholders numérotés {0}, {1}, etc.
     * @param format Format avec placeholders numérotés
     * @param args Arguments à insérer
     * @return Texte formaté
     */
    template<typename... Args>
    static std::string formatIndexed(const std::string& format, Args... args) {
        std::vector<std::string> argStrings;
        captureArgs(argStrings, args...);
        
        std::string result = format;
        std::regex pattern("\\{(\\d+)\\}");
        
        auto begin = std::sregex_iterator(format.begin(), format.end(), pattern);
        auto end = std::sregex_iterator();
        
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            int index = std::stoi(match[1].str());
            
            if (index >= 0 && index < argStrings.size()) {
                std::string placeholder = match[0].str();
                size_t pos = result.find(placeholder);
                if (pos != std::string::npos) {
                    result.replace(pos, placeholder.length(), argStrings[index]);
                }
            }
        }
        
        return result;
    }
    
private:
    // Cas de base pour la récursion
    static std::string formatInternal(const std::string& format, int) {
        return format;
    }
    
    // Cas récursif pour la récursion
    template<typename T, typename... Args>
    static std::string formatInternal(const std::string& format, int index, T value, Args... args) {
        std::string result = format;
        std::string placeholder = "{}";
        
        size_t pos = result.find(placeholder);
        if (pos != std::string::npos) {
            std::ostringstream oss;
            oss << value;
            result.replace(pos, placeholder.length(), oss.str());
        }
        
        return formatInternal(result, index + 1, args...);
    }
    
    // Cas de base pour capturer les arguments
    static void captureArgs(std::vector<std::string>&) {
        // Ne rien faire
    }
    
    // Cas récursif pour capturer les arguments
    template<typename T, typename... Args>
    static void captureArgs(std::vector<std::string>& argStrings, T value, Args... args) {
        std::ostringstream oss;
        oss << value;
        argStrings.push_back(oss.str());
        captureArgs(argStrings, args...);
    }
}; 