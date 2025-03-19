#include "TradingUI.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

TradingUI::TradingUI(Player &player, ConsoleUI &ui, TradingSystem &tradingSystem)
    : m_player(player), m_ui(ui), m_tradingSystem(tradingSystem)
{
}

void TradingUI::showTradingMenu(int cityId)
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen (for Windows)

        // Display trading menu title
        std::cout << "=== MENU DE COMMERCE : " << World().getCityName(cityId) << " ===" << std::endl;
        std::cout << "Or disponible : " << m_player.getGold() << " pièces" << std::endl;
        std::cout << "Capacité de cargo : " << m_player.getRemainingCargoCapacity() << "/" << m_player.getTotalCargoCapacity() << std::endl;
        std::cout << std::endl;

        // Menu options
        std::vector<std::string> options = {
            "Voir le marché (acheter/vendre)",
            "Voir les routes commerciales",
            "Comparer les prix",
            "Relations commerciales",
            "Événements commerciaux actuels",
            "Gérer vos compétences commerciales"};

        // Ajouter l'option marché noir si disponible
        if (m_tradingSystem.hasBlackMarket(cityId, m_player))
        {
            options.push_back("Accéder au marché noir");
        }

        options.push_back("Retour au menu principal");

        int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

        // Déterminer si le choix est "Retour" en fonction du nombre d'options
        bool isExitChoice = (choice == options.size() - 1);

        if (isExitChoice)
        {
            exitMenu = true;
        }
        else if (choice == 0) // Market
        {
            showMarketScreen(cityId);
        }
        else if (choice == 1) // Trade routes
        {
            showTradeRoutesScreen(cityId);
        }
        else if (choice == 2) // Price comparison
        {
            showPriceComparisonScreen(cityId);
        }
        else if (choice == 3) // Kingdom relations
        {
            showKingdomRelationsScreen();
        }
        else if (choice == 4) // Trade events
        {
            showTradeEventsScreen();
        }
        else if (choice == 5) // Trade skills
        {
            showTradeSkillsScreen();
        }
        else if (m_tradingSystem.hasBlackMarket(cityId, m_player) && choice == 6) // Black market
        {
            showBlackMarketScreen(cityId);
        }
    }
}

void TradingUI::showMarketScreen(int cityId)
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display market title
        std::cout << "=== MARCHÉ DE " << World().getCityName(cityId) << " ===" << std::endl;

        // Display market info
        displayMarketInfo(cityId);

        std::cout << std::endl;

        // Menu options
        std::vector<std::string> options = {
            "Acheter des marchandises",
            "Vendre des marchandises",
            "Voir votre cargaison",
            "Voir les tendances de prix",
            "Retour au menu de commerce"};

        int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

        switch (choice)
        {
        case 0: // Buy
            handleBuyGoods(cityId);
            break;

        case 1: // Sell
            handleSellGoods(cityId);
            break;

        case 2: // View cargo
            displayPlayerCargo();
            break;

        case 3: // View price trends
            // Demander quelle marchandise le joueur souhaite voir
            std::vector<GoodPrice> prices = m_tradingSystem.getCityPrices(cityId);
            std::vector<std::string> goodOptions;

            for (const auto &price : prices)
            {
                const TradeGood *good = GameData::getInstance().findTradeGoodById(price.goodId);
                if (good)
                {
                    goodOptions.push_back(good->getName());
                }
            }

            int goodChoice = m_ui.displayMenu("Quelle marchandise souhaitez-vous analyser ?", goodOptions);
            if (goodChoice >= 0 && goodChoice < prices.size())
            {
                displayPriceHistory(cityId, prices[goodChoice].goodId);
            }
            break;

        case 4: // Exit
            exitMenu = true;
            break;

        default:
            break;
        }
    }
}

void TradingUI::showTradeRoutesScreen(int cityId)
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display trade routes title
        std::cout << "=== ROUTES COMMERCIALES ===" << std::endl;

        std::vector<std::string> options = {
            "Voir les routes commerciales actuelles",
            "Créer une nouvelle route commerciale",
            "Voir les routes les plus rentables",
            "Retour au menu de commerce"};

        int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

        switch (choice)
        {
        case 0: // View existing routes
            manageExistingRoutes();
            break;

        case 1: // Create new route
            createTradeRoute(cityId);
            break;

        case 2: // View profitable routes
            displayProfitableRoutes(cityId);
            break;

        case 3: // Exit
            exitMenu = true;
            break;

        default:
            break;
        }
    }
}

void TradingUI::showPriceComparisonScreen(int cityId)
{
    // Get current market
    Market *market = TradingSystem::getInstance().getMarket(cityId);
    if (!market)
    {
        return;
    }

    // Get goods from this market
    auto prices = market->getAllPrices();

    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display comparison title
        std::cout << "=== COMPARAISON DES PRIX DES MARCHANDISES ===" << std::endl;
        std::cout << "Sélectionnez une marchandise pour comparer son prix dans différents marchés." << std::endl
                  << std::endl;

        // Create options from goods
        std::vector<std::string> options;
        for (const auto &price : prices)
        {
            if (price.goodId > 0) // Skip invalid goods
            {
                options.push_back(price.goodName);
            }
        }
        options.push_back("Retour au menu de commerce");

        int choice = m_ui.displayMenu("Choisissez une marchandise :", options);

        if (choice >= 0 && choice < static_cast<int>(prices.size()))
        {
            displayPriceComparison(prices[choice].goodId);
        }
        else
        {
            exitMenu = true;
        }
    }
}

void TradingUI::showKingdomRelationsScreen()
{
    system("cls"); // Clear the screen

    // Display kingdom relations title
    std::cout << "=== RELATIONS COMMERCIALES AVEC LES ROYAUMES ===" << std::endl;

    // Display kingdom relations
    displayKingdomRelations();

    std::cout << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour retourner au menu de commerce..." << std::endl;
    std::cin.get();
}

void TradingUI::showTradeEventsScreen()
{
    system("cls"); // Clear the screen

    // Display trade events title
    std::cout << "=== ÉVÉNEMENTS COMMERCIAUX ACTUELS ===" << std::endl;

    displayActiveEvents();

    std::cout << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour retourner au menu de commerce..." << std::endl;
    std::cin.get();
}

void TradingUI::showTradeSkillsScreen()
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display trade skills title
        std::cout << "=== COMPÉTENCES COMMERCIALES ===" << std::endl;

        displayTradeSkills();

        std::vector<std::string> options = {
            "Améliorer une compétence",
            "Retour au menu de commerce"};

        int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

        switch (choice)
        {
        case 0: // Improve skill
            improveTradeSkills();
            break;

        case 1: // Exit
            exitMenu = true;
            break;

        default:
            break;
        }
    }
}

void TradingUI::showBlackMarketScreen(int cityId)
{
    system("cls"); // Clear the screen

    // Display black market title
    std::cout << "=== MARCHÉ NOIR DE " << World().getCityName(cityId) << " ===" << std::endl;

    displayBlackMarket(cityId);

    std::cout << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour retourner au menu de commerce..." << std::endl;
    std::cin.get();
}

void TradingUI::displayMarketInfo(int cityId)
{
    const World world;
    auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);

    if (cityIt != cities.end())
    {
        const auto &city = cityIt->second;

        std::cout << "Région : " << city.region << std::endl;
        std::cout << "Population : " << city.population << std::endl;
        std::cout << "Réputation dans cette ville : " << m_player.getCityReputation(cityId) << std::endl;
        std::cout << "Réputation dans ce royaume : " << m_player.getKingdomReputation(city.region) << std::endl;

        // Afficher le bonus de négociation
        std::cout << "Bonus de négociation : " << (m_player.getTradeSkills().negotiation * 5) << "%" << std::endl;

        // Événements affectant cette ville
        bool hasEvents = false;
        for (const auto &event : m_tradingSystem.getActiveEvents())
        {
            if (std::find(event.affectedRegions.begin(), event.affectedRegions.end(), city.region) != event.affectedRegions.end())
            {
                if (!hasEvents)
                {
                    std::cout << "\nÉvénements actifs :" << std::endl;
                    hasEvents = true;
                }
                std::cout << "- " << event.description << std::endl;
            }
        }
    }
}

void TradingUI::displayTradeGoods(int cityId, bool buying)
{
    // Titre
    std::cout << (buying ? "=== ACHAT DE MARCHANDISES ===" : "=== VENTE DE MARCHANDISES ===") << std::endl;

    // En-tête du tableau
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Nom"
              << std::setw(10) << "Prix" << std::setw(10) << "Stock"
              << std::setw(12) << "Tendance" << std::endl;
    std::cout << std::string(57, '-') << std::endl;

    // Récupérer les prix dans cette ville
    std::vector<GoodPrice> prices = m_tradingSystem.getCityPrices(cityId);

    for (size_t i = 0; i < prices.size(); ++i)
    {
        const auto &price = prices[i];
        const TradeGood *good = GameData::getInstance().findTradeGoodById(price.goodId);

        if (!good)
            continue;

        // Si on vend, ne montrer que les marchandises que le joueur possède
        if (!buying && !m_player.hasCargo(price.goodId, 1))
            continue;

        std::string name = good->getName();
        int currentPrice = price.getCurrentPrice();

        // Appliquer le bonus de négociation
        if (buying)
        {
            // Réduction du prix d'achat (-5% par niveau)
            float negotiationBonus = 1.0f - (m_player.getTradeSkills().negotiation * 0.05f);
            currentPrice = static_cast<int>(currentPrice * negotiationBonus);
        }
        else
        {
            // Augmentation du prix de vente (+5% par niveau)
            float negotiationBonus = 1.0f + (m_player.getTradeSkills().negotiation * 0.05f);
            currentPrice = static_cast<int>(currentPrice * negotiationBonus);
        }

        // Afficher la tendance
        float trend = m_tradingSystem.getPriceTrend(cityId, price.goodId);
        std::string trendStr;

        if (trend > 1.0f)
            trendStr = "↑↑↑";
        else if (trend > 0.5f)
            trendStr = "↑↑";
        else if (trend > 0.1f)
            trendStr = "↑";
        else if (trend < -1.0f)
            trendStr = "↓↓↓";
        else if (trend < -0.5f)
            trendStr = "↓↓";
        else if (trend < -0.1f)
            trendStr = "↓";
        else
            trendStr = "→";

        std::cout << std::left << std::setw(5) << i
                  << std::setw(20) << name
                  << std::setw(10) << currentPrice
                  << std::setw(10) << price.currentStock
                  << std::setw(12) << trendStr << std::endl;
    }
}

void TradingUI::displayActiveEvents()
{
    const auto &events = m_tradingSystem.getActiveEvents();

    if (events.empty())
    {
        std::cout << "Aucun événement commercial actif actuellement." << std::endl;
        return;
    }

    std::cout << std::left << std::setw(30) << "Type"
              << std::setw(40) << "Description"
              << std::setw(15) << "Effet"
              << std::setw(10) << "Durée" << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    for (const auto &event : events)
    {
        std::string type = TradeEvent::getTypeName(event.type);
        std::string effect;

        if (event.priceModifier > 1.0f)
        {
            effect = "+" + std::to_string(static_cast<int>((event.priceModifier - 1.0f) * 100.0f)) + "% prix";
        }
        else
        {
            effect = "-" + std::to_string(static_cast<int>((1.0f - event.priceModifier) * 100.0f)) + "% prix";
        }

        std::cout << std::left << std::setw(30) << type
                  << std::setw(40) << event.description
                  << std::setw(15) << effect
                  << std::setw(10) << event.duration << " jours" << std::endl;

        // Afficher les régions affectées
        std::cout << "  Régions : ";
        for (size_t i = 0; i < event.affectedRegions.size(); ++i)
        {
            std::cout << event.affectedRegions[i];
            if (i < event.affectedRegions.size() - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;

        // Afficher les marchandises affectées
        if (!event.affectedGoods.empty())
        {
            std::cout << "  Marchandises : ";
            for (size_t i = 0; i < event.affectedGoods.size(); ++i)
            {
                const TradeGood *good = GameData::getInstance().findTradeGoodById(event.affectedGoods[i]);
                if (good)
                {
                    std::cout << good->getName();
                    if (i < event.affectedGoods.size() - 1)
                    {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

void TradingUI::displayTradeSkills()
{
    const auto &skills = m_player.getTradeSkills();

    std::cout << "Voici vos compétences commerciales actuelles :" << std::endl;
    std::cout << std::endl;

    std::cout << "Négociation : " << skills.negotiation << "/10" << std::endl;
    std::cout << "  Effet : " << (skills.negotiation * 5) << "% de réduction à l'achat, "
              << (skills.negotiation * 5) << "% d'augmentation à la vente" << std::endl;
    std::cout << std::endl;

    std::cout << "Logistique : " << skills.logistics << "/10" << std::endl;
    std::cout << "  Effet : " << (skills.logistics * 10) << "% de capacité de cargo supplémentaire" << std::endl;
    std::cout << std::endl;

    std::cout << "Contrebande : " << skills.smuggling << "/10" << std::endl;
    std::cout << "  Effet : " << (skills.smuggling * 10 + 10) << "% de chance d'accéder aux marchés noirs" << std::endl;
    std::cout << std::endl;

    std::cout << "Influence : " << skills.influence << "/10" << std::endl;
    std::cout << "  Effet : Débloque des opportunités commerciales spéciales" << std::endl;
    std::cout << std::endl;

    std::cout << "Points d'expérience disponibles : " << m_player.getExperience() << std::endl;
}

void TradingUI::improveTradeSkills()
{
    const auto &skills = m_player.getTradeSkills();

    std::vector<std::string> options = {
        "Négociation (" + std::to_string(skills.negotiation) + "/10)",
        "Logistique (" + std::to_string(skills.logistics) + "/10)",
        "Contrebande (" + std::to_string(skills.smuggling) + "/10)",
        "Influence (" + std::to_string(skills.influence) + "/10)",
        "Retour"};

    int choice = m_ui.displayMenu("Quelle compétence souhaitez-vous améliorer ?", options);

    if (choice < 0 || choice >= 4) // Retour ou choix invalide
    {
        return;
    }

    // Vérifier si le joueur a assez d'XP (coût = 100 * niveau actuel)
    const char *skillNames[] = {"negotiation", "logistics", "smuggling", "influence"};
    int currentLevel = 0;

    if (choice == 0)
        currentLevel = skills.negotiation;
    else if (choice == 1)
        currentLevel = skills.logistics;
    else if (choice == 2)
        currentLevel = skills.smuggling;
    else if (choice == 3)
        currentLevel = skills.influence;

    if (currentLevel >= 10)
    {
        std::cout << "Cette compétence est déjà au niveau maximum !" << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    int cost = 100 * (currentLevel + 1);

    if (m_player.getExperience() < cost)
    {
        std::cout << "Vous n'avez pas assez d'expérience !" << std::endl;
        std::cout << "Coût : " << cost << " XP, Disponible : " << m_player.getExperience() << " XP" << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    // Améliorer la compétence
    m_player.improveTradeSkill(skillNames[choice], 1);
    m_player.addExperience(-cost);

    std::cout << "Vous avez amélioré votre compétence " << options[choice] << " !" << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
    std::cin.get();
}

void TradingUI::displayPlayerCargo()
{
    system("cls"); // Clear the screen

    std::cout << "=== VOTRE CARGAISON ===" << std::endl;
    std::cout << "Capacité : " << m_player.getRemainingCargoCapacity() << "/" << m_player.getTotalCargoCapacity() << std::endl;
    std::cout << std::endl;

    const auto &cargo = m_player.getCargo();

    if (cargo.empty())
    {
        std::cout << "Votre cargaison est vide." << std::endl;
    }
    else
    {
        // Table header
        std::cout << std::left << std::setw(30) << "MARCHANDISE"
                  << std::setw(15) << "QUANTITÉ"
                  << std::setw(15) << "POIDS UNITAIRE"
                  << "POIDS TOTAL" << std::endl;

        std::cout << std::string(80, '-') << std::endl;

        int totalWeight = 0;

        for (const auto &item : cargo)
        {
            int itemTotalWeight = item.quantity * item.unitWeight;
            totalWeight += itemTotalWeight;

            std::cout << std::left << std::setw(30) << item.name
                      << std::setw(15) << item.quantity
                      << std::setw(15) << item.unitWeight
                      << itemTotalWeight << std::endl;
        }

        std::cout << std::string(80, '-') << std::endl;
        std::cout << "Poids total : " << totalWeight << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
    std::cin.get();
}

void TradingUI::displayTradeRoutes(int locationId)
{
    // Get profitable routes
    auto routes = TradingSystem::getInstance().findProfitableRoutes(locationId, 10);

    if (routes.empty())
    {
        std::cout << "Aucune route commerciale disponible depuis cet emplacement." << std::endl;
        return;
    }

    // Table header
    std::cout << std::left << std::setw(30) << "DESTINATION"
              << std::setw(15) << "DISTANCE"
              << std::setw(15) << "RISQUE"
              << "PROFIT ATTENDU" << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    for (const auto &route : routes)
    {
        // Format risk percentage
        std::ostringstream riskStr;
        riskStr << std::fixed << std::setprecision(1) << (route.risk * 100.0f) << "%";

        // Display route info
        std::cout << std::left << std::setw(30) << route.destName
                  << std::setw(15) << std::fixed << std::setprecision(0) << route.distance
                  << std::setw(15) << riskStr.str()
                  << route.expectedProfit << " or" << std::endl;
    }
}

void TradingUI::handleBuyGoods(int cityId)
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display buy goods title
        std::cout << "=== ACHETER DES MARCHANDISES ===" << std::endl;
        std::cout << "Or disponible : " << m_player.getGold() << " pièces" << std::endl;
        std::cout << "Capacité restante : " << m_player.getRemainingCargoCapacity() << "/" << m_player.getTotalCargoCapacity() << std::endl;
        std::cout << std::endl;

        // Display goods table
        displayTradeGoods(cityId, true);

        std::cout << std::endl;

        // Get all prices
        std::vector<GoodPrice> prices = m_tradingSystem.getCityPrices(cityId);

        // Create options from goods
        std::vector<std::string> options;
        for (const auto &price : prices)
        {
            if (price.goodId > 0) // Skip invalid goods
            {
                options.push_back(GameData::getInstance().findTradeGoodById(price.goodId)->getName());
            }
        }
        options.push_back("Retour au menu du marché");

        int choice = m_ui.displayMenu("Quelle marchandise souhaitez-vous acheter ?", options);

        if (choice >= 0 && choice < static_cast<int>(prices.size()))
        {
            // Ask for quantity
            std::cout << "Quantité à acheter (0 pour annuler) : ";
            int quantity;
            std::cin >> quantity;
            std::cin.ignore(); // Clear newline from input buffer

            if (quantity > 0)
            {
                // Try to buy the goods
                bool success = m_tradingSystem.buyGoods(m_player, prices[choice].goodId, quantity);

                if (success)
                {
                    std::cout << "Achat réussi ! Vous avez acheté " << quantity << " "
                              << GameData::getInstance().findTradeGoodById(prices[choice].goodId)->getName() << " pour "
                              << (prices[choice].getCurrentPrice() * quantity) << " or." << std::endl;
                }
                else
                {
                    std::cout << "Achat impossible. Vérifiez votre or, la capacité de cargo ou la disponibilité." << std::endl;
                }

                std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
                std::cin.get();
            }
        }
        else
        {
            exitMenu = true;
        }
    }
}

void TradingUI::handleSellGoods(int cityId)
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display sell goods title
        std::cout << "=== VENDRE DES MARCHANDISES ===" << std::endl;
        std::cout << "Or disponible : " << m_player.getGold() << " pièces" << std::endl;
        std::cout << std::endl;

        // Display goods table
        displayTradeGoods(cityId, false);

        std::cout << std::endl;

        // Get all prices
        std::vector<GoodPrice> prices = m_tradingSystem.getCityPrices(cityId);

        // Create options from goods
        std::vector<std::string> options;
        for (const auto &price : prices)
        {
            if (price.goodId > 0) // Skip invalid goods
            {
                options.push_back(GameData::getInstance().findTradeGoodById(price.goodId)->getName());
            }
        }
        options.push_back("Retour au menu du marché");

        int choice = m_ui.displayMenu("Quelle marchandise souhaitez-vous vendre ?", options);

        if (choice >= 0 && choice < static_cast<int>(prices.size()))
        {
            // Ask for quantity
            std::cout << "Quantité à vendre (0 pour annuler) : ";
            int quantity;
            std::cin >> quantity;
            std::cin.ignore(); // Clear newline from input buffer

            if (quantity > 0)
            {
                // Try to sell the goods
                bool success = m_tradingSystem.sellGoods(m_player, prices[choice].goodId, quantity);

                if (success)
                {
                    std::cout << "Vente réussie ! Vous avez vendu " << quantity << " "
                              << GameData::getInstance().findTradeGoodById(prices[choice].goodId)->getName() << " pour "
                              << (prices[choice].getCurrentPrice() * quantity) << " or." << std::endl;
                }
                else
                {
                    std::cout << "Vente impossible. Vérifiez que vous avez assez de cette marchandise." << std::endl;
                }

                std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
                std::cin.get();
            }
        }
        else
        {
            exitMenu = true;
        }
    }
}

void TradingUI::displayPriceComparison(int goodId)
{
    system("cls"); // Clear the screen

    // Get the trade good
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
    {
        std::cout << "Marchandise introuvable." << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    std::cout << "=== COMPARAISON DES PRIX : " << good->getName() << " ===" << std::endl;
    std::cout << "Description : " << good->getDescription() << std::endl;
    std::cout << "Catégorie : " << good->getCategory() << std::endl;
    std::cout << "Poids unitaire : " << good->getWeight() << std::endl;
    std::cout << std::endl;

    // Table header
    std::cout << std::left << std::setw(30) << "MARCHÉ"
              << std::setw(15) << "PRIX ACHAT"
              << std::setw(15) << "PRIX VENTE"
              << "DISPONIBLE" << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // Get all markets
    const auto &markets = World::getInstance().getCities();

    // Sort markets by buy price
    std::vector<std::pair<std::string, MarketPrice>> marketPrices;

    for (const auto &city : markets)
    {
        Market *market = TradingSystem::getInstance().getMarket(city.id);
        if (market)
        {
            MarketPrice price = market->getPriceInfo(goodId);
            marketPrices.push_back({city.name, price});
        }
    }

    // Sort by buy price (ascending)
    std::sort(marketPrices.begin(), marketPrices.end(),
              [](const auto &a, const auto &b)
              {
                  return a.second.buyPrice < b.second.buyPrice;
              });

    // Display sorted markets
    for (const auto &[name, price] : marketPrices)
    {
        std::cout << std::left << std::setw(30) << name
                  << std::setw(15) << price.buyPrice
                  << std::setw(15) << price.sellPrice
                  << price.available << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Conseil : Achetez dans les marchés en haut de la liste et vendez dans ceux en bas !" << std::endl;
    std::cout << std::endl;
    std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
    std::cin.get();
}

void TradingUI::displayKingdomRelations()
{
    // Table header
    std::cout << std::left << std::setw(30) << "ROYAUME"
              << std::setw(15) << "RÉPUTATION"
              << "BONUS COMMERCIAL" << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    // List of kingdoms (this should come from game data)
    std::vector<std::string> kingdoms = {
        "Espagne", "Portugal", "France", "Angleterre", "Venise",
        "Empire Ottoman", "Ming", "Japon"};

    for (const auto &kingdom : kingdoms)
    {
        float reputation = TradingSystem::getInstance().getKingdomReputation(kingdom);
        float bonus = TradingSystem::getInstance().getKingdomTradeBonus(kingdom);

        // Format bonus percentage
        std::ostringstream bonusStr;
        bonusStr << std::fixed << std::setprecision(1) << (bonus * 100.0f) << "%";

        std::cout << std::left << std::setw(30) << kingdom
                  << std::setw(15) << std::fixed << std::setprecision(1) << reputation
                  << bonusStr.str() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Une meilleure réputation vous donne des prix plus avantageux dans les ports de chaque royaume." << std::endl;
}