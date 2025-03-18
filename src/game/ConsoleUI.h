#pragma once

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>

class ConsoleUI {
public:
    // Structure pour représenter une option de menu
    struct MenuItem {
        std::string label;
        std::function<void()> action;
    };

private:
    int m_screenWidth;
    int m_screenHeight;

public:
    ConsoleUI(int width = 80, int height = 24)
        : m_screenWidth(width), m_screenHeight(height) {}
    
    // Effacer l'écran
    void clearScreen() const {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    // Afficher un titre
    void displayTitle(const std::string& title) const {
        clearScreen();
        
        std::cout << std::string(m_screenWidth, '=') << std::endl;
        std::cout << std::string((m_screenWidth - title.length()) / 2, ' ') << title << std::endl;
        std::cout << std::string(m_screenWidth, '=') << std::endl;
        std::cout << std::endl;
    }
    
    // Afficher un message
    void displayMessage(const std::string& message) const {
        std::cout << message << std::endl;
    }
    
    // Afficher une erreur
    void displayError(const std::string& message) const {
        std::cout << "ERREUR: " << message << std::endl;
    }
    
    // Afficher un menu et obtenir le choix de l'utilisateur
    int displayMenu(const std::string& title, const std::vector<MenuItem>& items) const {
        displayTitle(title);
        
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i + 1 << ". " << items[i].label << std::endl;
        }
        
        std::cout << std::endl;
        std::cout << "Votre choix (1-" << items.size() << "): ";
        
        int choice;
        std::cin >> choice;
        
        // Validation de l'entrée
        while (std::cin.fail() || choice < 1 || choice > static_cast<int>(items.size())) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Choix invalide. Veuillez réessayer: ";
            std::cin >> choice;
        }
        
        // Exécuter l'action associée
        items[choice - 1].action();
        
        return choice;
    }
    
    // Obtenir une entrée de texte
    std::string getInput(const std::string& prompt) const {
        std::cout << prompt;
        
        std::string input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        
        return input;
    }
    
    // Obtenir une entrée numérique
    int getIntInput(const std::string& prompt, int min = std::numeric_limits<int>::min(), 
                   int max = std::numeric_limits<int>::max()) const {
        std::cout << prompt;
        
        int input;
        std::cin >> input;
        
        // Validation de l'entrée
        while (std::cin.fail() || input < min || input > max) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Entrée invalide. Veuillez entrer un nombre entre " 
                      << min << " et " << max << ": ";
            std::cin >> input;
        }
        
        return input;
    }
    
    // Attendre que l'utilisateur appuie sur une touche
    void waitForKeyPress() const {
        std::cout << std::endl << "Appuyez sur Entrée pour continuer...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    
    // Afficher une table formatée
    template<typename T>
    void displayTable(const std::vector<std::string>& headers, 
                     const std::vector<std::vector<T>>& data,
                     const std::vector<int>& columnWidths) const {
        // Afficher les en-têtes
        for (size_t i = 0; i < headers.size(); ++i) {
            std::cout << "| " << std::left << std::setw(columnWidths[i]) << headers[i] << " ";
        }
        std::cout << "|" << std::endl;
        
        // Ligne de séparation
        int totalWidth = 1;  // For the first |
        for (int width : columnWidths) {
            totalWidth += width + 3;  // width + | + spaces
        }
        std::cout << std::string(totalWidth, '-') << std::endl;
        
        // Afficher les données
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << "| " << std::left << std::setw(columnWidths[i]) << row[i] << " ";
            }
            std::cout << "|" << std::endl;
        }
    }
}; 