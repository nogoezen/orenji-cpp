#pragma once

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>
#include <limits>

class ConsoleUI
{
public:
    // Structure pour représenter une option de menu
    struct MenuItem
    {
        std::string name;
        std::function<void()> action;

        // Constructor for easier creation
        MenuItem(const std::string &n, std::function<void()> a = nullptr)
            : name(n), action(a) {}
    };

private:
    int m_screenWidth;
    int m_screenHeight;

public:
    ConsoleUI(int width = 80, int height = 24)
        : m_screenWidth(width), m_screenHeight(height) {}

    // Effacer l'écran
    void clearScreen() const
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // Afficher un titre
    void displayTitle(const std::string &title) const
    {
        clearScreen();

        std::cout << std::string(m_screenWidth, '=') << std::endl;
        std::cout << std::string((m_screenWidth - title.length()) / 2, ' ') << title << std::endl;
        std::cout << std::string(m_screenWidth, '=') << std::endl;
        std::cout << std::endl;
    }

    // Afficher un message
    void displayMessage(const std::string &message) const
    {
        std::cout << message << std::endl;
    }

    // Afficher une erreur
    void displayError(const std::string &message) const
    {
        std::cout << "ERREUR: " << message << std::endl;
    }

    // Afficher un menu et obtenir le choix de l'utilisateur
    int displayMenu(const std::string &title, const std::vector<MenuItem> &items);

    // Overload to accept string vectors directly
    int displayMenu(const std::string &title, const std::vector<std::string> &options);

    // Obtenir une entrée de texte
    std::string getInput(const std::string &prompt);

    // Obtenir une entrée numérique
    int getIntInput(const std::string &prompt, int min = std::numeric_limits<int>::min(),
                    int max = std::numeric_limits<int>::max());

    // Attendre que l'utilisateur appuie sur une touche
    void waitForKeyPress();

    // Afficher une table formatée
    template <typename T>
    void displayTable(const std::vector<std::string> &headers,
                      const std::vector<std::vector<T>> &data,
                      const std::vector<int> &columnWidths) const
    {
        // Afficher les en-têtes
        for (size_t i = 0; i < headers.size(); ++i)
        {
            std::cout << "| " << std::left << std::setw(columnWidths[i]) << headers[i] << " ";
        }
        std::cout << "|" << std::endl;

        // Ligne de séparation
        int totalWidth = 1; // For the first |
        for (int width : columnWidths)
        {
            totalWidth += width + 3; // width + | + spaces
        }
        std::cout << std::string(totalWidth, '-') << std::endl;

        // Afficher les données
        for (const auto &row : data)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::cout << "| " << std::left << std::setw(columnWidths[i]) << row[i] << " ";
            }
            std::cout << "|" << std::endl;
        }
    }
};