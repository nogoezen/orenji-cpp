#include "TradingUI.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

TradingUI::TradingUI(Player &player, ConsoleUI &ui)
    : m_player(player), m_ui(ui)
{
}

void TradingUI::showTradingMenu(int locationId)
{
    // Get the market for this location
    Market *market = TradingSystem::getInstance().getMarket(locationId);
    if (!market)
    {
        std::cout << "Aucun marché disponible à cet endroit." << std::endl;
        std::cout << "Appuyez sur une touche pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen (for Windows)

        // Display trading menu title
        std::cout << "=== MENU DE COMMERCE : " << market->getLocationName() << " ===" << std::endl;
        std::cout << "Or disponible : " << m_player.getGold() << " pièces" << std::endl;
        std::cout << "Capacité de cargo : " << m_player.getRemainingCargoCapacity() << "/" << m_player.getTotalCargoCapacity() << std::endl;
        std::cout << std::endl;

        // Menu options
        std::vector<std::string> options = {
            "Voir le marché (acheter/vendre)",
            "Voir les routes commerciales",
            "Comparer les prix",
            "Relations commerciales",
            "Retour au menu principal"};

        int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

        switch (choice)
        {
        case 0: // Market
            showMarketScreen(locationId);
            break;

        case 1: // Trade routes
            showTradeRoutesScreen(locationId);
            break;

        case 2: // Price comparison
            showPriceComparisonScreen(locationId);
            break;

        case 3: // Kingdom relations
            showKingdomRelationsScreen();
            break;

        case 4: // Exit
            exitMenu = true;
            break;

        default:
            break;
        }
    }
}

void TradingUI::showMarketScreen(int locationId)
{
    // Get the market for this location
    Market *market = TradingSystem::getInstance().getMarket(locationId);
    if (!market)
    {
        return;
    }

    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display market title
        std::cout << "=== MARCHÉ DE " << market->getLocationName() << " ===" << std::endl;

        // Display market info
        displayMarketInfo(market);

        std::cout << std::endl;

        // Menu options
        std::vector<std::string> options = {
            "Acheter des marchandises",
            "Vendre des marchandises",
            "Voir votre cargaison",
            "Retour au menu de commerce"};

        int choice = m_ui.displayMenu("Que souhaitez-vous faire ?", options);

        switch (choice)
        {
        case 0: // Buy
            handleBuyGoods(market);
            break;

        case 1: // Sell
            handleSellGoods(market);
            break;

        case 2: // View cargo
            displayPlayerCargo();
            break;

        case 3: // Exit
            exitMenu = true;
            break;

        default:
            break;
        }
    }
}

void TradingUI::showTradeRoutesScreen(int locationId)
{
    bool exitMenu = false;
    while (!exitMenu)
    {
        system("cls"); // Clear the screen

        // Display trade routes title
        std::cout << "=== ROUTES COMMERCIALES DISPONIBLES ===" << std::endl;

        // Display trade routes
        displayTradeRoutes(locationId);

        std::cout << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour retourner au menu de commerce..." << std::endl;
        std::cin.get();
        exitMenu = true;
    }
}

void TradingUI::showPriceComparisonScreen(int locationId)
{
    // Get current market
    Market *market = TradingSystem::getInstance().getMarket(locationId);
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

void TradingUI::displayMarketInfo(Market *market)
{
    std::cout << "Prospérité : " << market->getProsperity() << "/10" << std::endl;

    // In a real implementation, display more information about the market
    std::cout << "Ce marché propose une variété de marchandises locales et importées." << std::endl;
    std::cout << "Les spécialités locales sont souvent moins chères, " << std::endl;
    std::cout << "tandis que les biens en demande se vendent plus cher." << std::endl;
}

void TradingUI::displayTradeGoods(Market *market, bool buying)
{
    // Get all prices
    auto prices = market->getAllPrices();

    // Table header
    std::cout << std::left << std::setw(30) << "MARCHANDISE"
              << std::setw(15) << "CATÉGORIE"
              << std::setw(15) << (buying ? "PRIX ACHAT" : "PRIX VENTE")
              << std::setw(15) << (buying ? "DISPONIBLE" : "QUANTITÉ")
              << "DEMANDE" << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    for (const auto &price : prices)
    {
        // Skip invalid goods
        if (price.goodId <= 0)
            continue;

        // Display price info
        std::cout << std::left << std::setw(30) << price.goodName
                  << std::setw(15) << price.category
                  << std::setw(15) << (buying ? price.buyPrice : price.sellPrice)
                  << std::setw(15);

        // If buying, show availability
        if (buying)
        {
            std::cout << price.available;
        }
        else
        {
            // If selling, show player's quantity
            std::cout << m_player.getCargoQuantity(price.goodId);
        }

        // Display demand indicator
        std::string demand;
        if (price.demand == 2)
            demand = "Élevée";
        else if (price.demand == 1)
            demand = "Moyenne";
        else
            demand = "Faible";

        std::cout << demand << std::endl;
    }
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

void TradingUI::handleBuyGoods(Market *market)
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
        displayTradeGoods(market, true);

        std::cout << std::endl;

        // Get all prices
        auto prices = market->getAllPrices();

        // Create options from goods
        std::vector<std::string> options;
        for (const auto &price : prices)
        {
            if (price.goodId > 0) // Skip invalid goods
            {
                options.push_back(price.goodName);
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
                bool success = market->buyGoods(m_player, prices[choice].goodId, quantity);

                if (success)
                {
                    std::cout << "Achat réussi ! Vous avez acheté " << quantity << " "
                              << prices[choice].goodName << " pour "
                              << (prices[choice].buyPrice * quantity) << " or." << std::endl;
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

void TradingUI::handleSellGoods(Market *market)
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
        displayTradeGoods(market, false);

        std::cout << std::endl;

        // Get all prices
        auto prices = market->getAllPrices();

        // Create options from goods
        std::vector<std::string> options;
        for (const auto &price : prices)
        {
            if (price.goodId > 0) // Skip invalid goods
            {
                options.push_back(price.goodName);
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
                bool success = market->sellGoods(m_player, prices[choice].goodId, quantity);

                if (success)
                {
                    std::cout << "Vente réussie ! Vous avez vendu " << quantity << " "
                              << prices[choice].goodName << " pour "
                              << (prices[choice].sellPrice * quantity) << " or." << std::endl;
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
            marketPrices.push_back({market->getLocationName(), price});
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