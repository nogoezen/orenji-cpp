#include "ConsoleUI.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>

int ConsoleUI::displayMenu(const std::string &title, const std::vector<MenuItem> &items)
{
    clearScreen();
    displayTitle(title);

    // Afficher les options du menu
    for (size_t i = 0; i < items.size(); ++i)
    {
        std::cout << (i + 1) << ". " << items[i].name << std::endl;
    }
    std::cout << std::endl;

    // Demander un choix à l'utilisateur
    int choice = getIntInput("Votre choix (" + std::to_string(1) + "-" + std::to_string(items.size()) + "): ", 1, items.size());

    // Exécuter l'action associée au choix
    if (choice > 0 && static_cast<size_t>(choice) <= items.size())
    {
        // Assurons-nous que l'action est appelée
        const auto &selectedItem = items[choice - 1];
        if (selectedItem.action)
        {
            try
            {
                selectedItem.action();
            }
            catch (const std::exception &e)
            {
                displayError("Erreur lors de l'exécution de l'action: " + std::string(e.what()));
                waitForKeyPress();
            }
        }
    }

    return choice - 1; // Return 0-based index
}

// Implementation of the string vector overload
int ConsoleUI::displayMenu(const std::string &title, const std::vector<std::string> &options)
{
    clearScreen();
    displayTitle(title);

    // Afficher les options du menu
    for (size_t i = 0; i < options.size(); ++i)
    {
        std::cout << (i + 1) << ". " << options[i] << std::endl;
    }
    std::cout << std::endl;

    // Demander un choix à l'utilisateur
    int choice = getIntInput("Votre choix (" + std::to_string(1) + "-" + std::to_string(options.size()) + "): ", 1, options.size());

    // Return 0-based index
    return choice - 1;
}

std::string ConsoleUI::getInput(const std::string &prompt)
{
    std::cout << prompt;

    std::string input;

    // Vérifier si le buffer d'entrée contient déjà un caractère de nouvelle ligne
    if (std::cin.peek() == '\n')
    {
        std::cin.ignore();
    }

    std::getline(std::cin, input);

    return input;
}

int ConsoleUI::getIntInput(const std::string &prompt, int min, int max)
{
    std::cout << prompt;

    int input;
    std::cin >> input;

    // Vider le buffer d'entrée
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Validation de l'entrée
    while (std::cin.fail() || input < min || input > max)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrée invalide. Veuillez entrer un nombre entre "
                  << min << " et " << max << ": ";
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return input;
}

void ConsoleUI::waitForKeyPress()
{
    std::cout << std::endl
              << "Appuyez sur Entrée pour continuer...";

    // Vérifier si le buffer d'entrée contient déjà un caractère de nouvelle ligne
    if (std::cin.peek() == '\n')
    {
        std::cin.ignore();
    }

    std::cin.get();
}