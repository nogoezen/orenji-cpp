#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "City.hpp"
#include "TradeGood.hpp"
#include "Player.hpp"
#include "Ship.hpp"

namespace Orenji
{
    /**
     * @brief Structure représentant une transaction commerciale
     */
    struct TradeTransaction
    {
        int goodId;           // ID de la marchandise échangée
        std::string goodName; // Nom de la marchandise
        int quantity;         // Quantité échangée
        int unitPrice;        // Prix unitaire
        int totalPrice;       // Prix total (quantité * unitPrice)
        int cityId;           // ID de la ville où l'échange a lieu
        std::string cityName; // Nom de la ville
        bool isBuy;           // True si achat, false si vente
        int timestamp;        // Horodatage de la transaction

        // Constructeur par défaut
        TradeTransaction()
            : goodId(-1), goodName(""), quantity(0), unitPrice(0), totalPrice(0),
              cityId(-1), cityName(""), isBuy(true), timestamp(0) {}

        // Constructeur avec paramètres
        TradeTransaction(int gId, const std::string &gName, int qty, int uPrice, int cId, const std::string &cName, bool buy, int time)
            : goodId(gId), goodName(gName), quantity(qty), unitPrice(uPrice),
              totalPrice(qty * uPrice), cityId(cId), cityName(cName), isBuy(buy), timestamp(time) {}
    };

    /**
     * @brief Enum représentant un type de modificateur de prix
     */
    enum class PriceModifierType
    {
        FACTION,     // Modificateur basé sur la faction
        REPUTATION,  // Modificateur basé sur la réputation
        SKILL,       // Modificateur basé sur les compétences
        EVENT,       // Modificateur basé sur un événement temporaire
        MARKET_STATE // Modificateur basé sur l'état du marché
    };

    /**
     * @brief Struct représentant un modificateur de prix
     */
    struct PriceModifier
    {
        PriceModifierType type;  // Type de modificateur
        std::string description; // Description du modificateur
        float value;             // Valeur du modificateur (ex: 0.9 = -10%, 1.1 = +10%)
        int expiryTime;          // Temps d'expiration (pour les modificateurs temporaires)

        // Constructeur
        PriceModifier(PriceModifierType t, const std::string &desc, float val, int expiry = -1)
            : type(t), description(desc), value(val), expiryTime(expiry) {}
    };

    /**
     * @brief Classe gérant le système de commerce
     *
     * Cette classe gère toutes les transactions commerciales entre le joueur
     * et les villes, ainsi que l'évolution des prix et des marchandises disponibles.
     */
    class TradingSystem
    {
    private:
        // Catalogue des marchandises disponibles dans le jeu
        std::unordered_map<int, TradeGood> m_goods;

        // Liste des dernières transactions effectuées
        std::vector<TradeTransaction> m_transactionHistory;

        // Liste des modificateurs de prix actifs
        std::vector<PriceModifier> m_priceModifiers;

        // Fonction de callback appelée après une transaction réussie
        std::function<void(const TradeTransaction &)> m_transactionCallback;

        // Limites du système
        int m_maxTransactionHistory;
        int m_maxPriceModifiers;
        int m_currentGameTime; // Pour gérer l'expiration des modificateurs

    public:
        /**
         * Constructeur
         */
        TradingSystem();

        /**
         * Destructeur
         */
        ~TradingSystem() = default;

        /**
         * @brief Initialise le système avec un catalogue de marchandises
         *
         * @param goods Catalogue de marchandises
         */
        void initialize(const std::unordered_map<int, TradeGood> &goods);

        /**
         * @brief Ajoute une nouvelle marchandise au catalogue
         *
         * @param good Marchandise à ajouter
         * @return true si l'ajout a réussi, false sinon
         */
        bool addTradeGood(const TradeGood &good);

        /**
         * @brief Obtient une marchandise par son ID
         *
         * @param goodId ID de la marchandise
         * @return Référence constante vers la marchandise, ou une marchandise invalide si non trouvée
         */
        const TradeGood &getTradeGood(int goodId) const;

        /**
         * @brief Obtient la liste de toutes les marchandises
         *
         * @return Référence constante vers le catalogue de marchandises
         */
        const std::unordered_map<int, TradeGood> &getAllTradeGoods() const { return m_goods; }

        /**
         * @brief Effectue une transaction d'achat du joueur à une ville
         *
         * @param player Joueur effectuant l'achat
         * @param city Ville où l'achat est effectué
         * @param goodId ID de la marchandise achetée
         * @param quantity Quantité achetée
         * @param shipId ID du navire où stocker les marchandises (si applicable)
         * @return true si la transaction a réussi, false sinon
         */
        bool buyGoods(Player &player, City &city, int goodId, int quantity, int shipId = -1);

        /**
         * @brief Effectue une transaction de vente du joueur à une ville
         *
         * @param player Joueur effectuant la vente
         * @param city Ville où la vente est effectuée
         * @param goodId ID de la marchandise vendue
         * @param quantity Quantité vendue
         * @param shipId ID du navire d'où proviennent les marchandises (si applicable)
         * @return true si la transaction a réussi, false sinon
         */
        bool sellGoods(Player &player, City &city, int goodId, int quantity, int shipId = -1);

        /**
         * @brief Calcule le prix final d'une marchandise en tenant compte des modificateurs
         *
         * @param basePrice Prix de base
         * @param buyer Identifiant de l'acheteur (ex: faction)
         * @param seller Identifiant du vendeur (ex: faction)
         * @return Prix final après application des modificateurs
         */
        int calculateFinalPrice(int basePrice, const std::string &buyer, const std::string &seller) const;

        /**
         * @brief Ajoute un modificateur de prix
         *
         * @param modifier Modificateur à ajouter
         */
        void addPriceModifier(const PriceModifier &modifier);

        /**
         * @brief Supprime un modificateur de prix par type et description
         *
         * @param type Type du modificateur
         * @param description Description du modificateur
         * @return true si le modificateur a été trouvé et supprimé, false sinon
         */
        bool removePriceModifier(PriceModifierType type, const std::string &description);

        /**
         * @brief Met à jour les modificateurs de prix (supprime ceux expirés)
         *
         * @param currentTime Temps actuel du jeu
         */
        void updatePriceModifiers(int currentTime);

        /**
         * @brief Définit le callback appelé après une transaction réussie
         *
         * @param callback Fonction de callback
         */
        void setTransactionCallback(std::function<void(const TradeTransaction &)> callback);

        /**
         * @brief Obtient l'historique des transactions
         *
         * @return Référence constante vers l'historique des transactions
         */
        const std::vector<TradeTransaction> &getTransactionHistory() const { return m_transactionHistory; }

        /**
         * @brief Obtient les modificateurs de prix actifs
         *
         * @return Référence constante vers les modificateurs de prix
         */
        const std::vector<PriceModifier> &getPriceModifiers() const { return m_priceModifiers; }

        /**
         * @brief Calcule le prix de transport entre deux villes
         *
         * @param fromCity Ville de départ
         * @param toCity Ville d'arrivée
         * @param quantity Quantité transportée
         * @return Prix du transport
         */
        int calculateTransportCost(const City &fromCity, const City &toCity, int quantity) const;

        /**
         * @brief Génère des fluctuations de prix aléatoires pour les marchandises dans une ville
         *
         * @param city Ville à mettre à jour
         * @param magnitude Amplitude des fluctuations (1-10)
         */
        void generatePriceFluctuations(City &city, int magnitude = 5);

        /**
         * @brief Met à jour la disponibilité des marchandises dans une ville
         *
         * @param city Ville à mettre à jour
         */
        void updateCityGoods(City &city);

        /**
         * @brief Vérifie si un navire peut transporter une quantité donnée de marchandises
         *
         * @param ship Navire à vérifier
         * @param goodId ID de la marchandise
         * @param quantity Quantité à transporter
         * @return true si le navire peut transporter la marchandise, false sinon
         */
        bool canShipCarry(const Ship &ship, int goodId, int quantity) const;
    };

} // namespace Orenji
