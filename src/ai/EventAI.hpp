#pragma once

#include "../models/World.hpp"
#include "../models/City.hpp"
#include "../models/Kingdom.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <random>

class EventAI
{
public:
    struct Event
    {
        enum class Type
        {
            NATURAL_DISASTER,  // Tempêtes, séismes, etc.
            POLITICAL_CRISIS,  // Révoltes, coups d'état
            ECONOMIC_EVENT,    // Crises économiques, booms
            MILITARY_CONFLICT, // Guerres, raids
            CULTURAL_EVENT,    // Festivals, épidémies
            TRADE_EVENT,       // Embargos, pénuries
            DISCOVERY_EVENT,   // Découvertes de nouvelles terres
            PERSONAL_EVENT     // Événements liés au joueur
        };

        Type type;
        std::string title;
        std::string description;
        std::vector<std::string> affectedEntities; // Villes, royaumes, etc.
        float severity;                            // 0.0 - 1.0
        float duration;                            // En jours
        std::map<std::string, float> effects;      // Effets sur différents aspects
    };

    struct EventTrigger
    {
        Type type;
        float probability;
        std::map<std::string, float> conditions;
        std::vector<std::string> prerequisites;
    };

    EventAI(std::shared_ptr<World> world);
    ~EventAI() = default;

    // Méthodes principales
    void update(float deltaTime);
    std::vector<Event> generatePossibleEvents();
    bool triggerEvent(const Event &event);
    void resolveEvent(const Event &event);

    // Méthodes de gestion des événements
    void updateActiveEvents(float deltaTime);
    float calculateEventProbability(const Event &event);
    std::vector<std::string> getAffectedEntities(const Event &event);
    void applyEventEffects(const Event &event);

    // Méthodes de génération d'événements
    Event generateNaturalDisaster();
    Event generatePoliticalCrisis();
    Event generateEconomicEvent();
    Event generateMilitaryConflict();
    Event generateCulturalEvent();
    Event generateTradeEvent();
    Event generateDiscoveryEvent();
    Event generatePersonalEvent();

private:
    std::shared_ptr<World> m_world;
    std::vector<Event> m_activeEvents;
    std::mt19937 m_rng;

    // Cache pour les calculs
    std::map<std::string, float> m_eventProbabilityCache;
    std::map<std::string, std::vector<std::string>> m_affectedEntitiesCache;

    // Méthodes utilitaires
    float calculateSeverity(const Event &event);
    float calculateDuration(const Event &event);
    std::map<std::string, float> calculateEffects(const Event &event);
    bool checkPrerequisites(const Event &event);
    void updateEventTriggers();
};
