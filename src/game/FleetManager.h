#pragma once

#include <memory>
#include "../models/Player.h"
#include "../models/Fleet.h"
#include "ConsoleUI.h"
#include "GameData.h"

class FleetManager
{
private:
    ConsoleUI &m_ui;
    std::shared_ptr<Player> m_player;

public:
    FleetManager(ConsoleUI &ui, std::shared_ptr<Player> player)
        : m_ui(ui), m_player(player) {}

    // Méthode principale pour afficher le menu de gestion de flotte
    void showFleetManagementMenu();

private:
    // Méthodes d'action pour chaque option du menu
    void viewFleetDetails();
    void manageShips();
    void transferCargo();
    void assignCrew();
    void selectFlagship();
    void renameFleet();
    void renameShip();

    // Méthodes utilitaires
    void displayFleetSummary();
    void displayShipDetails(std::shared_ptr<Ship> ship);
    void displayCargoDetails(std::shared_ptr<Ship> ship);
    void displayCrewDetails(std::shared_ptr<Ship> ship);

    // Méthodes pour la gestion des navires
    void addShipToFleet();
    void removeShipFromFleet();
    void transferCargoBetweeShips();
    void redistributeCrew();

    // Achat/vente de navires
    void buyNewShip();
    void sellShip();

    // Réparation et amélioration
    void repairShip();
    void upgradeShip();
};