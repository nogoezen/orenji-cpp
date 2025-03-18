#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

class Character
{
private:
    int m_id;
    std::string m_name;
    std::string m_nickname;

    struct Title
    {
        std::string current;
        int rank;
        int points;
        int nextRankRequirement;
    } m_title;

    struct Morality
    {
        int value;
        std::string alignment;
        std::map<std::string, int> reputation;
    } m_morality;

    int m_classId;
    std::vector<int> m_secondaryClasses;
    int m_level;
    int m_maxLevel;
    std::string m_profile;
    std::string m_characterName;
    int m_characterIndex;
    std::string m_faceName;
    int m_faceIndex;
    std::vector<int> m_params;
    std::vector<int> m_equips;

public:
    // Constructeur par défaut
    Character() : m_id(0), m_classId(0), m_level(1), m_maxLevel(99) {}

    // Constructeur à partir de données JSON
    Character(const nlohmann::json &actorData)
    {
        fromJson(actorData);
    }

    // Chargement depuis JSON
    void fromJson(const nlohmann::json &actorData)
    {
        m_id = actorData["id"].get<int>();
        m_name = actorData["name"].get<std::string>();
        m_nickname = actorData["nickname"].get<std::string>();

        // Chargement du titre
        m_title.current = actorData["title"]["current"].get<std::string>();
        m_title.rank = actorData["title"]["rank"].get<int>();
        m_title.points = actorData["title"]["points"].get<int>();
        m_title.nextRankRequirement = actorData["title"]["nextRankRequirement"].get<int>();

        // Chargement de la moralité
        m_morality.value = actorData["morality"]["value"].get<int>();
        m_morality.alignment = actorData["morality"]["alignment"].get<std::string>();

        // Chargement des réputations
        for (auto &[nation, rep] : actorData["morality"]["reputation"].items())
        {
            m_morality.reputation[nation] = rep.get<int>();
        }

        m_classId = actorData["classId"].get<int>();

        // Chargement des classes secondaires
        for (const auto &cls : actorData["secondaryClasses"])
        {
            m_secondaryClasses.push_back(cls.get<int>());
        }

        m_level = actorData["initialLevel"].get<int>();
        m_maxLevel = actorData["maxLevel"].get<int>();
        m_profile = actorData["profile"].get<std::string>();
        m_characterName = actorData["characterName"].get<std::string>();
        m_characterIndex = actorData["characterIndex"].get<int>();
        m_faceName = actorData["faceName"].get<std::string>();
        m_faceIndex = actorData["faceIndex"].get<int>();

        // Chargement des paramètres
        for (const auto &param : actorData["params"])
        {
            m_params.push_back(param.get<int>());
        }

        // Chargement des équipements
        for (const auto &equip : actorData["equips"])
        {
            m_equips.push_back(equip.get<int>());
        }
    }

    // Sauvegarde en JSON
    nlohmann::json toJson() const
    {
        nlohmann::json data;

        data["id"] = m_id;
        data["name"] = m_name;
        data["nickname"] = m_nickname;

        // Sauvegarde du titre
        data["title"]["current"] = m_title.current;
        data["title"]["rank"] = m_title.rank;
        data["title"]["points"] = m_title.points;
        data["title"]["nextRankRequirement"] = m_title.nextRankRequirement;

        // Sauvegarde de la moralité
        data["morality"]["value"] = m_morality.value;
        data["morality"]["alignment"] = m_morality.alignment;

        // Sauvegarde des réputations
        for (const auto &[nation, rep] : m_morality.reputation)
        {
            data["morality"]["reputation"][nation] = rep;
        }

        data["classId"] = m_classId;
        data["secondaryClasses"] = m_secondaryClasses;
        data["initialLevel"] = m_level;
        data["maxLevel"] = m_maxLevel;
        data["profile"] = m_profile;
        data["characterName"] = m_characterName;
        data["characterIndex"] = m_characterIndex;
        data["faceName"] = m_faceName;
        data["faceIndex"] = m_faceIndex;
        data["params"] = m_params;
        data["equips"] = m_equips;

        return data;
    }

    // Getters
    int getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    const std::string &getNickname() const { return m_nickname; }
    const Title &getTitle() const { return m_title; }
    const Morality &getMorality() const { return m_morality; }
    int getClassId() const { return m_classId; }
    const std::vector<int> &getSecondaryClasses() const { return m_secondaryClasses; }
    std::vector<int> &getSecondaryClasses() { return m_secondaryClasses; }
    int getLevel() const { return m_level; }
    int getMaxLevel() const { return m_maxLevel; }
    const std::string &getProfile() const { return m_profile; }
    const std::string &getCharacterName() const { return m_characterName; }
    int getCharacterIndex() const { return m_characterIndex; }
    const std::string &getFaceName() const { return m_faceName; }
    int getFaceIndex() const { return m_faceIndex; }
    const std::vector<int> &getParams() const { return m_params; }
    const std::vector<int> &getEquips() const { return m_equips; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const std::string &name) { m_name = name; }
    void setNickname(const std::string &nickname) { m_nickname = nickname; }
    void setClassId(int classId) { m_classId = classId; }
    void addSecondaryClass(int classId) { m_secondaryClasses.push_back(classId); }
    void clearSecondaryClasses() { m_secondaryClasses.clear(); }
    void setLevel(int level) { m_level = level; }
    void setProfile(const std::string &profile) { m_profile = profile; }

    // Méthodes pour définir les paramètres
    void setParam(int index, int value)
    {
        if (index >= 0 && static_cast<size_t>(index) < m_params.size())
        {
            m_params[index] = value;
        }
        else if (index >= 0 && index < 8)
        {
            // Si le tableau n'est pas assez grand, l'agrandir
            while (static_cast<size_t>(index) >= m_params.size())
            {
                m_params.push_back(0);
            }
            m_params[index] = value;
        }
    }

    // Obtenir un paramètre spécifique
    int getParam(int index) const
    {
        if (index >= 0 && static_cast<size_t>(index) < m_params.size())
        {
            return m_params[index];
        }
        return 0;
    }
};