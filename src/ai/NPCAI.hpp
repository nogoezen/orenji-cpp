#pragma once

#include "../models/World.hpp"
#include "../models/Character.hpp"
#include "../models/City.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

class NPCAI
{
public:
    struct NPCAction
    {
        enum class Type
        {
            TRAVEL,      // Voyage vers une destination
            TRADE,       // Commerce avec une ville
            COMBAT,      // Engagement dans un combat
            EXPLORE,     // Exploration d'une zone
            DIPLOMATIC,  // Action diplomatique
            PERSONAL,    // Action personnelle
            FLEET_MANAGE // Gestion de flotte
        };

        Type type;
        std::string target;                      // Ville, personnage, ou zone cible
        float priority;                          // Priorité de l'action (0.0 - 1.0)
        std::map<std::string, float> parameters; // Paramètres spécifiques à l'action
    };

    struct NPCState
    {
        float health;
        float wealth;
        float reputation;
        float influence;
        std::vector<std::string> relationships; // Relations avec d'autres personnages
        std::map<std::string, float> skills;    // Compétences du personnage
    };

    NPCAI(std::shared_ptr<World> world);
    ~NPCAI() = default;

    // Méthodes principales
    void update(float deltaTime);
    void updateNPC(const std::string &npcId);
    std::vector<NPCAction> evaluateActions(const std::string &npcId);
    bool executeAction(const std::string &npcId, const NPCAction &action);

    // Méthodes de gestion des NPCs
    void updateNPCState(const std::string &npcId);
    void updateRelationships(const std::string &npcId);
    void updateSkills(const std::string &npcId);

    // Méthodes de décision
    NPCAction selectBestAction(const std::string &npcId);
    float calculateActionPriority(const std::string &npcId, const NPCAction &action);
    bool shouldExecuteAction(const std::string &npcId, const NPCAction &action);

    // Méthodes de comportement
    void updateTravelBehavior(const std::string &npcId);
    void updateTradeBehavior(const std::string &npcId);
    void updateCombatBehavior(const std::string &npcId);
    void updateExplorationBehavior(const std::string &npcId);
    void updateDiplomaticBehavior(const std::string &npcId);
    void updatePersonalBehavior(const std::string &npcId);
    void updateFleetBehavior(const std::string &npcId);

private:
    std::shared_ptr<World> m_world;
    std::map<std::string, NPCState> m_npcStates;
    std::map<std::string, std::vector<NPCAction>> m_npcActions;

    // Cache pour les calculs
    std::map<std::string, float> m_actionPriorityCache;
    std::map<std::string, std::vector<std::string>> m_relationshipCache;

    // Méthodes utilitaires
    float calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2);
    float calculateTravelCost(const std::string &npcId, const std::string &destination);
    float calculateTradeValue(const std::string &npcId, const std::string &city);
    float calculateCombatStrength(const std::string &npcId);
    float calculateExplorationValue(const std::string &npcId, const std::string &area);
    float calculateDiplomaticValue(const std::string &npcId, const std::string &target);
    void updateNPCStats(const std::string &npcId);
};
