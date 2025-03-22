#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

namespace Orenji
{
    // Type d'énumération pour les types de navires
    enum class ShipType
    {
        SMALL_MERCHANT, // Petit navire marchand
        LARGE_MERCHANT, // Grand navire marchand
        WARSHIP,        // Navire de guerre
        PATROL_BOAT,    // Bateau de patrouille
        PIRATE_SHIP,    // Navire pirate
        ROYAL_SHIP,     // Navire royal
        CUSTOM          // Type personnalisé
    };

    /**
     * @brief Classe représentant un navire dans le jeu
     */
    class Ship
    {
    private:
        int m_id;                  // Identifiant unique du navire
        std::string m_name;        // Nom du navire
        ShipType m_type;           // Type de navire
        std::string m_description; // Description du navire

        // Propriétés de base
        int m_level;         // Niveau du navire
        int m_maxHealth;     // Santé maximale
        int m_currentHealth; // Santé actuelle

        // Position et mouvement
        float m_x;            // Position X sur la carte
        float m_y;            // Position Y sur la carte
        float m_rotation;     // Rotation du navire (en degrés)
        float m_speed;        // Vitesse actuelle
        float m_maxSpeed;     // Vitesse maximale
        float m_acceleration; // Accélération
        float m_turnRate;     // Vitesse de rotation

        // Propriétés de combat
        int m_attackPower; // Puissance d'attaque
        int m_defense;     // Défense
        int m_range;       // Portée d'attaque
        int m_accuracy;    // Précision

        // Équipage
        int m_maxCrewSize;     // Taille maximale de l'équipage
        int m_currentCrewSize; // Taille actuelle de l'équipage

        // Cargo
        float m_cargoCapacity;                // Capacité de fret totale (en unités de poids)
        std::unordered_map<int, int> m_cargo; // Inventaire de fret (id_produit -> quantité)
        float m_currentCargoWeight;           // Poids actuel du fret

        // Propriétaire et faction
        int m_ownerId;         // ID du propriétaire (joueur ou NPC)
        std::string m_faction; // Faction à laquelle appartient le navire

        // Canons et armement
        int m_cannonSlots;      // Nombre d'emplacements pour canons
        int m_installedCannons; // Nombre de canons installés

        // Propriétés de navigation
        float m_fuelCapacity;        // Capacité maximale de carburant
        float m_currentFuel;         // Carburant actuel
        float m_fuelConsumptionRate; // Taux de consommation de carburant

        // État du navire
        bool m_isDestroyed;                       // Indique si le navire est détruit
        bool m_isAnchored;                        // Indique si le navire est ancré
        std::vector<std::string> m_activeEffects; // Effets actifs sur le navire

    public:
        /**
         * @brief Constructeur par défaut
         */
        Ship();

        /**
         * @brief Constructeur avec paramètres de base
         *
         * @param id Identifiant unique du navire
         * @param name Nom du navire
         * @param type Type de navire
         */
        Ship(int id, const std::string &name, ShipType type);

        /**
         * @brief Constructeur à partir d'un objet JSON
         *
         * @param json Objet JSON contenant les données du navire
         */
        Ship(const nlohmann::json &json);

        /**
         * @brief Destructeur
         */
        ~Ship() = default;

        /**
         * @brief Convertit l'objet en JSON
         *
         * @return Objet JSON représentant le navire
         */
        nlohmann::json toJson() const;

        // Getters
        int getId() const { return m_id; }
        const std::string &getName() const { return m_name; }
        ShipType getType() const { return m_type; }
        const std::string &getDescription() const { return m_description; }
        int getLevel() const { return m_level; }
        int getMaxHealth() const { return m_maxHealth; }
        int getCurrentHealth() const { return m_currentHealth; }
        float getX() const { return m_x; }
        float getY() const { return m_y; }
        float getRotation() const { return m_rotation; }
        float getSpeed() const { return m_speed; }
        float getMaxSpeed() const { return m_maxSpeed; }
        float getAcceleration() const { return m_acceleration; }
        float getTurnRate() const { return m_turnRate; }
        int getAttackPower() const { return m_attackPower; }
        int getDefense() const { return m_defense; }
        int getRange() const { return m_range; }
        int getAccuracy() const { return m_accuracy; }
        int getMaxCrewSize() const { return m_maxCrewSize; }
        int getCurrentCrewSize() const { return m_currentCrewSize; }
        float getCargoCapacity() const { return m_cargoCapacity; }
        float getCurrentCargoWeight() const { return m_currentCargoWeight; }
        int getOwnerId() const { return m_ownerId; }
        const std::string &getFaction() const { return m_faction; }
        int getCannonSlots() const { return m_cannonSlots; }
        int getInstalledCannons() const { return m_installedCannons; }
        float getFuelCapacity() const { return m_fuelCapacity; }
        float getCurrentFuel() const { return m_currentFuel; }
        float getFuelConsumptionRate() const { return m_fuelConsumptionRate; }
        bool isDestroyed() const { return m_isDestroyed; }
        bool isAnchored() const { return m_isAnchored; }
        const std::vector<std::string> &getActiveEffects() const { return m_activeEffects; }

        // Setters
        void setId(int id) { m_id = id; }
        void setName(const std::string &name) { m_name = name; }
        void setType(ShipType type) { m_type = type; }
        void setDescription(const std::string &description) { m_description = description; }
        void setLevel(int level) { m_level = level; }
        void setMaxHealth(int maxHealth) { m_maxHealth = maxHealth; }
        void setCurrentHealth(int health) { m_currentHealth = health; }
        void setPosition(float x, float y)
        {
            m_x = x;
            m_y = y;
        }
        void setRotation(float rotation) { m_rotation = rotation; }
        void setSpeed(float speed) { m_speed = speed; }
        void setMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }
        void setAcceleration(float acceleration) { m_acceleration = acceleration; }
        void setTurnRate(float turnRate) { m_turnRate = turnRate; }
        void setAttackPower(int attackPower) { m_attackPower = attackPower; }
        void setDefense(int defense) { m_defense = defense; }
        void setRange(int range) { m_range = range; }
        void setAccuracy(int accuracy) { m_accuracy = accuracy; }
        void setMaxCrewSize(int maxCrewSize) { m_maxCrewSize = maxCrewSize; }
        void setCurrentCrewSize(int crewSize) { m_currentCrewSize = crewSize; }
        void setCargoCapacity(float capacity) { m_cargoCapacity = capacity; }
        void setOwnerId(int ownerId) { m_ownerId = ownerId; }
        void setFaction(const std::string &faction) { m_faction = faction; }
        void setCannonSlots(int slots) { m_cannonSlots = slots; }
        void setInstalledCannons(int cannons) { m_installedCannons = cannons; }
        void setFuelCapacity(float capacity) { m_fuelCapacity = capacity; }
        void setCurrentFuel(float fuel) { m_currentFuel = fuel; }
        void setFuelConsumptionRate(float rate) { m_fuelConsumptionRate = rate; }
        void setDestroyed(bool destroyed) { m_isDestroyed = destroyed; }
        void setAnchored(bool anchored) { m_isAnchored = anchored; }

        /**
         * @brief Ajoute un effet au navire
         *
         * @param effect Nom de l'effet à ajouter
         */
        void addEffect(const std::string &effect);

        /**
         * @brief Supprime un effet du navire
         *
         * @param effect Nom de l'effet à supprimer
         * @return true si l'effet a été trouvé et supprimé, false sinon
         */
        bool removeEffect(const std::string &effect);

        /**
         * @brief Vérifie si le navire a un effet spécifique
         *
         * @param effect Nom de l'effet à vérifier
         * @return true si l'effet est actif, false sinon
         */
        bool hasEffect(const std::string &effect) const;

        /**
         * @brief Supprime tous les effets du navire
         */
        void clearEffects();

        /**
         * @brief Ajoute des marchandises au cargo
         *
         * @param goodId ID de la marchandise à ajouter
         * @param quantity Quantité à ajouter
         * @param goodWeight Poids unitaire de la marchandise
         * @return true si l'ajout a réussi, false si la capacité est dépassée
         */
        bool addCargo(int goodId, int quantity, float goodWeight = 1.0f);

        /**
         * @brief Retire des marchandises du cargo
         *
         * @param goodId ID de la marchandise à retirer
         * @param quantity Quantité à retirer
         * @param goodWeight Poids unitaire de la marchandise
         * @return true si le retrait a réussi, false si la quantité est insuffisante
         */
        bool removeCargo(int goodId, int quantity, float goodWeight = 1.0f);

        /**
         * @brief Vérifie si le navire a une marchandise spécifique
         *
         * @param goodId ID de la marchandise à vérifier
         * @return true si la marchandise est présente, false sinon
         */
        bool hasCargo(int goodId) const;

        /**
         * @brief Obtient la quantité d'une marchandise dans le cargo
         *
         * @param goodId ID de la marchandise
         * @return Quantité de la marchandise, 0 si absente
         */
        int getCargoQuantity(int goodId) const;

        /**
         * @brief Obtient l'inventaire complet du cargo
         *
         * @return Map des marchandises (ID -> quantité)
         */
        const std::unordered_map<int, int> &getAllCargo() const { return m_cargo; }

        /**
         * @brief Vide complètement le cargo
         */
        void clearCargo();

        /**
         * @brief Inflige des dégâts au navire
         *
         * @param damage Quantité de dégâts à infliger
         * @return true si le navire est détruit après les dégâts, false sinon
         */
        bool takeDamage(int damage);

        /**
         * @brief Répare le navire
         *
         * @param amount Quantité de points de vie à restaurer
         * @return Points de vie effectivement restaurés
         */
        int repair(int amount);

        /**
         * @brief Met à jour la position du navire en fonction de sa vitesse et de sa direction
         *
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void updatePosition(float deltaTime);

        /**
         * @brief Accélère le navire
         *
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         * @param acceleration Facteur d'accélération (positif pour accélérer, négatif pour freiner)
         */
        void accelerate(float deltaTime, float acceleration = 1.0f);

        /**
         * @brief Fait tourner le navire
         *
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         * @param direction Direction de rotation (1 pour droite, -1 pour gauche)
         */
        void turn(float deltaTime, float direction);

        /**
         * @brief Consomme du carburant en fonction de la vitesse
         *
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         * @return Quantité de carburant consommée
         */
        float consumeFuel(float deltaTime);

        /**
         * @brief Remplit le réservoir de carburant
         *
         * @param amount Quantité de carburant à ajouter
         * @return Quantité de carburant effectivement ajoutée
         */
        float refuel(float amount);

        /**
         * @brief Calcule la distance jusqu'à un point
         *
         * @param x Coordonnée X du point
         * @param y Coordonnée Y du point
         * @return Distance euclidienne
         */
        float distanceTo(float x, float y) const;

        /**
         * @brief Calcule la distance jusqu'à un autre navire
         *
         * @param other Autre navire
         * @return Distance euclidienne
         */
        float distanceTo(const Ship &other) const;

        /**
         * @brief Vérifie si le navire est à portée d'attaque d'un point
         *
         * @param x Coordonnée X du point
         * @param y Coordonnée Y du point
         * @return true si le point est à portée, false sinon
         */
        bool isInRange(float x, float y) const;

        /**
         * @brief Vérifie si le navire est à portée d'attaque d'un autre navire
         *
         * @param other Autre navire
         * @return true si l'autre navire est à portée, false sinon
         */
        bool isInRange(const Ship &other) const;

        /**
         * @brief Convertit une chaîne de caractères en ShipType
         *
         * @param typeStr Chaîne représentant le type
         * @return ShipType correspondant
         */
        static ShipType stringToShipType(const std::string &typeStr);

        /**
         * @brief Convertit un ShipType en chaîne de caractères
         *
         * @param type Type à convertir
         * @return Chaîne représentant le type
         */
        static std::string shipTypeToString(ShipType type);
    };

} // namespace Orenji