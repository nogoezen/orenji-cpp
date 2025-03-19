#pragma once

#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// Forward declarations
class Game;
class Character;
class Player;

/**
 * @brief Menu de création de personnage
 *
 * Cette classe gère l'interface de création de personnage,
 * en permettant au joueur de choisir son nom, sa classe et son origine.
 */
class CharacterCreationMenu
{
private:
    // Référence au jeu
    Game *m_game;

    // Structures pour stocker les données de création
    struct ActorData
    {
        int id;
        std::string name;
        std::string description;
        std::vector<std::string> skills;
        std::map<std::string, int> attributes;
    };

    struct ClassData
    {
        int id;
        std::string name;
        std::string description;
        std::vector<std::string> skills;
        std::map<std::string, int> bonuses;
    };

    struct KingdomData
    {
        int id;
        std::string name;
        std::string description;
        std::vector<std::string> ships;
        std::map<std::string, int> relationModifiers;
    };

    // Données chargées
    std::vector<ActorData> m_actors;
    std::vector<ClassData> m_classes;
    std::vector<KingdomData> m_kingdoms;

    // Méthodes privées
    bool loadData();
    void displayActorSelection();
    void displayClassSelection();
    void displayKingdomSelection();
    void displayCharacterSummary(const std::string &name, int actorId, int classId, int kingdomId);
    std::shared_ptr<Character> createCharacter(const std::string &name, int actorId, int classId, int kingdomId);

public:
    /**
     * @brief Constructeur
     * @param game Référence au jeu
     */
    CharacterCreationMenu(Game *game);

    /**
     * @brief Destructeur
     */
    ~CharacterCreationMenu() = default;

    /**
     * @brief Lance le menu de création de personnage
     * @return true si la création a été confirmée, false si annulée
     */
    bool run();
};