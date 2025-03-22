#include "Character.hpp"
#include <algorithm>
#include <stdexcept>

namespace Orenji
{
    Character::Character() : m_id(0), m_classId(0), m_maxLevel(99), m_level(1),
                             m_experience(0), m_health(100), m_maxHealth(100),
                             m_navigation(10), m_commerce(10), m_diplomacy(10),
                             m_combat(10), m_leadership(10) {}

    Character::Character(const std::string &name)
        : m_id(0), m_classId(0), m_maxLevel(99), m_level(1),
          m_experience(0), m_health(100), m_maxHealth(100),
          m_navigation(10), m_commerce(10), m_diplomacy(10),
          m_combat(10), m_leadership(10), m_name(name), m_class("Aventurier")
    {
        // Construction avec seulement le nom (classe de base "Aventurier")
    }

    Character::Character(const std::string &name, const std::string &characterClass)
        : m_id(0), m_classId(0), m_maxLevel(99), m_level(1),
          m_experience(0), m_health(100), m_maxHealth(100),
          m_navigation(10), m_commerce(10), m_diplomacy(10),
          m_combat(10), m_leadership(10), m_name(name), m_class(characterClass)
    {
        // Construction complète avec tous les paramètres nécessaires
    }

    nlohmann::json Character::toJson() const
    {
        nlohmann::json data;

        // Données de base
        data["id"] = m_id;
        data["name"] = m_name;
        data["nickname"] = m_nickname;
        data["description"] = m_description;
        data["class"] = m_class;
        data["origin"] = m_origin;

        // Attributs principaux
        data["level"] = m_level;
        data["experience"] = m_experience;
        data["health"] = m_health;
        data["maxHealth"] = m_maxHealth;
        data["maxLevel"] = m_maxLevel;

        // Titre
        data["title"]["current"] = m_title.current;
        data["title"]["rank"] = m_title.rank;
        data["title"]["points"] = m_title.points;
        data["title"]["nextRankRequirement"] = m_title.nextRankRequirement;

        // Moralité
        data["morality"]["value"] = m_morality.value;
        data["morality"]["alignment"] = m_morality.alignment;

        // Réputations
        for (const auto &[nation, rep] : m_morality.reputation)
        {
            data["morality"]["reputation"][nation] = rep;
        }

        // Identité et apparence
        data["classId"] = m_classId;
        data["secondaryClasses"] = m_secondaryClasses;
        data["profile"] = m_profile;
        data["characterName"] = m_characterName;
        data["characterIndex"] = m_characterIndex;
        data["faceName"] = m_faceName;
        data["faceIndex"] = m_faceIndex;

        // Compétences
        data["navigation"] = m_navigation;
        data["commerce"] = m_commerce;
        data["diplomacy"] = m_diplomacy;
        data["combat"] = m_combat;
        data["leadership"] = m_leadership;
        data["skills"] = m_skills;

        // Données techniques
        data["params"] = m_params;
        data["equips"] = m_equips;

        return data;
    }

    std::shared_ptr<Character> Character::fromJson(const nlohmann::json &data)
    {
        std::shared_ptr<Character> character = std::make_shared<Character>();

        try
        {
            // Données de base
            if (data.contains("id") && data["id"].is_number())
                character->m_id = data["id"].get<int>();

            if (data.contains("name") && data["name"].is_string())
                character->m_name = data["name"].get<std::string>();

            if (data.contains("nickname") && data["nickname"].is_string())
                character->m_nickname = data["nickname"].get<std::string>();

            if (data.contains("description") && data["description"].is_string())
                character->m_description = data["description"].get<std::string>();

            if (data.contains("class") && data["class"].is_string())
                character->m_class = data["class"].get<std::string>();

            if (data.contains("origin") && data["origin"].is_string())
                character->m_origin = data["origin"].get<std::string>();

            // Attributs principaux
            if (data.contains("level") && data["level"].is_number())
                character->m_level = data["level"].get<int>();

            if (data.contains("experience") && data["experience"].is_number())
                character->m_experience = data["experience"].get<int>();

            if (data.contains("health") && data["health"].is_number())
                character->m_health = data["health"].get<int>();

            if (data.contains("maxHealth") && data["maxHealth"].is_number())
                character->m_maxHealth = data["maxHealth"].get<int>();

            if (data.contains("maxLevel") && data["maxLevel"].is_number())
                character->m_maxLevel = data["maxLevel"].get<int>();

            // Titre
            if (data.contains("title") && data["title"].is_object())
            {
                if (data["title"].contains("current") && data["title"]["current"].is_string())
                    character->m_title.current = data["title"]["current"].get<std::string>();

                if (data["title"].contains("rank") && data["title"]["rank"].is_number())
                    character->m_title.rank = data["title"]["rank"].get<int>();

                if (data["title"].contains("points") && data["title"]["points"].is_number())
                    character->m_title.points = data["title"]["points"].get<int>();

                if (data["title"].contains("nextRankRequirement") && data["title"]["nextRankRequirement"].is_number())
                    character->m_title.nextRankRequirement = data["title"]["nextRankRequirement"].get<int>();
            }

            // Moralité
            if (data.contains("morality") && data["morality"].is_object())
            {
                if (data["morality"].contains("value") && data["morality"]["value"].is_number())
                    character->m_morality.value = data["morality"]["value"].get<int>();

                if (data["morality"].contains("alignment") && data["morality"]["alignment"].is_string())
                    character->m_morality.alignment = data["morality"]["alignment"].get<std::string>();

                // Réputations
                if (data["morality"].contains("reputation") && data["morality"]["reputation"].is_object())
                {
                    for (auto &[nation, rep] : data["morality"]["reputation"].items())
                    {
                        if (rep.is_number())
                            character->m_morality.reputation[nation] = rep.get<int>();
                    }
                }
            }

            // Identité et apparence
            if (data.contains("classId") && data["classId"].is_number())
                character->m_classId = data["classId"].get<int>();

            if (data.contains("secondaryClasses") && data["secondaryClasses"].is_array())
            {
                for (const auto &cls : data["secondaryClasses"])
                {
                    if (cls.is_number())
                        character->m_secondaryClasses.push_back(cls.get<int>());
                }
            }

            if (data.contains("profile") && data["profile"].is_string())
                character->m_profile = data["profile"].get<std::string>();

            if (data.contains("characterName") && data["characterName"].is_string())
                character->m_characterName = data["characterName"].get<std::string>();

            if (data.contains("characterIndex") && data["characterIndex"].is_number())
                character->m_characterIndex = data["characterIndex"].get<int>();

            if (data.contains("faceName") && data["faceName"].is_string())
                character->m_faceName = data["faceName"].get<std::string>();

            if (data.contains("faceIndex") && data["faceIndex"].is_number())
                character->m_faceIndex = data["faceIndex"].get<int>();

            // Compétences
            if (data.contains("navigation") && data["navigation"].is_number())
                character->m_navigation = data["navigation"].get<int>();

            if (data.contains("commerce") && data["commerce"].is_number())
                character->m_commerce = data["commerce"].get<int>();

            if (data.contains("diplomacy") && data["diplomacy"].is_number())
                character->m_diplomacy = data["diplomacy"].get<int>();

            if (data.contains("combat") && data["combat"].is_number())
                character->m_combat = data["combat"].get<int>();

            if (data.contains("leadership") && data["leadership"].is_number())
                character->m_leadership = data["leadership"].get<int>();

            if (data.contains("skills") && data["skills"].is_array())
            {
                for (const auto &skill : data["skills"])
                {
                    if (skill.is_string())
                        character->m_skills.push_back(skill.get<std::string>());
                }
            }

            // Données techniques
            if (data.contains("params") && data["params"].is_array())
            {
                for (const auto &param : data["params"])
                {
                    if (param.is_number())
                        character->m_params.push_back(param.get<int>());
                }
            }

            if (data.contains("equips") && data["equips"].is_array())
            {
                for (const auto &equip : data["equips"])
                {
                    if (equip.is_number())
                        character->m_equips.push_back(equip.get<int>());
                }
            }
        }
        catch (const std::exception &e)
        {
            // En cas d'erreur, on continue mais on affiche un message
            // Dans un vrai jeu, on pourrait logger cette erreur ou alerter l'utilisateur
            character = std::make_shared<Character>();
        }

        return character;
    }

    void Character::addExperience(int experience)
    {
        m_experience += experience;

        // Vérifier si le personnage peut monter de niveau
        // Exemple simple de formule pour déterminer l'expérience nécessaire
        int experienceNeeded = m_level * 100;

        if (m_experience >= experienceNeeded && m_level < m_maxLevel)
        {
            levelUp();
        }
    }

    void Character::levelUp()
    {
        if (m_level < m_maxLevel)
        {
            m_level++;
            m_maxHealth += 10;      // Augmenter les points de vie maximum
            m_health = m_maxHealth; // Restaurer les points de vie

            // Augmenter les compétences (à adapter selon les règles du jeu)
            m_navigation += 2;
            m_commerce += 2;
            m_diplomacy += 2;
            m_combat += 2;
            m_leadership += 2;
        }
    }