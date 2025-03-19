#include "Game.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <random>

// Démarrer le jeu
void Game::start()
{
    // Initialiser le jeu
    m_running = true;

    // Précharger les données du jeu
    GameData::getInstance();
    World::getInstance();

    // Afficher le menu principal
    mainMenu();
}

// Menu principal
void Game::mainMenu()
{
    std::vector<ConsoleUI::MenuItem> menuItems = {
        {"Nouvelle partie", [this]()
         { newGame(); }},
        {"Charger une partie", [this]()
         { loadGame(); }},
        {"Quitter", [this]()
         { quitGame(); }}};

    m_ui.displayMenu("UNCHARTED WATERS - NOUVEAU HORIZONS", menuItems);
}

// Créer une nouvelle partie
void Game::newGame()
{
    m_ui.displayTitle("NOUVELLE PARTIE");

    // Obtenir le nom du joueur
    std::string playerName = m_ui.getInput("Entrez votre nom: ");

    // Message de transition vers l'éditeur de personnage
    m_ui.displayTitle("CRÉATION DE PERSONNAGE");
    m_ui.displayMessage("Bienvenue dans l'aventure, " + playerName + "!");
    m_ui.displayMessage("Vous allez maintenant créer votre personnage en personnalisant ses attributs et classes.");
    m_ui.displayMessage("\nVous pourrez choisir:");
    m_ui.displayMessage("- Un modèle de personnage avec son histoire et son background");
    m_ui.displayMessage("- Sa classe principale et ses compétences");
    m_ui.displayMessage("- Des classes secondaires pour des compétences supplémentaires");
    m_ui.displayMessage("- Ses attributs personnalisés");
    m_ui.displayMessage("\nAppuyez sur Entrée pour commencer l'édition de personnage...");
    m_ui.waitForKeyPress();

    // Utiliser l'éditeur de personnage
    CharacterEditor characterEditor(m_ui);
    std::shared_ptr<Character> playerCharacter = characterEditor.startEditor(playerName);

    if (!playerCharacter)
    {
        m_ui.displayError("Erreur lors de la création du personnage. Retour au menu principal.");
        m_ui.waitForKeyPress();
        mainMenu();
        return;
    }

    // Créer un nouveau joueur avec le personnage créé
    m_player = std::make_shared<Player>(playerName, playerCharacter);

    // Définir le mode initial sur terrestre
    m_player->setMode(PlayerMode::LAND);

    // Donner un navire de départ au joueur
    const auto &ships = GameData::getInstance().getShips();
    if (!ships.empty())
    {
        // Trouver la caravelle (ID 1)
        const Ship *startShip = GameData::getInstance().findShipById(1);
        if (startShip)
        {
            // Créer une copie du navire
            std::shared_ptr<Ship> playerShip = std::make_shared<Ship>(*startShip);
            m_player->setShip(playerShip);

            // Ajouter un équipage initial
            m_player->addCrew(playerShip->getRequiredCrew());

            m_ui.displayMessage("Vous avez reçu une Caravelle avec un équipage de " +
                                std::to_string(m_player->getCurrentCrew()) + " marins.");
        }
    }

    // Positionner le joueur à Lisbonne (ID 1)
    const City *startCity = World::getInstance().getCityById(1);
    if (startCity)
    {
        m_player->setPosition(startCity->posX, startCity->posY);
    }

    // Afficher un message de bienvenue
    m_ui.displayTitle("AVENTURE COMMENCE");
    m_ui.displayMessage("Bienvenue, Capitaine " + playerName + "!");
    m_ui.displayMessage("Une nouvelle aventure commence à Lisbonne. Vous pouvez explorer la ville ou prendre la mer.");

    // Expliquer les modes de jeu
    m_ui.displayMessage("\nModes de jeu:");
    m_ui.displayMessage("- Sur terre: Utilisez votre personnage " + playerCharacter->getName() +
                        " pour explorer, commercer et interagir avec les PNJ.");
    m_ui.displayMessage("- En mer: Prenez le contrôle de votre navire pour naviguer, commercer entre les ports et affronter d'autres navires.");
    m_ui.displayMessage("\nUtilisez la commande 'changer mode' pour basculer entre les deux modes.");

    m_ui.waitForKeyPress();

    // Débuter le jeu
    gameLoop();
}

// Charger une partie sauvegardée
void Game::loadGame()
{
    m_ui.displayTitle("CHARGER UNE PARTIE");
    m_ui.displayMessage("Fonctionnalité non implémentée.");
    m_ui.waitForKeyPress();

    mainMenu();
}

// Sauvegarder la partie
void Game::saveGame()
{
    m_ui.displayTitle("SAUVEGARDER LA PARTIE");
    m_ui.displayMessage("Fonctionnalité non implémentée.");
    m_ui.waitForKeyPress();
}

// Quitter le jeu
void Game::quitGame()
{
    m_ui.displayTitle("AU REVOIR");
    m_ui.displayMessage("Merci d'avoir joué à Uncharted Waters - Nouveaux Horizons!");
    m_ui.waitForKeyPress();

    stop();
}

// Boucle principale du jeu
void Game::gameLoop()
{
    while (m_running && m_player)
    {
        // Menu de navigation
        navigationMenu();

        // Vérifier s'il y a un événement ou une rencontre
        // (À implémenter plus tard)

        // Pause courte
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Menu de navigation
void Game::navigationMenu()
{
    // Position actuelle du joueur
    int playerX = m_player->getPosX();
    int playerY = m_player->getPosY();

    // Afficher la carte
    displayMap(playerX, playerY);

    // Options de navigation
    std::vector<ConsoleUI::MenuItem> menuItems = {
        {"Nord", [this]()
         { navigate(0, -1); }},
        {"Sud", [this]()
         { navigate(0, 1); }},
        {"Est", [this]()
         { navigate(1, 0); }},
        {"Ouest", [this]()
         { navigate(-1, 0); }},
        {"Gestion de la flotte", [this]()
         { fleetManagementMenu(); }},
        {"Statut", [this]()
         { statusMenu(); }},
        {"Inventaire", [this]()
         { inventoryMenu(); }},
        {"Menu de la ville", [this]()
         {
             const City *city = World::getInstance().getCityAtPosition(
                 m_player->getPosX(), m_player->getPosY());
             if (city)
             {
                 cityMenu();
             }
             else
             {
                 m_ui.displayError("Vous n'êtes pas dans une ville.");
                 m_ui.waitForKeyPress();
             }
         }},
        {"Sauvegarder", [this]()
         { saveGame(); }},
        {"Quitter", [this]()
         { quitGame(); }}};

    m_ui.displayMenu("NAVIGATION", menuItems);
}

// Naviguer dans une direction
void Game::navigate(int dx, int dy)
{
    int newX = m_player->getPosX() + dx;
    int newY = m_player->getPosY() + dy;

    // Vérifier si la nouvelle position est passable
    if (World::getInstance().isPassable(newX, newY))
    {
        m_player->setPosition(newX, newY);
        m_ui.displayMessage("Vous naviguez vers une nouvelle position.");
    }
    else
    {
        m_ui.displayError("Cette route est bloquée.");
    }

    m_ui.waitForKeyPress();
}

// Afficher la carte
void Game::displayMap(int centerX, int centerY, int radius) const
{
    m_ui.displayTitle("CARTE");

    const World &world = World::getInstance();

    // Calculer les limites de la carte à afficher
    int startX = std::max(0, centerX - radius);
    int endX = std::min(world.getWidth() - 1, centerX + radius);
    int startY = std::max(0, centerY - radius);
    int endY = std::min(world.getHeight() - 1, centerY + radius);

    // Afficher la légende
    m_ui.displayMessage("Légende: ~ = Eau, # = Terre, ^ = Montagne, C = Ville, P = Port, ! = Danger");
    m_ui.displayMessage("");

    // Afficher la carte
    for (int y = startY; y <= endY; ++y)
    {
        std::string line;

        for (int x = startX; x <= endX; ++x)
        {
            // Déterminer le caractère à afficher
            char c;
            const TerrainElement &element = world.getTerrain(x, y);

            // Position du joueur
            if (x == centerX && y == centerY)
            {
                c = '@';
            }
            else
            {
                // Type de terrain
                switch (element.type)
                {
                case TerrainElement::WATER:
                    c = '~';
                    break;
                case TerrainElement::LAND:
                    c = '#';
                    break;
                case TerrainElement::MOUNTAIN:
                    c = '^';
                    break;
                case TerrainElement::CITY:
                    c = 'C';
                    break;
                case TerrainElement::PORT:
                    c = 'P';
                    break;
                case TerrainElement::DANGER:
                    c = '!';
                    break;
                default:
                    c = '?';
                    break;
                }
            }

            line += c;
        }

        m_ui.displayMessage(line);
    }

    // Afficher les informations de position
    m_ui.displayMessage("");
    m_ui.displayMessage("Position actuelle: (" + std::to_string(centerX) + ", " + std::to_string(centerY) + ")");

    // Afficher le nom de la ville si on est dans une ville
    const City *city = world.getCityAtPosition(centerX, centerY);
    if (city)
    {
        m_ui.displayMessage("Vous êtes à " + city->name + " (" + city->region + ")");
    }
    else if (world.isPort(centerX, centerY))
    {
        m_ui.displayMessage("Vous êtes dans un port");
    }
}

// Menu d'état du joueur
void Game::statusMenu()
{
    m_ui.displayTitle("STATUT DU JOUEUR");

    m_ui.displayMessage("Nom: " + m_player->getName());
    m_ui.displayMessage("Or: " + std::to_string(m_player->getGold()));
    m_ui.displayMessage("Niveau: " + std::to_string(m_player->getLevel()));
    m_ui.displayMessage("Expérience: " + std::to_string(m_player->getExperience()));

    // Afficher les informations du navire si le joueur en possède un
    if (m_player->getCurrentShip())
    {
        const Ship &ship = *m_player->getCurrentShip();
        m_ui.displayMessage("");
        m_ui.displayMessage("Navire: " + ship.getName());
        m_ui.displayMessage("Classe: " + ship.getShipClass());
        m_ui.displayMessage("Durabilité: " + std::to_string(ship.getDurability()));
        m_ui.displayMessage("Vitesse: " + std::to_string(ship.getSpeed()));
        m_ui.displayMessage("Maniabilité: " + std::to_string(ship.getManeuverability()));
        m_ui.displayMessage("Capacité de cargo: " + std::to_string(ship.getCargoCapacity()));
        m_ui.displayMessage("Cargo utilisé: " + std::to_string(m_player->getTotalCargoWeight()));
        m_ui.displayMessage("Capacité restante: " + std::to_string(m_player->getRemainingCargoCapacity()));
    }
    else
    {
        m_ui.displayMessage("");
        m_ui.displayMessage("Vous ne possédez pas de navire.");
    }

    m_ui.waitForKeyPress();
}

// Menu d'inventaire
void Game::inventoryMenu()
{
    m_ui.displayTitle("INVENTAIRE");

    // Afficher l'or du joueur
    m_ui.displayMessage("Or: " + std::to_string(m_player->getGold()));

    // Afficher l'inventaire du joueur
    const auto &inventory = m_player->getInventory();

    if (inventory.empty())
    {
        m_ui.displayMessage("Votre inventaire est vide.");
    }
    else
    {
        m_ui.displayMessage("\nInventaire personnel:");
        for (const auto &[itemId, quantity] : inventory)
        {
            // Obtenir les informations de l'article (à implémenter plus tard)
            std::string itemName = "Article #" + std::to_string(itemId);
            m_ui.displayMessage(itemName + " x" + std::to_string(quantity));
        }
    }

    // Afficher la cargaison du navire
    const auto &cargo = m_player->getCargo();

    if (cargo.empty())
    {
        m_ui.displayMessage("\nVotre cargaison est vide.");
    }
    else
    {
        m_ui.displayMessage("\nCargaison du navire:");
        for (const auto &[goodId, quantity] : cargo)
        {
            // Obtenir les informations du bien commercial
            const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
            std::string goodName = good ? good->getName() : "Bien #" + std::to_string(goodId);

            if (good)
            {
                m_ui.displayMessage(goodName + " x" + std::to_string(quantity) +
                                    " (Poids: " + std::to_string(good->getWeight() * quantity) + ")");
            }
            else
            {
                m_ui.displayMessage(goodName + " x" + std::to_string(quantity));
            }
        }

        // Afficher le poids total et la capacité restante
        m_ui.displayMessage("\nPoids total: " + std::to_string(m_player->getTotalCargoWeight()));
        m_ui.displayMessage("Capacité restante: " + std::to_string(m_player->getRemainingCargoCapacity()));
    }

    m_ui.waitForKeyPress();
}

// Menu de ville
void Game::cityMenu()
{
    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Vous n'êtes pas dans une ville.");
        m_ui.waitForKeyPress();
        return;
    }

    // Options de la ville
    std::vector<ConsoleUI::MenuItem> menuItems = {
        {"Port", [this]()
         { portMenu(); }},
        {"Commerce", [this]()
         { tradeMenu(); }},
        {"Chantier naval", [this]()
         { shipyardMenu(); }},
        {"Taverne", [this]()
         { tavernMenu(); }},
        {"Retour à la navigation", [this]() {}}};

    m_ui.displayMenu("VILLE: " + city->name, menuItems);
}

// Menu du port
void Game::portMenu()
{
    m_ui.displayTitle("PORT");

    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Erreur: Ville non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Bienvenue au port de " + city->name + "!");
    m_ui.displayMessage("Services disponibles au port:");
    m_ui.displayMessage("1. Réparation de navire");
    m_ui.displayMessage("2. Recrutement d'équipage");
    m_ui.displayMessage("3. Missions spéciales");

    m_ui.displayMessage("\nFonctionnalité non implémentée.");
    m_ui.waitForKeyPress();
}

// Menu de commerce
void Game::tradeMenu()
{
    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Erreur: Ville non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Vérifier si le joueur a un navire
    if (!m_player->getCurrentShip())
    {
        m_ui.displayError("Vous avez besoin d'un navire pour commercer.");
        m_ui.waitForKeyPress();
        return;
    }

    // Menu de commerce
    bool trading = true;

    while (trading)
    {
        m_ui.displayTitle("COMMERCE - " + city->name);

        m_ui.displayMessage("Or: " + std::to_string(m_player->getGold()));
        m_ui.displayMessage("Capacité de cargo restante: " + std::to_string(m_player->getRemainingCargoCapacity()));
        m_ui.displayMessage("");

        // Options de commerce
        std::vector<ConsoleUI::MenuItem> menuItems = {
            {"Acheter des marchandises", [this]()
             { tradeBuyMenu(); }},
            {"Vendre des marchandises", [this]()
             { tradeSellMenu(); }},
            {"Consulter la cargaison", [this]()
             { inventoryMenu(); }},
            {"Quitter le marché", [&trading]()
             { trading = false; }}};

        m_ui.displayMenu("MARCHÉ", menuItems);
    }
}

// Menu d'achat
void Game::tradeBuyMenu()
{
    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Erreur: Ville non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("ACHETER - " + city->name);

    // Afficher l'or et la capacité disponible
    m_ui.displayMessage("Or disponible: " + std::to_string(m_player->getGold()));
    m_ui.displayMessage("Capacité de cargo restante: " + std::to_string(m_player->getRemainingCargoCapacity()));
    m_ui.displayMessage("");

    // Afficher les marchandises disponibles
    m_ui.displayMessage("Marchandises disponibles:");

    const auto &goods = GameData::getInstance().getTradeGoods();

    std::vector<std::string> headers = {"ID", "Nom", "Catégorie", "Prix", "Poids"};
    std::vector<std::vector<std::string>> data;

    for (const auto &good : goods)
    {
        // Calculer le prix local
        float marketFactor = 1.0f;
        auto it = city->marketFactors.find(good.getId());
        if (it != city->marketFactors.end())
        {
            marketFactor = it->second;
        }

        int localPrice = good.calculatePrice(marketFactor);

        std::vector<std::string> row = {
            std::to_string(good.getId()),
            good.getName(),
            good.getCategory(),
            std::to_string(localPrice),
            std::to_string(good.getWeight())};

        data.push_back(row);
    }

    std::vector<int> columnWidths = {4, 15, 12, 6, 6};
    m_ui.displayTable(headers, data, columnWidths);

    m_ui.displayMessage("");

    // Demander au joueur l'ID de la marchandise à acheter
    int goodId = m_ui.getIntInput("Entrez l'ID de la marchandise à acheter (0 pour annuler): ", 0,
                                  static_cast<int>(goods.size()));

    if (goodId == 0)
    {
        return;
    }

    // Demander la quantité
    int quantity = m_ui.getIntInput("Quantité à acheter: ", 1, 1000);

    // Trouver la marchandise
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
    {
        m_ui.displayError("Marchandise non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Calculer le prix total
    float marketFactor = 1.0f;
    auto it = city->marketFactors.find(goodId);
    if (it != city->marketFactors.end())
    {
        marketFactor = it->second;
    }

    int pricePerUnit = good->calculatePrice(marketFactor);
    int totalPrice = pricePerUnit * quantity;
    int totalWeight = good->getWeight() * quantity;

    // Vérifier si le joueur a assez d'argent
    if (totalPrice > m_player->getGold())
    {
        m_ui.displayError("Vous n'avez pas assez d'or.");
        m_ui.waitForKeyPress();
        return;
    }

    // Vérifier si le navire a assez d'espace
    if (totalWeight > m_player->getRemainingCargoCapacity())
    {
        m_ui.displayError("Votre navire n'a pas assez d'espace de cargo.");
        m_ui.waitForKeyPress();
        return;
    }

    // Confirmer l'achat
    m_ui.displayMessage("Vous êtes sur le point d'acheter " + std::to_string(quantity) + " " +
                        good->getName() + " pour " + std::to_string(totalPrice) + " or.");

    std::string confirmation = m_ui.getInput("Confirmer l'achat ? (o/n): ");

    if (confirmation == "o" || confirmation == "O")
    {
        // Effectuer l'achat
        m_player->removeGold(totalPrice);
        m_player->addToCargo(goodId, quantity);

        m_ui.displayMessage("Achat effectué avec succès!");
    }

    m_ui.waitForKeyPress();
}

// Menu de vente
void Game::tradeSellMenu()
{
    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Erreur: Ville non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("VENDRE - " + city->name);

    // Afficher l'or actuel
    m_ui.displayMessage("Or actuel: " + std::to_string(m_player->getGold()));
    m_ui.displayMessage("");

    // Afficher la cargaison actuelle
    const auto &cargo = m_player->getCargo();

    if (cargo.empty())
    {
        m_ui.displayError("Votre cargaison est vide.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Votre cargaison:");

    std::vector<std::string> headers = {"ID", "Nom", "Quantité", "Prix unitaire", "Prix total"};
    std::vector<std::vector<std::string>> data;

    for (const auto &[goodId, quantity] : cargo)
    {
        const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
        if (!good)
            continue;

        // Calculer le prix local
        float marketFactor = 1.0f;
        auto it = city->marketFactors.find(goodId);
        if (it != city->marketFactors.end())
        {
            marketFactor = it->second;
        }

        // Le prix de vente est 85% du prix d'achat
        int localPrice = static_cast<int>(good->calculatePrice(marketFactor) * 0.85f);
        int totalPrice = localPrice * quantity;

        std::vector<std::string> row = {
            std::to_string(goodId),
            good->getName(),
            std::to_string(quantity),
            std::to_string(localPrice),
            std::to_string(totalPrice)};

        data.push_back(row);
    }

    std::vector<int> columnWidths = {4, 15, 8, 12, 10};
    m_ui.displayTable(headers, data, columnWidths);

    m_ui.displayMessage("");

    // Demander au joueur l'ID de la marchandise à vendre
    int goodId = m_ui.getIntInput("Entrez l'ID de la marchandise à vendre (0 pour annuler): ", 0, 1000);

    if (goodId == 0)
    {
        return;
    }

    // Vérifier si le joueur possède cette marchandise
    int availableQuantity = m_player->getCargoQuantity(goodId);
    if (availableQuantity <= 0)
    {
        m_ui.displayError("Vous ne possédez pas cette marchandise.");
        m_ui.waitForKeyPress();
        return;
    }

    // Demander la quantité
    int quantity = m_ui.getIntInput("Quantité à vendre (max " + std::to_string(availableQuantity) + "): ",
                                    1, availableQuantity);

    // Trouver la marchandise
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
    {
        m_ui.displayError("Marchandise non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Calculer le prix total
    float marketFactor = 1.0f;
    auto it = city->marketFactors.find(goodId);
    if (it != city->marketFactors.end())
    {
        marketFactor = it->second;
    }

    // Le prix de vente est 85% du prix d'achat
    int pricePerUnit = static_cast<int>(good->calculatePrice(marketFactor) * 0.85f);
    int totalPrice = pricePerUnit * quantity;

    // Confirmer la vente
    m_ui.displayMessage("Vous êtes sur le point de vendre " + std::to_string(quantity) + " " +
                        good->getName() + " pour " + std::to_string(totalPrice) + " or.");

    std::string confirmation = m_ui.getInput("Confirmer la vente ? (o/n): ");

    if (confirmation == "o" || confirmation == "O")
    {
        // Effectuer la vente
        m_player->addGold(totalPrice);
        m_player->removeFromCargo(goodId, quantity);

        m_ui.displayMessage("Vente effectuée avec succès!");
    }

    m_ui.waitForKeyPress();
}

// Menu du chantier naval
void Game::shipyardMenu()
{
    m_ui.displayTitle("CHANTIER NAVAL");

    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Erreur: Ville non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Bienvenue au chantier naval de " + city->name + "!");
    m_ui.displayMessage("Services disponibles:");
    m_ui.displayMessage("1. Acheter un nouveau navire");
    m_ui.displayMessage("2. Réparer votre navire actuel");
    m_ui.displayMessage("3. Améliorer votre navire actuel");

    m_ui.displayMessage("\nFonctionnalité non implémentée.");
    m_ui.waitForKeyPress();
}

// Menu de la taverne
void Game::tavernMenu()
{
    m_ui.displayTitle("TAVERNE");

    // Obtenir la ville actuelle
    const City *city = World::getInstance().getCityAtPosition(
        m_player->getPosX(), m_player->getPosY());

    if (!city)
    {
        m_ui.displayError("Erreur: Ville non trouvée.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Bienvenue à la taverne de " + city->name + "!");
    m_ui.displayMessage("Services disponibles:");
    m_ui.displayMessage("1. Écouter les rumeurs");
    m_ui.displayMessage("2. Recruter des marins");
    m_ui.displayMessage("3. Accepter des quêtes");

    m_ui.displayMessage("\nFonctionnalité non implémentée.");
    m_ui.waitForKeyPress();
}

// Menu de gestion de la flotte
void Game::fleetManagementMenu()
{
    if (!m_player)
    {
        m_ui.displayError("Erreur: Joueur non initialisé.");
        m_ui.waitForKeyPress();
        return;
    }

    // Vérifier si le joueur est en mode maritime
    if (m_player->getCurrentMode() != PlayerMode::SEA)
    {
        m_ui.displayMessage("Vous devez être en mode navigation maritime pour gérer votre flotte.");
        m_ui.displayMessage("Utilisez la commande 'changer mode' pour passer en mode maritime.");
        m_ui.waitForKeyPress();
        return;
    }

    // Créer le gestionnaire de flotte et afficher le menu
    FleetManager fleetManager(m_ui, m_player);
    fleetManager.showFleetManagementMenu();
}