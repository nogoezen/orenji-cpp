#include "CharacterCreationMenu.h"
#include "../core/Game.h"
#include "../models/Player.h"
#include "../models/Character.h"
#include "../data/GameData.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <limits>

CharacterCreationMenu::CharacterCreationMenu(Game *game) : m_game(game)
{
    // Charger les données nécessaires à la création de personnage
    if (!loadData())
    {
        std::cerr << "Erreur lors du chargement des données de création de personnage!" << std::endl;
    }
}

bool CharacterCreationMenu::run()
{
    // Si les données n'ont pas été chargées correctement, on ne peut pas créer de personnage
    if (m_actors.empty() || m_classes.empty() || m_kingdoms.empty())
    {
        std::cerr << "Impossible de créer un personnage: données manquantes!" << std::endl;
        return false;
    }

    std::string playerName;
    int actorId = -1;
    int classId = -1;
    int kingdomId = -1;
    char choice;

    // Nom du joueur
    std::cout << "=== CRÉATION DE PERSONNAGE ===" << std::endl;
    std::cout << "Entrez votre nom: ";
    std::cin >> playerName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Sélection de l'archétype (Acteur)
    bool actorSelected = false;
    while (!actorSelected)
    {
        displayActorSelection();
        std::cout << "Choisissez votre archétype (1-" << m_actors.size() << ") ou 0 pour annuler: ";
        int index;
        std::cin >> index;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (index == 0)
        {
            return false;
        }

        if (index > 0 && index <= static_cast<int>(m_actors.size()))
        {
            actorId = m_actors[index - 1].id;
            actorSelected = true;
        }
        else
        {
            std::cout << "Choix invalide. Réessayez." << std::endl;
        }
    }

    // Sélection de la classe
    bool classSelected = false;
    while (!classSelected)
    {
        displayClassSelection();
        std::cout << "Choisissez votre classe (1-" << m_classes.size() << ") ou 0 pour annuler: ";
        int index;
        std::cin >> index;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (index == 0)
        {
            return false;
        }

        if (index > 0 && index <= static_cast<int>(m_classes.size()))
        {
            classId = m_classes[index - 1].id;
            classSelected = true;
        }
        else
        {
            std::cout << "Choix invalide. Réessayez." << std::endl;
        }
    }

    // Sélection du royaume d'origine
    bool kingdomSelected = false;
    while (!kingdomSelected)
    {
        displayKingdomSelection();
        std::cout << "Choisissez votre royaume d'origine (1-" << m_kingdoms.size() << ") ou 0 pour annuler: ";
        int index;
        std::cin >> index;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (index == 0)
        {
            return false;
        }

        if (index > 0 && index <= static_cast<int>(m_kingdoms.size()))
        {
            kingdomId = m_kingdoms[index - 1].id;
            kingdomSelected = true;
        }
        else
        {
            std::cout << "Choix invalide. Réessayez." << std::endl;
        }
    }

    // Confirmation de la création du personnage
    displayCharacterSummary(playerName, actorId, classId, kingdomId);
    std::cout << "Confirmer la création de ce personnage? (O/N): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'O' || choice == 'o')
    {
        // Créer le personnage et l'assigner au joueur
        auto character = createCharacter(playerName, actorId, classId, kingdomId);
        auto player = std::make_shared<Player>(playerName, character);
        m_game->getPlayer() = player;
        return true;
    }

    return false;
}

bool CharacterCreationMenu::loadData()
{
    try
    {
        // Charger les données des acteurs
        std::ifstream actorFile("bin/data/Actors.json");
        if (!actorFile.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier Actors.json" << std::endl;
            return false;
        }
        nlohmann::json actorJson;
        actorFile >> actorJson;

        for (const auto &actor : actorJson)
        {
            ActorData data;
            data.id = actor["id"].get<int>();
            data.name = actor["name"].get<std::string>();
            data.description = actor["profile"].get<std::string>();

            // Attributs du personnage
            if (actor.contains("attributes") && actor["attributes"].is_object())
            {
                for (const auto &[key, value] : actor["attributes"].items())
                {
                    data.attributes[key] = value.get<int>();
                }
            }

            // Compétences
            if (actor.contains("skills") && actor["skills"].is_array())
            {
                for (const auto &skill : actor["skills"])
                {
                    data.skills.push_back(skill.get<std::string>());
                }
            }

            m_actors.push_back(data);
        }

        // Charger les données des classes
        std::ifstream classFile("bin/data/Classes.json");
        if (!classFile.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier Classes.json" << std::endl;
            return false;
        }
        nlohmann::json classJson;
        classFile >> classJson;

        for (const auto &classObj : classJson)
        {
            ClassData data;
            data.id = classObj["id"].get<int>();
            data.name = classObj["name"].get<std::string>();
            data.description = classObj["description"].get<std::string>();

            // Bonus de classe
            if (classObj.contains("bonuses") && classObj["bonuses"].is_object())
            {
                for (const auto &[key, value] : classObj["bonuses"].items())
                {
                    data.bonuses[key] = value.get<int>();
                }
            }

            // Compétences de classe
            if (classObj.contains("skills") && classObj["skills"].is_array())
            {
                for (const auto &skill : classObj["skills"])
                {
                    data.skills.push_back(skill.get<std::string>());
                }
            }

            m_classes.push_back(data);
        }

        // Charger les données des royaumes
        std::ifstream kingdomFile("bin/data/Kingdoms.json");
        if (!kingdomFile.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier Kingdoms.json" << std::endl;
            return false;
        }
        nlohmann::json kingdomJson;
        kingdomFile >> kingdomJson;

        for (const auto &kingdom : kingdomJson)
        {
            KingdomData data;
            data.id = kingdom["id"].get<int>();
            data.name = kingdom["name"].get<std::string>();
            data.description = kingdom["description"].get<std::string>();

            // Navires disponibles
            if (kingdom.contains("ships") && kingdom["ships"].is_array())
            {
                for (const auto &ship : kingdom["ships"])
                {
                    data.ships.push_back(ship.get<std::string>());
                }
            }

            // Modificateurs de relation avec d'autres royaumes
            if (kingdom.contains("relations") && kingdom["relations"].is_object())
            {
                for (const auto &[key, value] : kingdom["relations"].items())
                {
                    data.relationModifiers[key] = value.get<int>();
                }
            }

            m_kingdoms.push_back(data);
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement des données: " << e.what() << std::endl;
        return false;
    }
}

void CharacterCreationMenu::displayActorSelection()
{
    std::cout << "\n=== CHOIX DE L'ARCHÉTYPE ===" << std::endl;

    for (size_t i = 0; i < m_actors.size(); i++)
    {
        std::cout << (i + 1) << ". " << m_actors[i].name << std::endl;
        std::cout << "   " << m_actors[i].description << std::endl;

        if (!m_actors[i].skills.empty())
        {
            std::cout << "   Compétences: ";
            for (size_t j = 0; j < m_actors[i].skills.size(); j++)
            {
                std::cout << m_actors[i].skills[j];
                if (j < m_actors[i].skills.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

void CharacterCreationMenu::displayClassSelection()
{
    std::cout << "\n=== CHOIX DE LA CLASSE ===" << std::endl;

    for (size_t i = 0; i < m_classes.size(); i++)
    {
        std::cout << (i + 1) << ". " << m_classes[i].name << std::endl;
        std::cout << "   " << m_classes[i].description << std::endl;

        if (!m_classes[i].bonuses.empty())
        {
            std::cout << "   Bonus: ";
            bool first = true;
            for (const auto &[attr, bonus] : m_classes[i].bonuses)
            {
                if (!first)
                {
                    std::cout << ", ";
                }
                std::cout << attr << " +" << bonus;
                first = false;
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

void CharacterCreationMenu::displayKingdomSelection()
{
    std::cout << "\n=== CHOIX DU ROYAUME D'ORIGINE ===" << std::endl;

    for (size_t i = 0; i < m_kingdoms.size(); i++)
    {
        std::cout << (i + 1) << ". " << m_kingdoms[i].name << std::endl;
        std::cout << "   " << m_kingdoms[i].description << std::endl;

        if (!m_kingdoms[i].ships.empty())
        {
            std::cout << "   Navires disponibles: ";
            for (size_t j = 0; j < m_kingdoms[i].ships.size(); j++)
            {
                std::cout << m_kingdoms[i].ships[j];
                if (j < m_kingdoms[i].ships.size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

void CharacterCreationMenu::displayCharacterSummary(const std::string &name, int actorId, int classId, int kingdomId)
{
    std::cout << "\n=== RÉSUMÉ DU PERSONNAGE ===" << std::endl;
    std::cout << "Nom: " << name << std::endl;

    // Trouver les détails de l'acteur sélectionné
    const ActorData *selectedActor = nullptr;
    for (const auto &actor : m_actors)
    {
        if (actor.id == actorId)
        {
            selectedActor = &actor;
            break;
        }
    }

    // Trouver les détails de la classe sélectionnée
    const ClassData *selectedClass = nullptr;
    for (const auto &cls : m_classes)
    {
        if (cls.id == classId)
        {
            selectedClass = &cls;
            break;
        }
    }

    // Trouver les détails du royaume sélectionné
    const KingdomData *selectedKingdom = nullptr;
    for (const auto &kingdom : m_kingdoms)
    {
        if (kingdom.id == kingdomId)
        {
            selectedKingdom = &kingdom;
            break;
        }
    }

    if (selectedActor)
    {
        std::cout << "Archétype: " << selectedActor->name << std::endl;
    }

    if (selectedClass)
    {
        std::cout << "Classe: " << selectedClass->name << std::endl;
    }

    if (selectedKingdom)
    {
        std::cout << "Royaume d'origine: " << selectedKingdom->name << std::endl;
    }

    std::cout << "\nAttributs initiaux:" << std::endl;

    // Calculer les attributs en combinant ceux de l'acteur et les bonus de classe
    std::map<std::string, int> attributes;
    if (selectedActor)
    {
        attributes = selectedActor->attributes;
    }

    if (selectedClass)
    {
        for (const auto &[attr, bonus] : selectedClass->bonuses)
        {
            attributes[attr] += bonus;
        }
    }

    // Afficher les attributs
    if (!attributes.empty())
    {
        for (const auto &[attr, value] : attributes)
        {
            std::cout << "   " << attr << ": " << value << std::endl;
        }
    }
    else
    {
        std::cout << "   Attributs par défaut" << std::endl;
    }

    std::cout << std::endl;
}

std::shared_ptr<Character> CharacterCreationMenu::createCharacter(const std::string &name, int actorId, int classId, int kingdomId)
{
    // Trouver les données pour les sélections
    const ActorData *selectedActor = nullptr;
    for (const auto &actor : m_actors)
    {
        if (actor.id == actorId)
        {
            selectedActor = &actor;
            break;
        }
    }

    const ClassData *selectedClass = nullptr;
    for (const auto &cls : m_classes)
    {
        if (cls.id == classId)
        {
            selectedClass = &cls;
            break;
        }
    }

    const KingdomData *selectedKingdom = nullptr;
    for (const auto &kingdom : m_kingdoms)
    {
        if (kingdom.id == kingdomId)
        {
            selectedKingdom = &kingdom;
            break;
        }
    }

    // Créer un nouveau personnage
    auto character = std::make_shared<Character>(name,
                                                 selectedClass ? selectedClass->name : "Novice");

    // Définir les attributs de base
    character->setOrigin(selectedKingdom ? selectedKingdom->name : "Inconnu");

    // Description générée
    std::string description = "Un ";
    description += selectedClass ? selectedClass->name : "aventurier";
    description += " originaire ";
    description += selectedKingdom ? selectedKingdom->name : "de terres inconnues";
    description += ".";
    character->setDescription(description);

    // Appliquer les attributs de l'archétype
    if (selectedActor)
    {
        // Attributs
        for (const auto &[attr, value] : selectedActor->attributes)
        {
            if (attr == "navigation")
                character->setNavigation(value);
            else if (attr == "commerce")
                character->setCommerce(value);
            else if (attr == "diplomacy")
                character->setDiplomacy(value);
            else if (attr == "combat")
                character->setCombat(value);
            else if (attr == "leadership")
                character->setLeadership(value);
        }

        // Compétences
        for (const auto &skill : selectedActor->skills)
        {
            character->addSkill(skill);
        }
    }

    // Appliquer les bonus de classe
    if (selectedClass)
    {
        for (const auto &[attr, bonus] : selectedClass->bonuses)
        {
            if (attr == "navigation")
                character->setNavigation(character->getNavigation() + bonus);
            else if (attr == "commerce")
                character->setCommerce(character->getCommerce() + bonus);
            else if (attr == "diplomacy")
                character->setDiplomacy(character->getDiplomacy() + bonus);
            else if (attr == "combat")
                character->setCombat(character->getCombat() + bonus);
            else if (attr == "leadership")
                character->setLeadership(character->getLeadership() + bonus);
        }

        // Compétences de classe
        for (const auto &skill : selectedClass->skills)
        {
            character->addSkill(skill);
        }
    }

    return character;
}