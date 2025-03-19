#include "ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

// Efface l'écran
void ConsoleUI::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Affiche un titre centré
void ConsoleUI::displayTitle(const std::string &title)
{
    clearScreen();

    const int width = 80;
    const int padding = (width - title.length()) / 2;

    drawLine(width);
    std::cout << std::setw(padding) << "" << title << std::endl;
    drawLine(width);
    std::cout << std::endl;
}

// Trace une ligne horizontale
void ConsoleUI::drawLine(int width, char symbol)
{
    std::cout << std::string(width, symbol) << std::endl;
}

// Attendre l'entrée utilisateur
void ConsoleUI::waitForEnter(const std::string &message)
{
    std::cout << std::endl
              << message;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Affiche un menu et retourne la sélection de l'utilisateur
int ConsoleUI::displayMenu(const std::string &title, const std::vector<MenuItem> &items)
{
    if (items.empty())
    {
        return -1;
    }

    clearScreen();
    displayTitle(title);

    // Afficher les options
    for (size_t i = 0; i < items.size(); i++)
    {
        const auto &item = items[i];
        if (item.enabled)
        {
            std::cout << i + 1 << ". " << item.label << std::endl;
        }
        else
        {
            std::cout << i + 1 << ". " << item.label << " (désactivé)" << std::endl;
        }
    }

    std::cout << std::endl;

    // Obtenir la sélection de l'utilisateur
    int selection = getUserInput<int>(
        "Entrez votre choix: ",
        [&items](const int &choice)
        {
            return choice >= 1 && choice <= static_cast<int>(items.size()) &&
                   items[choice - 1].enabled;
        },
        "Choix invalide. Veuillez entrer un numéro valide.");

    // Exécuter l'action associée
    if (selection >= 1 && selection <= static_cast<int>(items.size()))
    {
        const auto &item = items[selection - 1];
        if (item.enabled && item.action)
        {
            item.action();
        }
    }

    return selection;
}

// Afficher un message d'erreur
void ConsoleUI::displayError(const std::string &message)
{
    std::cout << std::endl
              << "ERREUR: " << message << std::endl;
}

// Afficher une liste numérotée
void ConsoleUI::displayNumberedList(const std::vector<std::string> &items, const std::string &title)
{
    if (!title.empty())
    {
        std::cout << title << ":" << std::endl;
    }

    if (items.empty())
    {
        std::cout << "   Aucun élément" << std::endl;
        return;
    }

    for (size_t i = 0; i < items.size(); i++)
    {
        std::cout << i + 1 << ". " << items[i] << std::endl;
    }
}

// Afficher des attributs
void ConsoleUI::displayAttributes(const std::map<std::string, std::string> &attributes, const std::string &title)
{
    if (!title.empty())
    {
        std::cout << title << ":" << std::endl;
    }

    if (attributes.empty())
    {
        std::cout << "   Aucun attribut" << std::endl;
        return;
    }

    size_t maxKeyLength = 0;
    for (const auto &[key, _] : attributes)
    {
        maxKeyLength = std::max(maxKeyLength, key.length());
    }

    for (const auto &[key, value] : attributes)
    {
        std::cout << "  " << std::setw(maxKeyLength) << std::left << key << " : " << value << std::endl;
    }
}