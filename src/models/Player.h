#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Ship.h"
#include "TradeGood.h"

class Player {
private:
    std::string m_name;
    int m_gold;
    int m_level;
    int m_experience;
    
    // Navire actuel du joueur
    std::shared_ptr<Ship> m_currentShip;
    
    // Inventaire du joueur (ID de l'article, quantité)
    std::map<int, int> m_inventory;
    
    // Cargaison du navire (ID du bien commercial, quantité)
    std::map<int, int> m_cargo;
    
    // Position du joueur (x, y)
    int m_posX;
    int m_posY;

public:
    // Constructeur
    Player(const std::string& name, int gold = 1000, int level = 1, int experience = 0)
        : m_name(name), m_gold(gold), m_level(level), m_experience(experience),
          m_posX(0), m_posY(0) {}
    
    // Getters
    const std::string& getName() const { return m_name; }
    int getGold() const { return m_gold; }
    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }
    const std::shared_ptr<Ship>& getCurrentShip() const { return m_currentShip; }
    const std::map<int, int>& getInventory() const { return m_inventory; }
    const std::map<int, int>& getCargo() const { return m_cargo; }
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    
    // Setters et méthodes d'action
    void setName(const std::string& name) { m_name = name; }
    void addGold(int amount) { m_gold += amount; }
    void removeGold(int amount) { m_gold = std::max(0, m_gold - amount); }
    void addExperience(int amount);
    void setPosition(int x, int y) { m_posX = x; m_posY = y; }
    void move(int deltaX, int deltaY) { m_posX += deltaX; m_posY += deltaY; }
    
    // Méthodes liées au navire
    void setShip(std::shared_ptr<Ship> ship) { m_currentShip = ship; }
    
    // Méthodes d'inventaire
    void addToInventory(int itemId, int quantity = 1);
    void removeFromInventory(int itemId, int quantity = 1);
    int getItemQuantity(int itemId) const;
    
    // Méthodes de cargaison
    bool addToCargo(int goodId, int quantity = 1);
    void removeFromCargo(int goodId, int quantity = 1);
    int getCargoQuantity(int goodId) const;
    int getTotalCargoWeight() const;
    int getRemainingCargoCapacity() const;
}; 