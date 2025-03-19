#include "Fleet.h"
#include <algorithm>

Fleet::Fleet(const std::string &name) : m_name(name)
{
    // Initialisation avec un vecteur vide
}

bool Fleet::addShip(std::shared_ptr<Ship> ship)
{
    if (m_ships.size() >= MAX_FLEET_SIZE)
    {
        return false; // La flotte est déjà pleine
    }

    m_ships.push_back(ship);

    // Si c'est le premier navire, il devient automatiquement le navire amiral
    if (m_ships.size() == 1)
    {
        m_flagship = 0;
    }

    return true;
}

bool Fleet::removeShip(size_t index)
{
    if (index >= m_ships.size())
    {
        return false; // Index invalide
    }

    // Si le navire à supprimer est le navire amiral
    if (index == m_flagship)
    {
        // S'il y a d'autres navires, définir le premier comme amiral
        if (m_ships.size() > 1)
        {
            m_flagship = (index == 0) ? 1 : 0;
        }
    }
    else if (index < m_flagship)
    {
        // Si le navire supprimé est avant le navire amiral, ajuster l'index
        m_flagship--;
    }

    m_ships.erase(m_ships.begin() + index);

    return true;
}

std::shared_ptr<Ship> Fleet::getShip(size_t index) const
{
    if (index >= m_ships.size())
    {
        return nullptr; // Index invalide
    }

    return m_ships[index];
}

std::shared_ptr<Ship> Fleet::getFlagship() const
{
    if (m_ships.empty())
    {
        return nullptr;
    }

    return m_ships[m_flagship];
}

bool Fleet::setFlagship(size_t index)
{
    if (index >= m_ships.size())
    {
        return false; // Index invalide
    }

    m_flagship = index;
    return true;
}

int Fleet::getTotalCargo() const
{
    int totalCargo = 0;
    for (const auto &ship : m_ships)
    {
        totalCargo += ship->getCargoCapacity();
    }
    return totalCargo;
}

int Fleet::getUsedCargo() const
{
    int usedCargo = 0;
    for (const auto &ship : m_ships)
    {
        usedCargo += ship->getUsedCargo();
    }
    return usedCargo;
}

int Fleet::getTotalCrew() const
{
    int totalCrew = 0;
    for (const auto &ship : m_ships)
    {
        totalCrew += ship->getRequiredCrew();
    }
    return totalCrew;
}

int Fleet::getCurrentCrew() const
{
    int currentCrew = 0;
    for (const auto &ship : m_ships)
    {
        currentCrew += ship->getCurrentCrew();
    }
    return currentCrew;
}

int Fleet::getSpeed() const
{
    // La vitesse de la flotte est déterminée par le navire le plus lent
    if (m_ships.empty())
    {
        return 0;
    }

    int minSpeed = m_ships[0]->getSpeed();
    for (size_t i = 1; i < m_ships.size(); ++i)
    {
        int shipSpeed = m_ships[i]->getSpeed();
        if (shipSpeed < minSpeed)
        {
            minSpeed = shipSpeed;
        }
    }

    return minSpeed;
}

int Fleet::getVisibilityRange() const
{
    // La visibilité est déterminée par le navire amiral
    auto flagship = getFlagship();
    if (!flagship)
    {
        return 5; // Valeur par défaut
    }

    return flagship->getVisibilityRange();
}

bool Fleet::canNavigate() const
{
    if (m_ships.empty())
    {
        return false;
    }

    // Vérifier que tous les navires ont suffisamment d'équipage
    for (const auto &ship : m_ships)
    {
        if (!ship->hasMinimumCrew())
        {
            return false;
        }
    }

    return true;
}