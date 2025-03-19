#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>
#include "../models/Character.h"
#include "../ui/GuiUI.h"

class CharacterEditor
{
private:
    GuiUI &m_ui;
    std::shared_ptr<Character> m_character;
    nlohmann::json m_classesData;

    // Caractéristiques modifiables
    int m_availablePoints;
    std::map<std::string, int> m_attributes;
    std::vector<std::string> m_skills;
    std::vector<std::string> m_traits;
    std::vector<std::string> m_backgrounds;

    // Méthodes internes
    void displayCharacterInfo();
    void modifyAttributes();
    void selectSkills();
    void selectTraits();
    void selectBackground();
    void randomizeCharacter();
    void confirmCharacter();

    // Helpers
    std::string getAttributeDescription(const std::string &attribute);
    std::string getSkillDescription(const std::string &skill);
    std::string getTraitDescription(const std::string &trait);
    std::string getBackgroundDescription(const std::string &background);

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
    CharacterEditor(GuiUI &ui) : m_ui(ui), m_character(nullptr), m_availablePoints(10)
    {
        // Charger les données des classes depuis un fichier JSON
        // Si GameData n'a pas de méthode getClasses(), initialisons simplement un JSON vide
        m_classesData = nlohmann::json::object();
    }

    // Démarre l'éditeur de personnage et retourne le personnage créé
    std::shared_ptr<Character> startEditor(const std::string &playerName);

    // Obtenir le personnage créé
    std::shared_ptr<Character> getCharacter() const { return m_character; }

    std::shared_ptr<Character> createCharacter();
    void editCharacter(std::shared_ptr<Character> character);
};