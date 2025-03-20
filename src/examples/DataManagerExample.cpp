#include "../../include/data/DataUtils.hpp"
#include <iostream>
#include <string>

using namespace Orenji;

// Exemple d'utilisation du DataManager
void runDataManagerExample()
{
    std::cout << "=== Exemple d'utilisation du DataManager ===" << std::endl;

    // Initialisation du système de données
    if (!DataUtils::initialize("./resources/data/"))
    {
        std::cerr << "Erreur lors de l'initialisation du système de données" << std::endl;
        return;
    }

    std::cout << "Initialisation réussie !" << std::endl;

    // === Création et manipulation de personnages ===
    std::cout << "\n--- Gestion des personnages ---" << std::endl;

    // Création de quelques personnages
    Character captain;
    captain.setName("Capitaine Roberts");
    captain.setAge(42);
    captain.setStrength(75);
    captain.setDexterity(65);
    captain.setIntelligence(80);
    captain.setCharisma(90);
    int captainId = DataUtils::addCharacter(captain);

    Character navigator;
    navigator.setName("Mary Reed");
    navigator.setAge(35);
    navigator.setStrength(55);
    navigator.setDexterity(85);
    navigator.setIntelligence(95);
    navigator.setCharisma(70);
    int navigatorId = DataUtils::addCharacter(navigator);

    Character boatswain;
    boatswain.setName("John Silver");
    boatswain.setAge(50);
    boatswain.setStrength(90);
    boatswain.setDexterity(60);
    boatswain.setIntelligence(65);
    boatswain.setCharisma(80);
    int boatswainId = DataUtils::addCharacter(boatswain);

    std::cout << "Personnages créés avec les IDs: " << captainId << ", " << navigatorId << ", " << boatswainId << std::endl;

    // Récupération et affichage de tous les personnages
    auto characters = DataUtils::getAllCharacters();
    std::cout << "Nombre de personnages: " << characters.size() << std::endl;

    for (const auto &character : characters)
    {
        std::cout << "- " << character->getName() << " (ID: " << character->getId()
                  << ", Force: " << character->getStrength()
                  << ", Intelligence: " << character->getIntelligence() << ")" << std::endl;
    }

    // Récupération des personnages par ID
    auto captainCharacter = DataUtils::getCharacter(captainId);
    auto navigatorCharacter = DataUtils::getCharacter(navigatorId);
    auto boatswainCharacter = DataUtils::getCharacter(boatswainId);

    if (captainCharacter)
    {
        std::cout << "\nRécupération du capitaine: " << captainCharacter->getName() << std::endl;

        // Modification du personnage
        captainCharacter->setExperience(1500);
        captainCharacter->setLevel(5);
        DataUtils::updateCharacter(*captainCharacter);

        std::cout << "Capitaine mis à jour - Niveau: " << captainCharacter->getLevel()
                  << ", XP: " << captainCharacter->getExperience() << std::endl;
    }

    // Filtrage des personnages par critère
    auto smartCharacters = DataUtils::getCharactersWhere([](const Character &c)
                                                         { return c.getIntelligence() > 70; });

    std::cout << "\nPersonnages avec une intelligence > 70:" << std::endl;
    for (const auto &character : smartCharacters)
    {
        std::cout << "- " << character->getName() << " (Intelligence: " << character->getIntelligence() << ")" << std::endl;
    }

    // === Création et manipulation de navires ===
    std::cout << "\n--- Gestion des navires ---" << std::endl;

    // Création de quelques navires
    Ship galleon;
    galleon.setName("La Vengeance");
    galleon.setType(Ship::Type::GALLEON);
    galleon.setMaxHealth(1000);
    galleon.setHealth(1000);
    galleon.setMaxSpeed(12.0f);
    galleon.setSpeed(10.0f);
    galleon.setMaxCapacity(500);
    galleon.setCapacity(0);
    galleon.setMaxCrewCapacity(50);
    galleon.setCrewCapacity(0);
    galleon.setPosition(sf::Vector2f(500.0f, 300.0f));
    int galleonId = DataUtils::addShip(galleon);

    Ship brigantine;
    brigantine.setName("L'Étoile du Matin");
    brigantine.setType(Ship::Type::BRIGANTINE);
    brigantine.setMaxHealth(750);
    brigantine.setHealth(750);
    brigantine.setMaxSpeed(16.0f);
    brigantine.setSpeed(14.0f);
    brigantine.setMaxCapacity(300);
    brigantine.setCapacity(0);
    brigantine.setMaxCrewCapacity(30);
    brigantine.setCrewCapacity(0);
    brigantine.setPosition(sf::Vector2f(550.0f, 320.0f));
    int brigantineId = DataUtils::addShip(brigantine);

    std::cout << "Navires créés avec les IDs: " << galleonId << ", " << brigantineId << std::endl;

    // Ajout d'équipage aux navires
    auto galleonShip = DataUtils::getShip(galleonId);
    if (galleonShip && captainCharacter)
    {
        galleonShip->addCrewMember(captainCharacter, Ship::CrewRole::CAPTAIN);
        std::cout << captainCharacter->getName() << " ajouté comme capitaine de " << galleonShip->getName() << std::endl;
    }

    auto brigantineShip = DataUtils::getShip(brigantineId);
    if (brigantineShip && navigatorCharacter && boatswainCharacter)
    {
        brigantineShip->addCrewMember(navigatorCharacter, Ship::CrewRole::NAVIGATOR);
        brigantineShip->addCrewMember(boatswainCharacter, Ship::CrewRole::BOATSWAIN);
        std::cout << navigatorCharacter->getName() << " et " << boatswainCharacter->getName()
                  << " ajoutés à l'équipage de " << brigantineShip->getName() << std::endl;
    }

    // === Création et manipulation de biens commerciaux ===
    std::cout << "\n--- Gestion des biens commerciaux ---" << std::endl;

    // Création de quelques biens commerciaux
    TradeGood silk;
    silk.setName("Soie");
    silk.setBasePrice(100);
    silk.setWeight(0.1f);
    silk.setRarity(TradeGood::Rarity::RARE);
    silk.setCategory(TradeGood::Category::TEXTILE);
    int silkId = DataUtils::addTradeGood(silk);

    TradeGood spices;
    spices.setName("Épices");
    spices.setBasePrice(75);
    spices.setWeight(0.2f);
    spices.setRarity(TradeGood::Rarity::COMMON);
    spices.setCategory(TradeGood::Category::FOOD);
    int spicesId = DataUtils::addTradeGood(spices);

    TradeGood rum;
    rum.setName("Rhum");
    rum.setBasePrice(50);
    rum.setWeight(1.0f);
    rum.setRarity(TradeGood::Rarity::UNCOMMON);
    rum.setCategory(TradeGood::Category::BEVERAGE);
    int rumId = DataUtils::addTradeGood(rum);

    std::cout << "Biens commerciaux créés avec les IDs: " << silkId << ", " << spicesId << ", " << rumId << std::endl;

    // Ajout de cargaison aux navires
    if (galleonShip)
    {
        galleonShip->addCargo(silkId, 50);
        galleonShip->addCargo(spicesId, 100);
        galleonShip->addCargo(rumId, 200);

        DataUtils::updateShip(*galleonShip);

        std::cout << "Cargaison ajoutée à " << galleonShip->getName() << " - Capacité: "
                  << galleonShip->getCapacity() << "/" << galleonShip->getMaxCapacity() << std::endl;
    }

    // === Création et manipulation de flottes ===
    std::cout << "\n--- Gestion des flottes ---" << std::endl;

    // Création d'une flotte
    Fleet merchantFleet;
    merchantFleet.setName("Flotte Marchande");
    merchantFleet.setPosition(sf::Vector2f(500.0f, 300.0f));

    int fleetId = DataUtils::addFleet(merchantFleet);
    std::cout << "Flotte créée avec l'ID: " << fleetId << std::endl;

    // Ajout de navires à la flotte
    auto fleet = DataUtils::getFleet(fleetId);
    if (fleet)
    {
        fleet->addShip(galleonId);
        fleet->addShip(brigantineId);

        DataUtils::updateFleet(*fleet);

        std::cout << "Navires ajoutés à la flotte " << fleet->getName() << std::endl;

        // Récupération des navires de la flotte
        auto shipIds = fleet->getShipIds();
        std::cout << "Nombre de navires dans la flotte: " << shipIds.size() << std::endl;

        for (int shipId : shipIds)
        {
            auto ship = DataUtils::getShip(shipId);
            if (ship)
            {
                std::cout << "- " << ship->getName() << " (Type: " << static_cast<int>(ship->getType()) << ")" << std::endl;
            }
        }
    }

    // === Création et manipulation du joueur ===
    std::cout << "\n--- Gestion du joueur ---" << std::endl;

    // Configuration du joueur
    Player &player = DataUtils::getPlayer();
    player.setName("Jean Lafitte");
    player.setMoney(5000);
    player.setReputation(75);
    player.setExperience(2500);
    player.setLevel(8);

    // Ajout de la flotte au joueur
    player.addFleet(fleetId);

    // Ajout d'une quête
    Player::Quest mainQuest;
    mainQuest.setId(1);
    mainQuest.setTitle("À la recherche du trésor perdu");
    mainQuest.setDescription("Retrouvez le trésor légendaire du capitaine Kidd.");
    mainQuest.setStatus(Player::QuestStatus::IN_PROGRESS);
    mainQuest.setReward(10000);
    player.addQuest(mainQuest);

    DataUtils::updatePlayer(player);

    std::cout << "Joueur créé: " << player.getName() << " (Niveau: " << player.getLevel()
              << ", Argent: " << player.getMoney() << ")" << std::endl;
    std::cout << "Nombre de flottes du joueur: " << player.getFleetIds().size() << std::endl;
    std::cout << "Nombre de quêtes du joueur: " << player.getQuests().size() << std::endl;

    // === Création et manipulation du monde ===
    std::cout << "\n--- Gestion du monde ---" << std::endl;

    // Configuration du monde
    World &world = DataUtils::getWorld();
    world.setName("Les Caraïbes");
    world.setWidth(2000);
    world.setHeight(2000);

    // Ajout de villes
    World::City portRoyal;
    portRoyal.setId(1);
    portRoyal.setName("Port Royal");
    portRoyal.setPosition(sf::Vector2f(800.0f, 600.0f));
    portRoyal.setPopulation(5000);
    portRoyal.setWealth(8000);

    // Définition des prix du marché
    World::MarketData silkMarket;
    silkMarket.price = 120;
    silkMarket.supply = 50;
    silkMarket.demand = 80;
    portRoyal.addMarketData(silkId, silkMarket);

    World::MarketData spicesMarket;
    spicesMarket.price = 60;
    spicesMarket.supply = 200;
    spicesMarket.demand = 150;
    portRoyal.addMarketData(spicesId, spicesMarket);

    world.addCity(portRoyal);

    World::City havana;
    havana.setId(2);
    havana.setName("La Havane");
    havana.setPosition(sf::Vector2f(500.0f, 400.0f));
    havana.setPopulation(8000);
    havana.setWealth(12000);

    World::MarketData rumMarket;
    rumMarket.price = 40;
    rumMarket.supply = 300;
    rumMarket.demand = 250;
    havana.addMarketData(rumId, rumMarket);

    world.addCity(havana);

    DataUtils::updateWorld(world);

    std::cout << "Monde créé: " << world.getName() << " (Taille: " << world.getWidth() << "x" << world.getHeight() << ")" << std::endl;
    std::cout << "Nombre de villes: " << world.getCities().size() << std::endl;

    for (const auto &city : world.getCities())
    {
        std::cout << "- " << city.getName() << " (Population: " << city.getPopulation()
                  << ", Richesse: " << city.getWealth() << ")" << std::endl;

        std::cout << "  Marché: " << std::endl;
        for (const auto &[goodId, marketData] : city.getMarket())
        {
            auto good = DataUtils::getTradeGood(goodId);
            if (good)
            {
                std::cout << "    " << good->getName() << " - Prix: " << marketData.price
                          << ", Offre: " << marketData.supply
                          << ", Demande: " << marketData.demand << std::endl;
            }
        }
    }

    // Sauvegarde des données
    std::cout << "\n--- Sauvegarde des données ---" << std::endl;
    if (DataUtils::saveAllData())
    {
        std::cout << "Données sauvegardées avec succès !" << std::endl;
    }
    else
    {
        std::cerr << "Erreur lors de la sauvegarde des données" << std::endl;
    }
}

int main()
{
    runDataManagerExample();
    return 0;
}