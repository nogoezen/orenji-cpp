#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "../ui/GuiUI.h"
#include "../models/Player.h"
#include "../models/Ship.h"
#include "../models/Fleet.h"

class FleetManager
{
private:
    GuiUI &m_ui;
    std::shared_ptr<Player> m_player;

public:
    FleetManager(GuiUI &ui, std::shared_ptr<Player> player)
        : m_ui(ui), m_player(player) {}

    // Méthode principale pour afficher le menu de gestion de flotte
    void showFleetManagementMenu();

    // Nouvelles méthodes pour la gestion d'urgence
    void performEmergencyRepairs();
    void reorganizeCrew();
    void manageCrewStations();
    void reassignCrewStations(std::shared_ptr<Ship> ship);
    void rotateCrewShifts(std::shared_ptr<Ship> ship);

    // Méthodes de gestion de la flotaison
    void showFleetMenu();
    void showShipDetails(const Ship &ship);
    void showAllShipDetails();
    void buyShip(int portId);
    void sellShip(int shipIndex);
    void repairShip(int shipIndex);
    void upgradeShip(int shipIndex);
    void recruitCrew(int shipIndex);
    void setFlagship(int shipIndex);

    // Méthodes de gestion de la cargaison
    void transferCargo(int sourceShipIndex, int targetShipIndex);
    void loadCargo(int shipIndex, int portId);
    void unloadCargo(int shipIndex, int portId);
    void showCargoCapacity(int shipIndex);

    // Méthodes avancées
    void formFleet(const std::vector<int> &shipIndices);
    void disbandFleet(int fleetIndex);
    void manageFleets();

private:
    // Options principales du menu de gestion de flotte
    void viewFleetDetails();
    void manageShips();
    void transferCargo();
    void assignCrew();
    void selectFlagship();
    void renameFleet();
    void manageFleetFormations();
    void manageFleetSupplies();
    void manageTradeRoutes();
    void viewTradeRoutes();

    // Utilitaires pour la gestion des navires
    void showFleetSummary();
    void showFleetDetails();
    void showShipDetails(const std::shared_ptr<Ship> &ship);
    void showCargoDetails();
    void showCrewDetails();
    void addShipToFleet();
    void removeShipFromFleet();
    void repairShip();
    void upgradeShip();
    void buyNewShip();
    void sellShip();
    void renameShip();

    // Utilitaires pour la gestion des formations
    void showFormationDetails();
    void selectFormation();
    void showFormationBonuses(FleetFormation formation);
    void trainFleetInFormation();
    void customizeFormation();

    // Utilitaires pour la gestion des provisions
    void showSuppliesStatus();
    void restockProvisions();
    void distributeRations();
    void boostCrewMorale();

    // Utilitaires pour les routes commerciales
    void createTradeRoute();
    void modifyTradeRoute();
    void deleteTradeRoute();
    void simulateTradeRoutes(int days);
    void calculateRouteProfit(const TradeRoute &route);
    void findOptimalTradeRoutes();
    void displayTradeRouteDetails(const TradeRoute &route, int routeIndex = -1);

    // Utilitaires pour la gestion de l'équipage
    void redistributeCrew();
    void transferCargoToShip(size_t sourceIdx, size_t destIdx);
    void transferCargoToShore();
    void transferCargoFromShore();

    // Méthodes d'interface utilisateur améliorées
    void showFleetStatusGauge();
    void showMoraleGauge();
    void showFatigueGauge();
    void showProvisionsGauge();
    void showCohesionGauge();

    // Méthodes pour appliquer les bonus d'amirauté
    void applyAdmiraltyBonuses();
    float calculateNavigationBonus();
    float calculateTacticsBonus();
    float calculateLeadershipBonus();
    float calculateMaintainanceBonus();

    // Utilitaire pour afficher une jauge visuelle
    void displayGauge(const std::string &label, float value, float maxValue,
                      int width = 20, char fillChar = '#', char emptyChar = '-');
};