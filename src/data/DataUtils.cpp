#include "../../include/data/DataUtils.hpp"

namespace Orenji
{
    bool DataUtils::initialize(const std::string &basePath)
    {
        return DataManager::getInstance().initialize(basePath);
    }

    bool DataUtils::loadAllData()
    {
        return DataManager::getInstance().loadAllData();
    }

    bool DataUtils::saveAllData()
    {
        return DataManager::getInstance().saveAllData();
    }

    // ================================================
    // Méthodes pour les personnages (Character)
    // ================================================

    std::shared_ptr<Character> DataUtils::getCharacter(int id)
    {
        return DataManager::getInstance().getEntityById<Character>(id);
    }

    std::vector<std::shared_ptr<Character>> DataUtils::getAllCharacters()
    {
        return DataManager::getInstance().getAllCharacters();
    }

    std::vector<std::shared_ptr<Character>> DataUtils::getCharactersWhere(std::function<bool(const Character &)> filterFunc)
    {
        return DataManager::getInstance().getEntitiesWhere<Character>(filterFunc);
    }

    int DataUtils::addCharacter(const Character &character)
    {
        return DataManager::getInstance().addEntity<Character>(character);
    }

    bool DataUtils::updateCharacter(const Character &character)
    {
        return DataManager::getInstance().updateEntity<Character>(character);
    }

    bool DataUtils::deleteCharacter(int id)
    {
        return DataManager::getInstance().deleteEntity<Character>(id);
    }

    // ================================================
    // Méthodes pour les navires (Ship)
    // ================================================

    std::shared_ptr<Ship> DataUtils::getShip(int id)
    {
        return DataManager::getInstance().getEntityById<Ship>(id);
    }

    std::vector<std::shared_ptr<Ship>> DataUtils::getAllShips()
    {
        return DataManager::getInstance().getAllShips();
    }

    std::vector<std::shared_ptr<Ship>> DataUtils::getShipsWhere(std::function<bool(const Ship &)> filterFunc)
    {
        return DataManager::getInstance().getEntitiesWhere<Ship>(filterFunc);
    }

    int DataUtils::addShip(const Ship &ship)
    {
        return DataManager::getInstance().addEntity<Ship>(ship);
    }

    bool DataUtils::updateShip(const Ship &ship)
    {
        return DataManager::getInstance().updateEntity<Ship>(ship);
    }

    bool DataUtils::deleteShip(int id)
    {
        return DataManager::getInstance().deleteEntity<Ship>(id);
    }

    // ================================================
    // Méthodes pour les flottes (Fleet)
    // ================================================

    std::shared_ptr<Fleet> DataUtils::getFleet(int id)
    {
        return DataManager::getInstance().getEntityById<Fleet>(id);
    }

    std::vector<std::shared_ptr<Fleet>> DataUtils::getAllFleets()
    {
        return DataManager::getInstance().getAllFleets();
    }

    std::vector<std::shared_ptr<Fleet>> DataUtils::getFleetsWhere(std::function<bool(const Fleet &)> filterFunc)
    {
        return DataManager::getInstance().getEntitiesWhere<Fleet>(filterFunc);
    }

    int DataUtils::addFleet(const Fleet &fleet)
    {
        return DataManager::getInstance().addEntity<Fleet>(fleet);
    }

    bool DataUtils::updateFleet(const Fleet &fleet)
    {
        return DataManager::getInstance().updateEntity<Fleet>(fleet);
    }

    bool DataUtils::deleteFleet(int id)
    {
        return DataManager::getInstance().deleteEntity<Fleet>(id);
    }

    // ================================================
    // Méthodes pour les biens commerciaux (TradeGood)
    // ================================================

    std::shared_ptr<TradeGood> DataUtils::getTradeGood(int id)
    {
        return DataManager::getInstance().getEntityById<TradeGood>(id);
    }

    std::vector<std::shared_ptr<TradeGood>> DataUtils::getAllTradeGoods()
    {
        return DataManager::getInstance().getAllTradeGoods();
    }

    std::vector<std::shared_ptr<TradeGood>> DataUtils::getTradeGoodsWhere(std::function<bool(const TradeGood &)> filterFunc)
    {
        return DataManager::getInstance().getEntitiesWhere<TradeGood>(filterFunc);
    }

    int DataUtils::addTradeGood(const TradeGood &tradeGood)
    {
        return DataManager::getInstance().addEntity<TradeGood>(tradeGood);
    }

    bool DataUtils::updateTradeGood(const TradeGood &tradeGood)
    {
        return DataManager::getInstance().updateEntity<TradeGood>(tradeGood);
    }

    bool DataUtils::deleteTradeGood(int id)
    {
        return DataManager::getInstance().deleteEntity<TradeGood>(id);
    }

    // ================================================
    // Méthodes pour le monde (World)
    // ================================================

    World &DataUtils::getWorld()
    {
        return DataManager::getInstance().getWorld();
    }

    bool DataUtils::updateWorld(const World &world)
    {
        return DataManager::getInstance().updateEntity<World>(world);
    }

    // ================================================
    // Méthodes pour le joueur (Player)
    // ================================================

    Player &DataUtils::getPlayer()
    {
        return DataManager::getInstance().getPlayer();
    }

    bool DataUtils::updatePlayer(const Player &player)
    {
        return DataManager::getInstance().updateEntity<Player>(player);
    }

} // namespace Orenji