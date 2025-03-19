#include "EventAI.h"
#include <algorithm>
#include <cmath>

EventAI::EventAI(std::shared_ptr<World> world)
    : m_world(world), m_rng(std::random_device()())
{
}

void EventAI::update(float deltaTime)
{
    updateActiveEvents(deltaTime);
    updateEventTriggers();

    // Génération d'événements possibles
    auto possibleEvents = generatePossibleEvents();
    for (const auto &event : possibleEvents)
    {
        if (calculateEventProbability(event) > 0.5f)
        {
            triggerEvent(event);
        }
    }
}

std::vector<EventAI::Event> EventAI::generatePossibleEvents()
{
    std::vector<Event> events;

    // Génération d'événements naturels
    if (m_rng() % 100 < 20) // 20% de chance
    {
        events.push_back(generateNaturalDisaster());
    }

    // Génération d'événements politiques
    if (m_rng() % 100 < 15) // 15% de chance
    {
        events.push_back(generatePoliticalCrisis());
    }

    // Génération d'événements économiques
    if (m_rng() % 100 < 25) // 25% de chance
    {
        events.push_back(generateEconomicEvent());
    }

    // Génération d'événements militaires
    if (m_rng() % 100 < 10) // 10% de chance
    {
        events.push_back(generateMilitaryConflict());
    }

    // Génération d'événements culturels
    if (m_rng() % 100 < 30) // 30% de chance
    {
        events.push_back(generateCulturalEvent());
    }

    // Génération d'événements commerciaux
    if (m_rng() % 100 < 20) // 20% de chance
    {
        events.push_back(generateTradeEvent());
    }

    // Génération d'événements de découverte
    if (m_rng() % 100 < 5) // 5% de chance
    {
        events.push_back(generateDiscoveryEvent());
    }

    // Génération d'événements personnels
    if (m_rng() % 100 < 15) // 15% de chance
    {
        events.push_back(generatePersonalEvent());
    }

    return events;
}

bool EventAI::triggerEvent(const Event &event)
{
    if (!checkPrerequisites(event))
    {
        return false;
    }

    Event triggeredEvent = event;
    triggeredEvent.severity = calculateSeverity(triggeredEvent);
    triggeredEvent.duration = calculateDuration(triggeredEvent);
    triggeredEvent.effects = calculateEffects(triggeredEvent);
    triggeredEvent.affectedEntities = getAffectedEntities(triggeredEvent);

    m_activeEvents.push_back(triggeredEvent);
    applyEventEffects(triggeredEvent);

    return true;
}

void EventAI::resolveEvent(const Event &event)
{
    // TODO: Implémenter la résolution des événements
}

void EventAI::updateActiveEvents(float deltaTime)
{
    for (auto it = m_activeEvents.begin(); it != m_activeEvents.end();)
    {
        it->duration -= deltaTime;
        if (it->duration <= 0.0f)
        {
            resolveEvent(*it);
            it = m_activeEvents.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

float EventAI::calculateEventProbability(const Event &event)
{
    std::string key = event.title + std::to_string(static_cast<int>(event.type));
    if (m_eventProbabilityCache.find(key) != m_eventProbabilityCache.end())
    {
        return m_eventProbabilityCache[key];
    }

    float probability = 0.5f; // Probabilité de base
    // TODO: Implémenter le calcul de la probabilité basé sur les conditions

    m_eventProbabilityCache[key] = probability;
    return probability;
}

std::vector<std::string> EventAI::getAffectedEntities(const Event &event)
{
    std::string key = event.title + std::to_string(static_cast<int>(event.type));
    if (m_affectedEntitiesCache.find(key) != m_affectedEntitiesCache.end())
    {
        return m_affectedEntitiesCache[key];
    }

    std::vector<std::string> entities;
    // TODO: Implémenter la sélection des entités affectées

    m_affectedEntitiesCache[key] = entities;
    return entities;
}

void EventAI::applyEventEffects(const Event &event)
{
    // TODO: Implémenter l'application des effets des événements
}

EventAI::Event EventAI::generateNaturalDisaster()
{
    Event event;
    event.type = Event::Type::NATURAL_DISASTER;
    event.title = "Tempête dévastatrice";
    event.description = "Une violente tempête s'abat sur la région";
    return event;
}

EventAI::Event EventAI::generatePoliticalCrisis()
{
    Event event;
    event.type = Event::Type::POLITICAL_CRISIS;
    event.title = "Crise politique";
    event.description = "Des tensions politiques éclatent dans le royaume";
    return event;
}

EventAI::Event EventAI::generateEconomicEvent()
{
    Event event;
    event.type = Event::Type::ECONOMIC_EVENT;
    event.title = "Crise économique";
    event.description = "Une crise économique frappe la région";
    return event;
}

EventAI::Event EventAI::generateMilitaryConflict()
{
    Event event;
    event.type = Event::Type::MILITARY_CONFLICT;
    event.title = "Conflit militaire";
    event.description = "Un conflit militaire éclate";
    return event;
}

EventAI::Event EventAI::generateCulturalEvent()
{
    Event event;
    event.type = Event::Type::CULTURAL_EVENT;
    event.title = "Festival culturel";
    event.description = "Un grand festival culturel est organisé";
    return event;
}

EventAI::Event EventAI::generateTradeEvent()
{
    Event event;
    event.type = Event::Type::TRADE_EVENT;
    event.title = "Crise commerciale";
    event.description = "Une crise commerciale affecte les échanges";
    return event;
}

EventAI::Event EventAI::generateDiscoveryEvent()
{
    Event event;
    event.type = Event::Type::DISCOVERY_EVENT;
    event.title = "Nouvelle découverte";
    event.description = "Une nouvelle terre est découverte";
    return event;
}

EventAI::Event EventAI::generatePersonalEvent()
{
    Event event;
    event.type = Event::Type::PERSONAL_EVENT;
    event.title = "Événement personnel";
    event.description = "Un événement personnel affecte le joueur";
    return event;
}

float EventAI::calculateSeverity(const Event &event)
{
    float severity = 0.5f; // Sévérité de base

    switch (event.type)
    {
    case Event::Type::NATURAL_DISASTER:
        severity = 0.8f + (m_rng() % 20) / 100.0f; // 0.8 - 1.0
        break;

    case Event::Type::POLITICAL_CRISIS:
        severity = 0.6f + (m_rng() % 30) / 100.0f; // 0.6 - 0.9
        break;

    case Event::Type::ECONOMIC_EVENT:
        severity = 0.5f + (m_rng() % 40) / 100.0f; // 0.5 - 0.9
        break;

    case Event::Type::MILITARY_CONFLICT:
        severity = 0.7f + (m_rng() % 30) / 100.0f; // 0.7 - 1.0
        break;

    case Event::Type::CULTURAL_EVENT:
        severity = 0.3f + (m_rng() % 40) / 100.0f; // 0.3 - 0.7
        break;

    case Event::Type::TRADE_EVENT:
        severity = 0.4f + (m_rng() % 40) / 100.0f; // 0.4 - 0.8
        break;

    case Event::Type::DISCOVERY_EVENT:
        severity = 0.6f + (m_rng() % 30) / 100.0f; // 0.6 - 0.9
        break;

    case Event::Type::PERSONAL_EVENT:
        severity = 0.4f + (m_rng() % 50) / 100.0f; // 0.4 - 0.9
        break;
    }

    return severity;
}

float EventAI::calculateDuration(const Event &event)
{
    float duration = 7.0f; // Durée de base

    switch (event.type)
    {
    case Event::Type::NATURAL_DISASTER:
        duration = 3.0f + (m_rng() % 4); // 3-7 jours
        break;

    case Event::Type::POLITICAL_CRISIS:
        duration = 10.0f + (m_rng() % 15); // 10-25 jours
        break;

    case Event::Type::ECONOMIC_EVENT:
        duration = 15.0f + (m_rng() % 20); // 15-35 jours
        break;

    case Event::Type::MILITARY_CONFLICT:
        duration = 5.0f + (m_rng() % 10); // 5-15 jours
        break;

    case Event::Type::CULTURAL_EVENT:
        duration = 2.0f + (m_rng() % 3); // 2-5 jours
        break;

    case Event::Type::TRADE_EVENT:
        duration = 8.0f + (m_rng() % 12); // 8-20 jours
        break;

    case Event::Type::DISCOVERY_EVENT:
        duration = 1.0f; // 1 jour
        break;

    case Event::Type::PERSONAL_EVENT:
        duration = 3.0f + (m_rng() % 7); // 3-10 jours
        break;
    }

    return duration;
}

std::map<std::string, float> EventAI::calculateEffects(const Event &event)
{
    std::map<std::string, float> effects;

    switch (event.type)
    {
    case Event::Type::NATURAL_DISASTER:
        effects["infrastructure"] = -0.3f * event.severity;
        effects["population"] = -0.2f * event.severity;
        effects["economy"] = -0.4f * event.severity;
        effects["stability"] = -0.5f * event.severity;
        break;

    case Event::Type::POLITICAL_CRISIS:
        effects["stability"] = -0.6f * event.severity;
        effects["economy"] = -0.3f * event.severity;
        effects["population"] = -0.1f * event.severity;
        effects["military"] = -0.2f * event.severity;
        break;

    case Event::Type::ECONOMIC_EVENT:
        effects["economy"] = -0.5f * event.severity;
        effects["population"] = -0.2f * event.severity;
        effects["stability"] = -0.3f * event.severity;
        effects["trade"] = -0.4f * event.severity;
        break;

    case Event::Type::MILITARY_CONFLICT:
        effects["military"] = -0.4f * event.severity;
        effects["population"] = -0.3f * event.severity;
        effects["economy"] = -0.5f * event.severity;
        effects["stability"] = -0.6f * event.severity;
        break;

    case Event::Type::CULTURAL_EVENT:
        effects["culture"] = 0.3f * event.severity;
        effects["population"] = 0.2f * event.severity;
        effects["economy"] = 0.1f * event.severity;
        effects["stability"] = 0.2f * event.severity;
        break;

    case Event::Type::TRADE_EVENT:
        effects["trade"] = -0.4f * event.severity;
        effects["economy"] = -0.3f * event.severity;
        effects["population"] = -0.1f * event.severity;
        effects["stability"] = -0.2f * event.severity;
        break;

    case Event::Type::DISCOVERY_EVENT:
        effects["exploration"] = 0.5f * event.severity;
        effects["economy"] = 0.3f * event.severity;
        effects["population"] = 0.1f * event.severity;
        effects["culture"] = 0.2f * event.severity;
        break;

    case Event::Type::PERSONAL_EVENT:
        effects["personal"] = 0.4f * event.severity;
        effects["reputation"] = 0.3f * event.severity;
        effects["wealth"] = 0.2f * event.severity;
        effects["influence"] = 0.3f * event.severity;
        break;
    }

    return effects;
}

bool EventAI::checkPrerequisites(const Event &event)
{
    // Vérification des conditions de base
    if (event.affectedEntities.empty())
    {
        return false;
    }

    // Vérification des conditions spécifiques selon le type d'événement
    switch (event.type)
    {
    case Event::Type::NATURAL_DISASTER:
        // Vérifier la météo et la saison
        return true;

    case Event::Type::POLITICAL_CRISIS:
        // Vérifier la stabilité politique
        return true;

    case Event::Type::ECONOMIC_EVENT:
        // Vérifier l'état de l'économie
        return true;

    case Event::Type::MILITARY_CONFLICT:
        // Vérifier les relations diplomatiques
        return true;

    case Event::Type::CULTURAL_EVENT:
        // Vérifier la culture locale
        return true;

    case Event::Type::TRADE_EVENT:
        // Vérifier le volume de commerce
        return true;

    case Event::Type::DISCOVERY_EVENT:
        // Vérifier les zones explorées
        return true;

    case Event::Type::PERSONAL_EVENT:
        // Vérifier les relations personnelles
        return true;

    default:
        return false;
    }
}

void EventAI::updateEventTriggers()
{
    // Mise à jour des probabilités d'événements
    for (auto &[key, probability] : m_eventProbabilityCache)
    {
        // Ajuster les probabilités en fonction des conditions actuelles
        if (m_rng() % 100 < 20) // 20% de chance de changement
        {
            probability += (m_rng() % 20 - 10) / 100.0f;               // -0.1 à +0.1
            probability = std::max(0.0f, std::min(1.0f, probability)); // Clamp entre 0 et 1
        }
    }

    // Nettoyer le cache des entités affectées
    m_affectedEntitiesCache.clear();
}