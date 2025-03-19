#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <memory>

/**
 * @brief Classe représentant un personnage joueur
 *
 * Cette classe contient les attributs et méthodes liés au personnage du joueur,
 * comme son nom, sa classe, ses compétences, etc.
 */
class Character
{
private:
    int m_id;
    std::string m_name;
    std::string m_nickname;
    std::string m_description;
    std::string m_class;  // Classe principale (Navigateur, Marchand, etc.)
    std::string m_origin; // Royaume d'origine

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
    int m_level = 1;
    int m_maxLevel;
    int m_experience = 0;
    int m_health = 100;
    int m_maxHealth = 100;
    int m_navigation = 10; // Compétence de navigation
    int m_commerce = 10;   // Compétence de commerce
    int m_diplomacy = 10;  // Compétence de diplomatie
    int m_combat = 10;     // Compétence de combat
    int m_leadership = 10; // Compétence de commandement
    std::string m_profile;
    std::string m_characterName;
    int m_characterIndex;
    std::string m_faceName;
    int m_faceIndex;
    std::vector<int> m_params;
    std::vector<int> m_equips;
    std::vector<std::string> m_skills;

public:
    /**
     * Constructeur par défaut
     */
    Character();

    /**
     * Constructeur avec paramètres de base
     * @param name Nom du personnage
     * @param characterClass Classe du personnage
     */
    Character(const std::string &name, const std::string &characterClass);

    /**
     * Destructeur
     */
    ~Character() = default;

    /**
     * Conversion en JSON pour sauvegarde
     */
    nlohmann::json toJson() const;

    /**
     * Chargement depuis JSON
     */
    static std::shared_ptr<Character> fromJson(const nlohmann::json &data);

    // Accesseurs
    int getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    const std::string &getNickname() const { return m_nickname; }
    const std::string &getDescription() const { return m_description; }
    const std::string &getCharacterClass() const { return m_class; }
    const std::string &getOrigin() const { return m_origin; }
    const Title &getTitle() const { return m_title; }
    const Morality &getMorality() const { return m_morality; }
    int getClassId() const { return m_classId; }
    const std::vector<int> &getSecondaryClasses() const { return m_secondaryClasses; }
    std::vector<int> &getSecondaryClasses() { return m_secondaryClasses; }
    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    int getMaxLevel() const { return m_maxLevel; }
    const std::string &getProfile() const { return m_profile; }
    const std::string &getCharacterName() const { return m_characterName; }
    int getCharacterIndex() const { return m_characterIndex; }
    const std::string &getFaceName() const { return m_faceName; }
    int getFaceIndex() const { return m_faceIndex; }
    const std::vector<int> &getParams() const { return m_params; }
    const std::vector<int> &getEquips() const { return m_equips; }
    int getNavigation() const { return m_navigation; }
    int getCommerce() const { return m_commerce; }
    int getDiplomacy() const { return m_diplomacy; }
    int getCombat() const { return m_combat; }
    int getLeadership() const { return m_leadership; }
    const std::vector<std::string> &getSkills() const { return m_skills; }

    // Mutateurs
    void setId(int id) { m_id = id; }
    void setName(const std::string &name) { m_name = name; }
    void setNickname(const std::string &nickname) { m_nickname = nickname; }
    void setDescription(const std::string &description) { m_description = description; }
    void setClass(const std::string &characterClass) { m_class = characterClass; }
    void setOrigin(const std::string &origin) { m_origin = origin; }
    void setClassId(int classId) { m_classId = classId; }
    void addSecondaryClass(int classId) { m_secondaryClasses.push_back(classId); }
    void clearSecondaryClasses() { m_secondaryClasses.clear(); }
    void setLevel(int level) { m_level = level; }
    void setExperience(int experience) { m_experience = experience; }
    void setHealth(int health) { m_health = health; }
    void setMaxHealth(int maxHealth) { m_maxHealth = maxHealth; }
    void setProfile(const std::string &profile) { m_profile = profile; }
    void setCharacterName(const std::string &characterName) { m_characterName = characterName; }
    void setCharacterIndex(int characterIndex) { m_characterIndex = characterIndex; }
    void setFaceName(const std::string &faceName) { m_faceName = faceName; }
    void setFaceIndex(int faceIndex) { m_faceIndex = faceIndex; }
    void setNavigation(int navigation) { m_navigation = navigation; }
    void setCommerce(int commerce) { m_commerce = commerce; }
    void setDiplomacy(int diplomacy) { m_diplomacy = diplomacy; }
    void setCombat(int combat) { m_combat = combat; }
    void setLeadership(int leadership) { m_leadership = leadership; }

    // Méthodes
    void addSkill(const std::string &skill) { m_skills.push_back(skill); }
    bool hasSkill(const std::string &skill) const { return std::find(m_skills.begin(), m_skills.end(), skill) != m_skills.end(); }
    void addExperience(int experience);
    void levelUp();

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