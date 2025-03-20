#pragma once

#include "DataManager.hpp"
#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace Orenji
{
    /**
     * @brief Classe utilitaire pour accéder facilement aux données du jeu
     *
     * Cette classe fournit des fonctions statiques pour manipuler les données du jeu
     * sans avoir à utiliser directement le DataManager.
     */
    class DataUtils
    {
    public:
        /**
         * @brief Initialise le système de données
         * @param basePath Chemin de base pour les fichiers de données
         * @return true si l'initialisation a réussi, false sinon
         */
        static bool initialize(const std::string &basePath = "resources/data/");

        /**
         * @brief Charge toutes les données du jeu
         * @return true si le chargement a réussi, false sinon
         */
        static bool loadAllData();

        /**
         * @brief Sauvegarde toutes les données du jeu
         * @return true si la sauvegarde a réussi, false sinon
         */
        static bool saveAllData();

        // ================================================
        // Méthodes pour les personnages (Character)
        // ================================================

        /**
         * @brief Obtient un personnage par son ID
         * @param id Identifiant du personnage
         * @return Pointeur partagé vers le personnage ou nullptr si non trouvé
         */
        static std::shared_ptr<Character> getCharacter(int id);

        /**
         * @brief Obtient tous les personnages
         * @return Vecteur de tous les personnages
         */
        static std::vector<std::shared_ptr<Character>> getAllCharacters();

        /**
         * @brief Obtient des personnages qui correspondent à un critère
         * @param filterFunc Fonction de filtrage
         * @return Vecteur de personnages filtrés
         */
        static std::vector<std::shared_ptr<Character>> getCharactersWhere(std::function<bool(const Character &)> filterFunc);

        /**
         * @brief Ajoute un nouveau personnage
         * @param character Personnage à ajouter
         * @return Identifiant du nouveau personnage
         */
        static int addCharacter(const Character &character);

        /**
         * @brief Met à jour un personnage existant
         * @param character Personnage avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        static bool updateCharacter(const Character &character);

        /**
         * @brief Supprime un personnage par son ID
         * @param id Identifiant du personnage
         * @return true si la suppression a réussi, false sinon
         */
        static bool deleteCharacter(int id);

        // ================================================
        // Méthodes pour les navires (Ship)
        // ================================================

        /**
         * @brief Obtient un navire par son ID
         * @param id Identifiant du navire
         * @return Pointeur partagé vers le navire ou nullptr si non trouvé
         */
        static std::shared_ptr<Ship> getShip(int id);

        /**
         * @brief Obtient tous les navires
         * @return Vecteur de tous les navires
         */
        static std::vector<std::shared_ptr<Ship>> getAllShips();

        /**
         * @brief Obtient des navires qui correspondent à un critère
         * @param filterFunc Fonction de filtrage
         * @return Vecteur de navires filtrés
         */
        static std::vector<std::shared_ptr<Ship>> getShipsWhere(std::function<bool(const Ship &)> filterFunc);

        /**
         * @brief Ajoute un nouveau navire
         * @param ship Navire à ajouter
         * @return Identifiant du nouveau navire
         */
        static int addShip(const Ship &ship);

        /**
         * @brief Met à jour un navire existant
         * @param ship Navire avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        static bool updateShip(const Ship &ship);

        /**
         * @brief Supprime un navire par son ID
         * @param id Identifiant du navire
         * @return true si la suppression a réussi, false sinon
         */
        static bool deleteShip(int id);

        // ================================================
        // Méthodes pour les flottes (Fleet)
        // ================================================

        /**
         * @brief Obtient une flotte par son ID
         * @param id Identifiant de la flotte
         * @return Pointeur partagé vers la flotte ou nullptr si non trouvée
         */
        static std::shared_ptr<Fleet> getFleet(int id);

        /**
         * @brief Obtient toutes les flottes
         * @return Vecteur de toutes les flottes
         */
        static std::vector<std::shared_ptr<Fleet>> getAllFleets();

        /**
         * @brief Obtient des flottes qui correspondent à un critère
         * @param filterFunc Fonction de filtrage
         * @return Vecteur de flottes filtrées
         */
        static std::vector<std::shared_ptr<Fleet>> getFleetsWhere(std::function<bool(const Fleet &)> filterFunc);

        /**
         * @brief Ajoute une nouvelle flotte
         * @param fleet Flotte à ajouter
         * @return Identifiant de la nouvelle flotte
         */
        static int addFleet(const Fleet &fleet);

        /**
         * @brief Met à jour une flotte existante
         * @param fleet Flotte avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        static bool updateFleet(const Fleet &fleet);

        /**
         * @brief Supprime une flotte par son ID
         * @param id Identifiant de la flotte
         * @return true si la suppression a réussi, false sinon
         */
        static bool deleteFleet(int id);

        // ================================================
        // Méthodes pour les biens commerciaux (TradeGood)
        // ================================================

        /**
         * @brief Obtient un bien commercial par son ID
         * @param id Identifiant du bien
         * @return Pointeur partagé vers le bien ou nullptr si non trouvé
         */
        static std::shared_ptr<TradeGood> getTradeGood(int id);

        /**
         * @brief Obtient tous les biens commerciaux
         * @return Vecteur de tous les biens commerciaux
         */
        static std::vector<std::shared_ptr<TradeGood>> getAllTradeGoods();

        /**
         * @brief Obtient des biens commerciaux qui correspondent à un critère
         * @param filterFunc Fonction de filtrage
         * @return Vecteur de biens commerciaux filtrés
         */
        static std::vector<std::shared_ptr<TradeGood>> getTradeGoodsWhere(std::function<bool(const TradeGood &)> filterFunc);

        /**
         * @brief Ajoute un nouveau bien commercial
         * @param tradeGood Bien commercial à ajouter
         * @return Identifiant du nouveau bien commercial
         */
        static int addTradeGood(const TradeGood &tradeGood);

        /**
         * @brief Met à jour un bien commercial existant
         * @param tradeGood Bien commercial avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        static bool updateTradeGood(const TradeGood &tradeGood);

        /**
         * @brief Supprime un bien commercial par son ID
         * @param id Identifiant du bien commercial
         * @return true si la suppression a réussi, false sinon
         */
        static bool deleteTradeGood(int id);

        // ================================================
        // Méthodes pour le monde (World)
        // ================================================

        /**
         * @brief Obtient le monde actuel
         * @return Référence au monde actuel
         */
        static World &getWorld();

        /**
         * @brief Met à jour le monde
         * @param world Monde avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        static bool updateWorld(const World &world);

        // ================================================
        // Méthodes pour le joueur (Player)
        // ================================================

        /**
         * @brief Obtient le joueur actuel
         * @return Référence au joueur actuel
         */
        static Player &getPlayer();

        /**
         * @brief Met à jour le joueur
         * @param player Joueur avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        static bool updatePlayer(const Player &player);
    };

} // namespace Orenji