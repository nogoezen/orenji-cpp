#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>
#include "../models/Character.h"
#include "ConsoleUI.h"
#include "GameData.h"

class CharacterEditor
{
private:
    ConsoleUI &m_ui;
    std::shared_ptr<Character> m_character;
    nlohmann::json m_classesData;

    // Affiche les informations complètes d'un personnage
    void displayCharacterDetails(const Character &character) const;

    // Affiche les informations sur une classe
    void displayClassDetails(int classId) const;

    // Obtient le nom d'une classe à partir de son ID
    std::string getClassName(int classId) const;

    // Interface pour choisir un personnage prédéfini
    std::shared_ptr<Character> chooseCharacterTemplate() const;

    // Interface pour modifier le nom du personnage
    void editCharacterName();

    // Interface pour choisir la classe principale
    void chooseMainClass();

    // Interface pour choisir les classes secondaires
    void chooseSecondaryClasses();

    // Interface pour attribuer manuellement les points de caractéristiques
    void assignAttributePoints();

public:
    CharacterEditor(ConsoleUI &ui) : m_ui(ui), m_character(nullptr)
    {
        // Charger les données des classes
        m_classesData = GameData::getInstance().getClasses();
    }

    // Démarre l'éditeur de personnage et retourne le personnage créé
    std::shared_ptr<Character> startEditor(const std::string &playerName);

    // Obtenir le personnage créé
    std::shared_ptr<Character> getCharacter() const { return m_character; }
};