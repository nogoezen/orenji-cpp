#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Ship.h"

// Nombre maximum de navires dans une flotte
constexpr int MAX_FLEET_SIZE = 5;

class Fleet
{
private:
    std::vector<std::shared_ptr<Ship>> m_ships;
    std::string m_name;
    int m_posX = 0;
    int m_posY = 0;
    int m_flagship = 0; // Index du navire amiral dans le vecteur

public:
    Fleet(const std::string &name = "Ma Flotte");
    ~Fleet() = default;

    // Gestion des navires
    bool addShip(std::shared_ptr<Ship> ship);
    bool removeShip(size_t index);
    std::shared_ptr<Ship> getShip(size_t index) const;
    std::shared_ptr<Ship> getFlagship() const;
    bool setFlagship(size_t index);
    size_t getSize() const { return m_ships.size(); }
    bool isFull() const { return m_ships.size() >= MAX_FLEET_SIZE; }

    // Propriétés de la flotte
    std::string getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    // Position de la flotte
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    void setPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
    }

    // Capacités de la flotte
    int getTotalCargo() const;
    int getUsedCargo() const;
    int getRemainingCargo() const { return getTotalCargo() - getUsedCargo(); }

    int getTotalCrew() const;
    int getCurrentCrew() const;

    // Vitesse et mobilité
    int getSpeed() const;
    int getVisibilityRange() const;

    // État de la flotte
    bool canNavigate() const;
};