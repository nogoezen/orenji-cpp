/*
 * Implémentations des nouvelles méthodes pour le système de commerce amélioré
 * À intégrer dans les fichiers appropriés
 */

// ---- TradingUI.cpp ----

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

void TradingUI::displayPriceHistory(int cityId, int goodId)
{
    const PriceHistory *history = m_tradingSystem.getPriceHistory(cityId, goodId);
    if (!history || history->prices.empty())
    {
        std::cout << "Aucun historique de prix disponible pour cette marchandise." << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
    {
        std::cout << "Marchandise introuvable." << std::endl;
        std::cout << "Appuyez sur ENTRÉE pour continuer..." << std::endl;
        std::cin.get();
        return;
    }

    system("cls");
    std::cout << "=== HISTORIQUE DES PRIX : " << good->getName() << " ===" << std::endl;

    // Trouver les valeurs min et max pour l'échelle
    int minPrice = *std::min_element(history->prices.begin(), history->prices.end());
    int maxPrice = *std::max_element(history->prices.begin(), history->prices.end());

    // Pour éviter la division par zéro
    if (maxPrice == minPrice)
    {
        maxPrice = minPrice + 1;
    }

    const int graphHeight = 10;
    const int graphWidth = std::min(static_cast<int>(history->prices.size()), 20);

    // Afficher l'axe Y et les valeurs
    for (int y = 0; y < graphHeight; ++y)
    {
        int price = maxPrice - y * (maxPrice - minPrice) / (graphHeight - 1);
        std::cout << std::setw(5) << price << " |";

        for (int x = 0; x < graphWidth; ++x)
        {
            int index = history->prices.size() - graphWidth + x;
            if (index >= 0 && index < history->prices.size())
            {
                int p = history->prices[index];
                int scaledY = (p - minPrice) * (graphHeight - 1) / (maxPrice - minPrice);

                if (graphHeight - 1 - y == scaledY)
                {
                    std::cout << "*";
                }
                else if (graphHeight - 1 - y < scaledY)
                {
                    std::cout << "|";
                }
                else
                {
                    std::cout << " ";
                }
            }
        }
        std::cout << std::endl;
    }

    // Afficher l'axe X
    std::cout << "      ";
    for (int x = 0; x < graphWidth; ++x)
    {
        std::cout << "-";
    }
    std::cout << std::endl;

    // Afficher la tendance
    float trend = history->getTrend();
    std::cout << "Tendance : ";
    if (trend > 0.1f)
    {
        std::cout << "À la hausse (+" << std::fixed << std::setprecision(1) << trend << ")";
    }
    else if (trend < -0.1f)
    {
        std::cout << "À la baisse (" << std::fixed << std::setprecision(1) << trend << ")";
    }
    else
    {
        std::cout << "Stable (" << std::fixed << std::setprecision(1) << trend << ")";
    }
    std::cout << std::endl;

    std::cout << "\nAppuyez sur ENTRÉE pour continuer..." << std::endl;
    std::cin.get();
}

void TradingUI::displayProfitableRoutes(int cityId)
{
    system("cls");
    std::cout << "=== ROUTES COMMERCIALES RENTABLES ===" << std::endl;

    const World world;
    auto routes = m_tradingSystem.findProfitableRoutes(cityId, 5);

    if (routes.empty())
    {
        std::cout << "Aucune route commerciale rentable trouvée depuis cette ville." << std::endl;
    }
    else
    {
        std::cout << std::left << std::setw(5) << "No."
                  << std::setw(20) << "Destination"
                  << std::setw(15) << "Profit estimé" << std::endl;
        std::cout << std::string(40, '-') << std::endl;

        for (size_t i = 0; i < routes.size(); ++i)
        {
            const auto &[destCityId, profitMargin] = routes[i];
            std::string cityName = world.getCityName(destCityId);

            std::cout << std::left << std::setw(5) << (i + 1)
                      << std::setw(20) << cityName
                      << std::setw(15) << (static_cast<int>(profitMargin * 100)) << "%" << std::endl;
        }
    }

    std::cout << "\nAppuyez sur ENTRÉE pour continuer..." << std::endl;
    std::cin.get();
}

void TradingUI::displayBlackMarket(int cityId)
{
    // Vérifier si le joueur a accès au marché noir
    if (!m_tradingSystem.hasBlackMarket(cityId, m_player))
    {
        std::cout << "Vous n'avez pas accès au marché noir dans cette ville." << std::endl;
        return;
    }

    std::cout << "Bienvenue au marché noir de " << World().getCityName(cityId) << "." << std::endl;
    std::cout << "Ici, vous pouvez trouver des marchandises rares et des opportunités spéciales," << std::endl;
    std::cout << "mais attention aux autorités !" << std::endl;
    std::cout << std::endl;

    // Proposer des marchandises spéciales ou à des prix avantageux
    // Pour cet exemple, on utilise simplement des prix réduits sur les marchandises existantes

    std::cout << "== MARCHANDISES SPÉCIALES ==" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Nom"
              << std::setw(10) << "Prix" << std::setw(10) << "Stock" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    // Récupérer les prix dans cette ville
    std::vector<GoodPrice> prices = m_tradingSystem.getCityPrices(cityId);

    // Appliquer une réduction de 25% pour le marché noir
    for (size_t i = 0; i < prices.size(); ++i)
    {
        const auto &price = prices[i];
        const TradeGood *good = GameData::getInstance().findTradeGoodById(price.goodId);

        if (!good)
            continue;

        std::string name = good->getName();
        int currentPrice = static_cast<int>(price.getCurrentPrice() * 0.75f);

        std::cout << std::left << std::setw(5) << i
                  << std::setw(20) << name
                  << std::setw(10) << currentPrice
                  << std::setw(10) << (price.currentStock / 2) << std::endl;
    }

    // Implémentation réelle : permettre d'acheter/vendre ces marchandises
}

// --- Implémentation de TradeRoute ---

int TradeRoute::estimateProfit(TradingSystem &system) const
{
    int totalBuyCost = 0;
    int totalSellValue = 0;

    // Calculer le coût total à l'achat dans la ville source
    for (const auto &[goodId, quantity] : tradedGoods)
    {
        int buyPrice = system.getGoodPrice(sourceCity, goodId);
        if (buyPrice > 0)
        {
            totalBuyCost += buyPrice * quantity;
        }
    }

    // Calculer la valeur totale à la vente dans la ville de destination
    for (const auto &[goodId, quantity] : tradedGoods)
    {
        int sellPrice = system.getGoodPrice(destinationCity, goodId);
        if (sellPrice > 0)
        {
            // On vend généralement à 90% du prix affiché
            totalSellValue += static_cast<int>(sellPrice * 0.9f) * quantity;
        }
    }

    // Profit estimé
    return totalSellValue - totalBuyCost;
}

bool TradeRoute::calculateRisks() const
{
    // La sécurité va de 0 à 10, où 10 est très sécurisé
    // Retourne true s'il y a un risque, false sinon

    // Chance de base de 50% qui diminue avec le niveau de sécurité
    float riskChance = 0.5f - (securityLevel * 0.05f);

    // Génération d'un nombre aléatoire pour déterminer le risque
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    return dist(gen) < riskChance;
}