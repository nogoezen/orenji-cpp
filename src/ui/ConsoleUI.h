#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <iostream>

/**
 * @brief Classe de base pour les interfaces utilisateur en console
 *
 * Fournit des utilitaires pour l'affichage de menus, de messages, et la gestion des entrées
 * utilisateur dans un environnement console.
 */
class ConsoleUI
{
public:
    // Structure pour les options de menu
    struct MenuItem
    {
        std::string label;
        std::function<void()> action;
        bool enabled;

        MenuItem(const std::string &lbl, std::function<void()> act, bool en = true)
            : label(lbl), action(act), enabled(en) {}
    };

protected:
    // Efface l'écran de la console (compatible Windows/Unix)
    static void clearScreen();

    // Affiche un titre centré avec bordure
    static void displayTitle(const std::string &title);

    // Attendre une entrée utilisateur
    static void waitForEnter(const std::string &message = "Appuyez sur ENTRÉE pour continuer...");

    // Afficher un menu avec options numérotées et exécuter l'action sélectionnée
    int displayMenu(const std::string &title, const std::vector<MenuItem> &items);

    // Afficher un message d'erreur
    static void displayError(const std::string &message);

    // Demander une entrée à l'utilisateur avec validation
    template <typename T>
    static T getUserInput(const std::string &prompt, std::function<bool(const T &)> validator = [](const T &)
                                                     { return true; },
                          const std::string &errorMsg = "Entrée invalide. Veuillez réessayer.");

    // Tracer une ligne horizontale
    static void drawLine(int width = 80, char symbol = '-');

    // Afficher une liste de chaînes avec numérotation
    static void displayNumberedList(const std::vector<std::string> &items, const std::string &title);

    // Afficher une liste de chaînes avec valeurs (pour les attributs)
    static void displayAttributes(const std::map<std::string, std::string> &attributes, const std::string &title);
};

// Implémentation du template
template <typename T>
T ConsoleUI::getUserInput(const std::string &prompt,
                          std::function<bool(const T &)> validator,
                          const std::string &errorMsg)
{
    T input;
    bool valid = false;

    do
    {
        std::cout << prompt;
        if (!(std::cin >> input))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << errorMsg << std::endl;
            continue;
        }

        std::cin.ignore(10000, '\n');

        valid = validator(input);
        if (!valid)
        {
            std::cout << errorMsg << std::endl;
        }
    } while (!valid);

    return input;
}