#pragma once

#include <string>
#include <memory>
#include "../models/Player.h"
#include "../models/Ship.h"
#include "../models/Fleet.h"
#include "GameData.h"
#include "World.h"
#include "ConsoleUI.h"
#include "CharacterEditor.h"
#include "FleetManager.h"

class Game
{
private:
    std::shared_ptr<Player> m_player;
    ConsoleUI m_ui;
    bool m_running;

    // Méthodes pour les différentes actions du jeu
    void mainMenu();
    void newGame();
    void loadGame();
    void saveGame();
    void quitGame();

    // Boucle principale de jeu
    void gameLoop();

    // Actions de jeu
    void navigationMenu();
    void navigate(int dx, int dy);
    void cityMenu();
    void portMenu();
    void tradeMenu();
    void tradeBuyMenu();
    void tradeSellMenu();
    void shipyardMenu();
    void tavernMenu();
    void inventoryMenu();
    void statusMenu();
    void fleetManagementMenu(); // Gestion de la flotte

    // Affichage de la carte
    void displayMap(int centerX, int centerY, int radius = 5) const;

    // Combat naval
    void initiateBattle();
    void battleLoop();

public:
    Game() : m_ui(80, 24), m_running(false) {}

    // Démarrer le jeu
    void start();

    // Arrêter le jeu
    void stop() { m_running = false; }

    // Obtenir le joueur
    std::shared_ptr<Player> getPlayer() const { return m_player; }
};