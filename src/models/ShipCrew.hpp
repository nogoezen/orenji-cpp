#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

// Structure pour représenter un membre d'équipage
struct CrewMember {
    int id;
    std::string name;
    std::string specialty;  // Navigation, Commerce, Combat, etc.
    int level;
    int experience;
    int salary;
    int morale;
    
    // Compétences
    int navigation;
    int commerce;
    int combat;
    int diplomatie;
    
    CrewMember(int _id, const std::string& _name, const std::string& _specialty, 
              int _level = 1, int _exp = 0, int _salary = 50, int _morale = 100,
              int _nav = 1, int _com = 1, int _comb = 1, int _dip = 1)
        : id(_id), name(_name), specialty(_specialty), level(_level), experience(_exp),
          salary(_salary), morale(_morale), navigation(_nav), commerce(_com),
          combat(_comb), diplomatie(_dip) {}
    
    // Constructeur depuis JSON
    CrewMember(const nlohmann::json& data)
        : id(data["id"]), name(data["name"]), specialty(data["specialty"]),
          level(data["level"]), experience(data["experience"]), salary(data["salary"]),
          morale(data["morale"]), navigation(data["navigation"]), commerce(data["commerce"]),
          combat(data["combat"]), diplomatie(data["diplomatie"]) {}
    
    // Ajouter de l'expérience et monter de niveau si nécessaire
    void addExperience(int amount) {
        experience += amount;
        // Calcul simplifié du niveau basé sur l'expérience
        int newLevel = 1 + (experience / 1000);
        
        // Si le membre monte de niveau
        if (newLevel > level) {
            int levelsGained = newLevel - level;
            level = newLevel;
            
            // Augmenter les compétences en fonction de la spécialité
            if (specialty == "Navigation") {
                navigation += levelsGained * 2;
                commerce += levelsGained;
                combat += levelsGained;
            } else if (specialty == "Commerce") {
                navigation += levelsGained;
                commerce += levelsGained * 2;
                diplomatie += levelsGained;
            } else if (specialty == "Combat") {
                navigation += levelsGained;
                combat += levelsGained * 2;
                diplomatie += levelsGained;
            } else {
                // Spécialité générale
                navigation += levelsGained;
                commerce += levelsGained;
                combat += levelsGained;
                diplomatie += levelsGained;
            }
            
            // Mettre à jour le salaire
            salary = 50 + (level * 25);
        }
    }
};

// Classe pour gérer l'équipage d'un navire
class ShipCrew {
private:
    std::vector<CrewMember> m_crew;
    int m_maxSize;
    int m_captain;  // ID du capitaine (généralement le joueur)

public:
    ShipCrew(int maxSize = 20, int captain = 0) : m_maxSize(maxSize), m_captain(captain) {}
    
    // Ajouter un membre d'équipage
    bool addCrewMember(const CrewMember& member) {
        if (m_crew.size() >= m_maxSize) {
            return false;  // Équipage complet
        }
        
        // Vérifier que l'ID n'existe pas déjà
        for (const auto& m : m_crew) {
            if (m.id == member.id) {
                return false;  // Membre déjà présent
            }
        }
        
        m_crew.push_back(member);
        return true;
    }
    
    // Supprimer un membre d'équipage par ID
    bool removeCrewMember(int id) {
        for (auto it = m_crew.begin(); it != m_crew.end(); ++it) {
            if (it->id == id) {
                m_crew.erase(it);
                return true;
            }
        }
        return false;
    }
    
    // Obtenir le nombre total de membres d'équipage
    int getSize() const { return m_crew.size(); }
    
    // Obtenir la taille maximale de l'équipage
    int getMaxSize() const { return m_maxSize; }
    
    // Définir la taille maximale de l'équipage (en cas de changement de navire)
    void setMaxSize(int size) { m_maxSize = size; }
    
    // Obtenir un membre d'équipage par ID
    CrewMember* getCrewMemberById(int id) {
        for (auto& member : m_crew) {
            if (member.id == id) {
                return &member;
            }
        }
        return nullptr;
    }
    
    // Obtenir le capitaine
    int getCaptainId() const { return m_captain; }
    
    // Définir le capitaine
    void setCaptainId(int id) { m_captain = id; }
    
    // Obtenir la compétence totale de l'équipage dans un domaine
    int getTotalSkill(const std::string& skill) const {
        int total = 0;
        
        for (const auto& member : m_crew) {
            if (skill == "Navigation") {
                total += member.navigation;
            } else if (skill == "Commerce") {
                total += member.commerce;
            } else if (skill == "Combat") {
                total += member.combat;
            } else if (skill == "Diplomatie") {
                total += member.diplomatie;
            }
        }
        
        return total;
    }
    
    // Payer les salaires de l'équipage
    int payWages() {
        int totalWages = 0;
        
        for (auto& member : m_crew) {
            totalWages += member.salary;
            
            // Augmenter le moral car le salaire est payé
            member.morale = std::min(100, member.morale + 10);
        }
        
        return totalWages;
    }
    
    // Réduire le moral en cas de non-paiement
    void reduceMorale(int amount = 10) {
        for (auto& member : m_crew) {
            member.morale = std::max(0, member.morale - amount);
        }
    }
    
    // Obtenir la liste des membres d'équipage
    const std::vector<CrewMember>& getCrew() const { return m_crew; }
}; 