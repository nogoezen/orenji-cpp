#include "FleetManager.h"
#include <sstream>
#include <iomanip>

void FleetManager::showFleetManagementMenu()
{
    bool exitMenu = false;

    while (!exitMenu)
    {
        // Afficher le résumé de la flotte
        displayFleetSummary();

        // Créer le menu
        std::vector<ConsoleUI::MenuItem> menuItems = {
            {"Voir les détails de la flotte", [this]()
             { viewFleetDetails(); }},
            {"Gérer les navires", [this]()
             { manageShips(); }},
            {"Transférer la cargaison", [this]()
             { transferCargo(); }},
            {"Assigner l'équipage", [this]()
             { assignCrew(); }},
            {"Sélectionner le navire amiral", [this]()
             { selectFlagship(); }},
            {"Renommer la flotte", [this]()
             { renameFleet(); }},
            {"Gérer les formations", [this]()
             { manageFleetFormations(); }},
            {"Gérer les provisions", [this]()
             { manageFleetSupplies(); }},
            {"Quitter le menu de gestion", [&exitMenu]()
             { exitMenu = true; }}};

        m_ui.displayMenu("GESTION DE LA FLOTTE", menuItems);
    }
}

void FleetManager::displayFleetSummary()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        return;
    }

    // Afficher le nom de la flotte et le nombre de navires
    m_ui.displayMessage("Flotte: " + fleet->getName());
    m_ui.displayMessage("Nombre de navires: " + std::to_string(fleet->getSize()) + "/" + std::to_string(MAX_FLEET_SIZE));

    // Afficher le résumé des cargaisons
    m_ui.displayMessage("Capacité de cargaison: " + std::to_string(fleet->getUsedCargo()) + "/" + std::to_string(fleet->getTotalCargo()) + " unités");

    // Afficher le résumé de l'équipage
    m_ui.displayMessage("Équipage: " + std::to_string(fleet->getCurrentCrew()) + "/" + std::to_string(fleet->getTotalCrew()) + " marins");

    // Afficher le navire amiral
    auto flagship = fleet->getFlagship();
    if (flagship)
    {
        m_ui.displayMessage("Navire amiral: " + flagship->getName());
    }

    m_ui.displayMessage("");
}

void FleetManager::viewFleetDetails()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("DÉTAILS DE LA FLOTTE");

    // Afficher le résumé de la flotte
    m_ui.displayMessage("Flotte: " + fleet->getName());
    m_ui.displayMessage("Position: (" + std::to_string(fleet->getPosX()) + ", " + std::to_string(fleet->getPosY()) + ")");
    m_ui.displayMessage("Vitesse: " + std::to_string(fleet->getSpeed()) + " nœuds");
    m_ui.displayMessage("Capacité de cargaison: " + std::to_string(fleet->getUsedCargo()) + "/" + std::to_string(fleet->getTotalCargo()) + " unités");
    m_ui.displayMessage("Équipage: " + std::to_string(fleet->getCurrentCrew()) + "/" + std::to_string(fleet->getTotalCrew()) + " marins");
    m_ui.displayMessage("");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark
           << " - " << ship->getShipClass()
           << " (" << ship->getCurrentCrew() << "/" << ship->getMaxCrew() << " marins"
           << ", " << ship->getUsedCargo() << "/" << ship->getCargoCapacity() << " cargaison"
           << ", Vitesse: " << ship->getSpeed() << " nœuds"
           << ", État: " << std::fixed << std::setprecision(1) << ship->getDurabilityPercent() << "%)";

        m_ui.displayMessage(ss.str());
    }

    m_ui.waitForKeyPress();
}

void FleetManager::manageShips()
{
    bool exitMenu = false;

    while (!exitMenu)
    {
        m_ui.displayTitle("GESTION DES NAVIRES");

        // Afficher la liste des navires
        auto fleet = m_player->getFleet();
        if (fleet && fleet->getSize() > 0)
        {
            m_ui.displayMessage("Liste des navires:");
            for (size_t i = 0; i < fleet->getSize(); ++i)
            {
                auto ship = fleet->getShip(i);
                std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";
                std::stringstream ss;
                ss << (i + 1) << ". " << ship->getName() << flagshipMark
                   << " - " << ship->getShipClass();

                m_ui.displayMessage(ss.str());
            }
            m_ui.displayMessage("");
        }
        else
        {
            m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
            m_ui.displayMessage("");
        }

        // Créer le menu
        std::vector<ConsoleUI::MenuItem> menuItems = {
            {"Ajouter un navire à la flotte", [this]()
             { addShipToFleet(); }},
            {"Retirer un navire de la flotte", [this]()
             { removeShipFromFleet(); }},
            {"Acheter un nouveau navire", [this]()
             { buyNewShip(); }},
            {"Vendre un navire", [this]()
             { sellShip(); }},
            {"Réparer un navire", [this]()
             { repairShip(); }},
            {"Améliorer un navire", [this]()
             { upgradeShip(); }},
            {"Renommer un navire", [this]()
             { renameShip(); }},
            {"Retour au menu précédent", [&exitMenu]()
             { exitMenu = true; }}};

        m_ui.displayMenu("GESTION DES NAVIRES", menuItems);
    }
}

void FleetManager::transferCargo()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() < 2)
    {
        m_ui.displayMessage("Vous avez besoin d'au moins deux navires pour transférer de la cargaison.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("TRANSFERT DE CARGAISON");

    // Liste des navires sources possibles
    m_ui.displayMessage("Sélectionnez le navire SOURCE:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName()
           << " (" << ship->getUsedCargo() << "/" << ship->getCargoCapacity() << " cargaison)";

        m_ui.displayMessage(ss.str());
    }

    int sourceIndex = m_ui.getIntInput("Navire source (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;
    auto sourceShip = fleet->getShip(sourceIndex);

    // Vérifier si le navire source a de la cargaison
    if (sourceShip->getUsedCargo() == 0)
    {
        m_ui.displayMessage("Ce navire n'a pas de cargaison à transférer.");
        m_ui.waitForKeyPress();
        return;
    }

    // Liste des cargaisons du navire source
    const auto &cargo = sourceShip->getCargo();
    if (cargo.empty())
    {
        m_ui.displayMessage("Ce navire n'a pas de cargaison à transférer.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("\nCargaison disponible:");
    for (size_t i = 0; i < cargo.size(); ++i)
    {
        std::stringstream ss;
        ss << (i + 1) << ". " << cargo[i].name
           << " x" << cargo[i].quantity
           << " (Poids unitaire: " << cargo[i].unitWeight << ")";

        m_ui.displayMessage(ss.str());
    }

    int cargoIndex = m_ui.getIntInput("Choisissez la cargaison à transférer (1-" + std::to_string(cargo.size()) + "): ", 1, cargo.size()) - 1;

    // Sélectionner le navire de destination
    m_ui.displayMessage("\nSélectionnez le navire DESTINATION:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        if (i == sourceIndex)
            continue; // Skip source ship

        auto ship = fleet->getShip(i);
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName()
           << " (" << ship->getUsedCargo() << "/" << ship->getCargoCapacity() << " cargaison)";

        m_ui.displayMessage(ss.str());
    }

    int destIndex;
    do
    {
        destIndex = m_ui.getIntInput("Navire destination (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;
    } while (destIndex == sourceIndex);

    auto destShip = fleet->getShip(destIndex);

    // Déterminer la quantité à transférer
    const auto &selectedCargo = cargo[cargoIndex];
    int maxTransfer = std::min(selectedCargo.quantity,
                               destShip->getRemainingCargo() / selectedCargo.unitWeight);

    if (maxTransfer <= 0)
    {
        m_ui.displayMessage("Le navire de destination n'a pas assez d'espace pour cette cargaison.");
        m_ui.waitForKeyPress();
        return;
    }

    int quantity = m_ui.getIntInput("Quantité à transférer (1-" + std::to_string(maxTransfer) + "): ", 1, maxTransfer);

    // Effectuer le transfert
    bool success = destShip->addCargo(
        selectedCargo.itemId,
        selectedCargo.name,
        quantity,
        selectedCargo.unitWeight);

    if (success)
    {
        sourceShip->removeCargo(selectedCargo.itemId, quantity);
        m_ui.displayMessage("Transfert réussi de " + std::to_string(quantity) + " " + selectedCargo.name + ".");
    }
    else
    {
        m_ui.displayMessage("Le transfert a échoué.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::assignCrew()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("AFFECTATION DE L'ÉQUIPAGE");

    // Montrer l'état actuel de l'équipage
    m_ui.displayMessage("Équipage total: " + std::to_string(fleet->getCurrentCrew()) + "/" + std::to_string(fleet->getTotalCrew()) + " marins");
    m_ui.displayMessage("");

    m_ui.displayMessage("État actuel de l'équipage par navire:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName()
           << " - " << ship->getCurrentCrew() << "/" << ship->getMaxCrew() << " marins"
           << " (minimum requis: " << ship->getRequiredCrew() << ")";

        m_ui.displayMessage(ss.str());
    }

    m_ui.displayMessage("\nQuel navire souhaitez-vous modifier?");
    int shipIndex = m_ui.getIntInput("Navire (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;

    auto ship = fleet->getShip(shipIndex);
    m_ui.displayMessage("\nNavire: " + ship->getName());
    m_ui.displayMessage("Équipage actuel: " + std::to_string(ship->getCurrentCrew()) + "/" + std::to_string(ship->getMaxCrew()));
    m_ui.displayMessage("Équipage minimum requis: " + std::to_string(ship->getRequiredCrew()));

    // Options
    m_ui.displayMessage("\nOptions:");
    m_ui.displayMessage("1. Ajouter de l'équipage");
    m_ui.displayMessage("2. Retirer de l'équipage");
    m_ui.displayMessage("3. Retour");

    int choice = m_ui.getIntInput("Votre choix: ", 1, 3);

    switch (choice)
    {
    case 1: // Ajouter
    {
        int maxToAdd = ship->getMaxCrew() - ship->getCurrentCrew();
        if (maxToAdd <= 0)
        {
            m_ui.displayMessage("Ce navire a déjà son équipage complet.");
            m_ui.waitForKeyPress();
            return;
        }

        int toAdd = m_ui.getIntInput("Combien de marins ajouter? (1-" + std::to_string(maxToAdd) + "): ", 1, maxToAdd);
        if (ship->addCrew(toAdd))
        {
            m_ui.displayMessage("Équipage ajouté avec succès.");
        }
        else
        {
            m_ui.displayMessage("Impossible d'ajouter l'équipage.");
        }
        break;
    }
    case 2: // Retirer
    {
        int maxToRemove = ship->getCurrentCrew() - ship->getRequiredCrew();
        if (maxToRemove <= 0)
        {
            m_ui.displayMessage("Ce navire a besoin de son équipage actuel pour fonctionner.");
            m_ui.waitForKeyPress();
            return;
        }

        int toRemove = m_ui.getIntInput("Combien de marins retirer? (1-" + std::to_string(maxToRemove) + "): ", 1, maxToRemove);
        if (ship->removeCrew(toRemove))
        {
            m_ui.displayMessage("Équipage retiré avec succès.");
        }
        else
        {
            m_ui.displayMessage("Impossible de retirer l'équipage.");
        }
        break;
    }
    case 3: // Retour
        return;
    }

    m_ui.waitForKeyPress();
}

void FleetManager::selectFlagship()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() < 2)
    {
        m_ui.displayMessage("Vous avez besoin d'au moins deux navires pour changer de navire amiral.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("SÉLECTION DU NAVIRE AMIRAL");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL ACTUEL]" : "";
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark
           << " - " << ship->getShipClass()
           << " (Vitesse: " << ship->getSpeed() << " nœuds"
           << ", Manoeuvrabilité: " << ship->getManeuverability() << ")";

        m_ui.displayMessage(ss.str());
    }

    int index = m_ui.getIntInput("Choisissez le nouveau navire amiral (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;

    if (fleet->setFlagship(index))
    {
        auto newFlagship = fleet->getShip(index);
        m_ui.displayMessage(newFlagship->getName() + " est maintenant votre navire amiral.");
    }
    else
    {
        m_ui.displayMessage("Échec du changement de navire amiral.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::renameFleet()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
    {
        m_ui.displayMessage("Vous n'avez pas de flotte à renommer.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("RENOMMER LA FLOTTE");

    m_ui.displayMessage("Nom actuel: " + fleet->getName());
    std::string newName = m_ui.getInput("Nouveau nom: ");

    if (!newName.empty())
    {
        fleet->setName(newName);
        m_ui.displayMessage("La flotte a été renommée en " + newName + ".");
    }
    else
    {
        m_ui.displayMessage("Opération annulée.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::renameShip()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire à renommer.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("RENOMMER UN NAVIRE");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark;

        m_ui.displayMessage(ss.str());
    }

    int index = m_ui.getIntInput("Choisissez le navire à renommer (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;
    auto ship = fleet->getShip(index);

    m_ui.displayMessage("Nom actuel: " + ship->getName());
    std::string newName = m_ui.getInput("Nouveau nom: ");

    if (!newName.empty())
    {
        ship->setName(newName);
        m_ui.displayMessage("Le navire a été renommé en " + newName + ".");
    }
    else
    {
        m_ui.displayMessage("Opération annulée.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::addShipToFleet()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
    {
        m_ui.displayError("Vous n'avez pas de flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    if (fleet->isFull())
    {
        m_ui.displayMessage("Votre flotte est déjà complète (maximum " + std::to_string(MAX_FLEET_SIZE) + " navires).");
        m_ui.waitForKeyPress();
        return;
    }

    // Obtenir la liste des navires disponibles dans le jeu
    const auto &ships = GameData::getInstance().getShips();
    if (ships.empty())
    {
        m_ui.displayError("Aucun navire disponible.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("AJOUTER UN NAVIRE À LA FLOTTE");

    // Afficher les navires disponibles
    m_ui.displayMessage("Navires disponibles:");
    for (size_t i = 0; i < ships.size(); ++i)
    {
        std::stringstream ss;
        ss << (i + 1) << ". " << ships[i].getName()
           << " - " << ships[i].getShipClass()
           << " (Capacité: " << ships[i].getCargoCapacity() << ", Équipage min: " << ships[i].getRequiredCrew() << ")";

        m_ui.displayMessage(ss.str());
    }

    int shipIndex = m_ui.getIntInput("Choisissez un navire (1-" + std::to_string(ships.size()) + "): ", 1, ships.size()) - 1;

    // Créer une copie du navire
    std::shared_ptr<Ship> newShip = std::make_shared<Ship>(ships[shipIndex]);

    // Vérifier si le joueur a assez d'or
    int cost = newShip->getDurability() * 100; // Prix basé sur la durabilité

    m_ui.displayMessage("\nCe navire coûte " + std::to_string(cost) + " pièces d'or.");

    if (m_player->getGold() < cost)
    {
        m_ui.displayError("Vous n'avez pas assez d'or pour acheter ce navire.");
        m_ui.waitForKeyPress();
        return;
    }

    // Confirmer l'achat
    m_ui.displayMessage("Confirmez-vous l'achat? (1: Oui, 2: Non)");
    int confirm = m_ui.getIntInput("Votre choix: ", 1, 2);

    if (confirm == 2)
    {
        m_ui.displayMessage("Achat annulé.");
        m_ui.waitForKeyPress();
        return;
    }

    // Faire l'achat
    m_player->removeGold(cost);

    // Ajouter le navire à la flotte
    if (m_player->addShipToFleet(newShip))
    {
        m_ui.displayMessage("Achat réussi! " + newShip->getName() + " a été ajouté à votre flotte.");
    }
    else
    {
        m_ui.displayError("Impossible d'ajouter le navire à votre flotte.");
        m_player->addGold(cost); // Rembourser le joueur
    }

    m_ui.waitForKeyPress();
}

void FleetManager::removeShipFromFleet()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    if (fleet->getSize() == 1)
    {
        m_ui.displayMessage("Vous ne pouvez pas retirer votre dernier navire de la flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("RETIRER UN NAVIRE DE LA FLOTTE");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark;

        m_ui.displayMessage(ss.str());
    }

    int index = m_ui.getIntInput("Choisissez le navire à retirer (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;

    // Vérifier si c'est le navire amiral
    if (index == 0)
    {
        m_ui.displayMessage("Vous ne pouvez pas retirer le navire amiral. Sélectionnez d'abord un autre navire comme amiral.");
        m_ui.waitForKeyPress();
        return;
    }

    // Confirmer le retrait
    m_ui.displayMessage("Confirmez-vous le retrait de ce navire? (1: Oui, 2: Non)");
    int confirm = m_ui.getIntInput("Votre choix: ", 1, 2);

    if (confirm == 2)
    {
        m_ui.displayMessage("Retrait annulé.");
        m_ui.waitForKeyPress();
        return;
    }

    // Retirer le navire
    auto shipToRemove = fleet->getShip(index);
    std::string shipName = shipToRemove->getName();

    if (m_player->removeShipFromFleet(index))
    {
        m_ui.displayMessage("Le navire " + shipName + " a été retiré de votre flotte.");
    }
    else
    {
        m_ui.displayError("Impossible de retirer le navire de votre flotte.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::buyNewShip()
{
    // Pour l'instant, redirigeons vers addShipToFleet qui gère déjà l'achat
    addShipToFleet();
}

void FleetManager::sellShip()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire à vendre.");
        m_ui.waitForKeyPress();
        return;
    }

    if (fleet->getSize() == 1)
    {
        m_ui.displayMessage("Vous ne pouvez pas vendre votre dernier navire.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("VENDRE UN NAVIRE");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";
        int sellPrice = ship->getDurability() * 70; // Prix de vente (70% du prix d'achat)

        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark
           << " - Valeur: " << sellPrice << " pièces d'or"
           << " - État: " << std::fixed << std::setprecision(1) << ship->getDurabilityPercent() << "%";

        m_ui.displayMessage(ss.str());
    }

    int index = m_ui.getIntInput("Choisissez le navire à vendre (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;

    // Vérifier si c'est le navire amiral
    if (index == 0)
    {
        m_ui.displayMessage("Vous ne pouvez pas vendre le navire amiral. Sélectionnez d'abord un autre navire comme amiral.");
        m_ui.waitForKeyPress();
        return;
    }

    auto shipToSell = fleet->getShip(index);
    std::string shipName = shipToSell->getName();
    int sellPrice = shipToSell->getDurability() * 70; // Prix de vente (70% du prix d'achat)

    // Confirmer la vente
    m_ui.displayMessage("Vous vendrez " + shipName + " pour " + std::to_string(sellPrice) + " pièces d'or.");
    m_ui.displayMessage("Confirmez-vous la vente? (1: Oui, 2: Non)");
    int confirm = m_ui.getIntInput("Votre choix: ", 1, 2);

    if (confirm == 2)
    {
        m_ui.displayMessage("Vente annulée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer la vente
    if (m_player->removeShipFromFleet(index))
    {
        m_player->addGold(sellPrice);
        m_ui.displayMessage("Vente réussie! Vous avez reçu " + std::to_string(sellPrice) + " pièces d'or.");
    }
    else
    {
        m_ui.displayError("Impossible de vendre le navire.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::repairShip()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire à réparer.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("RÉPARER UN NAVIRE");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";

        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark
           << " - État: " << ship->getCurrentDurability() << "/" << ship->getDurability()
           << " (" << std::fixed << std::setprecision(1) << ship->getDurabilityPercent() << "%)";

        m_ui.displayMessage(ss.str());
    }

    int index = m_ui.getIntInput("Choisissez le navire à réparer (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;
    auto ship = fleet->getShip(index);

    // Vérifier si le navire est déjà en parfait état
    if (ship->getCurrentDurability() >= ship->getDurability())
    {
        m_ui.displayMessage("Ce navire est déjà en parfait état.");
        m_ui.waitForKeyPress();
        return;
    }

    // Calculer le coût de réparation
    int damagePoints = ship->getDurability() - ship->getCurrentDurability();
    int repairCost = damagePoints * 5; // 5 pièces d'or par point de dégât

    m_ui.displayMessage("Coût de réparation: " + std::to_string(repairCost) + " pièces d'or.");

    if (m_player->getGold() < repairCost)
    {
        m_ui.displayError("Vous n'avez pas assez d'or pour cette réparation.");
        m_ui.waitForKeyPress();
        return;
    }

    // Confirmer la réparation
    m_ui.displayMessage("Confirmez-vous la réparation? (1: Oui, 2: Non)");
    int confirm = m_ui.getIntInput("Votre choix: ", 1, 2);

    if (confirm == 2)
    {
        m_ui.displayMessage("Réparation annulée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer la réparation
    m_player->removeGold(repairCost);
    ship->repair(damagePoints);

    m_ui.displayMessage("Réparation terminée! Le navire est maintenant à " +
                        std::to_string(ship->getCurrentDurability()) + "/" +
                        std::to_string(ship->getDurability()) + " points de durabilité.");

    m_ui.waitForKeyPress();
}

void FleetManager::upgradeShip()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire à améliorer.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("AMÉLIORER UN NAVIRE");

    // Afficher la liste des navires
    m_ui.displayMessage("Liste des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";

        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName() << flagshipMark
           << " - " << ship->getShipClass();

        m_ui.displayMessage(ss.str());
    }

    int index = m_ui.getIntInput("Choisissez le navire à améliorer (1-" + std::to_string(fleet->getSize()) + "): ", 1, fleet->getSize()) - 1;
    auto ship = fleet->getShip(index);

    // Afficher les améliorations actuelles
    const auto &upgrades = ship->getUpgrades();
    m_ui.displayMessage("\nAméliorations actuelles:");
    m_ui.displayMessage("1. Coque: Niveau " + std::to_string(upgrades.hull) + "/10");
    m_ui.displayMessage("2. Gréement: Niveau " + std::to_string(upgrades.rigging) + "/10");
    m_ui.displayMessage("3. Canons: Niveau " + std::to_string(upgrades.cannons) + "/10");

    // Choisir le type d'amélioration
    int upgradeType = m_ui.getIntInput("Quelle amélioration souhaitez-vous effectuer? (1-3): ", 1, 3);

    // Vérifier si l'amélioration est déjà au maximum
    int currentLevel = 0;
    std::string upgradeName;

    switch (upgradeType)
    {
    case 1:
        currentLevel = upgrades.hull;
        upgradeName = "Coque";
        break;
    case 2:
        currentLevel = upgrades.rigging;
        upgradeName = "Gréement";
        break;
    case 3:
        currentLevel = upgrades.cannons;
        upgradeName = "Canons";
        break;
    }

    if (currentLevel >= 10)
    {
        m_ui.displayMessage("Cette amélioration est déjà au niveau maximum (10).");
        m_ui.waitForKeyPress();
        return;
    }

    // Calculer le coût de l'amélioration
    int upgradeCost = (currentLevel + 1) * 500; // Plus cher à chaque niveau

    m_ui.displayMessage("Coût de l'amélioration: " + std::to_string(upgradeCost) + " pièces d'or.");

    if (m_player->getGold() < upgradeCost)
    {
        m_ui.displayError("Vous n'avez pas assez d'or pour cette amélioration.");
        m_ui.waitForKeyPress();
        return;
    }

    // Confirmer l'amélioration
    m_ui.displayMessage("Confirmez-vous l'amélioration? (1: Oui, 2: Non)");
    int confirm = m_ui.getIntInput("Votre choix: ", 1, 2);

    if (confirm == 2)
    {
        m_ui.displayMessage("Amélioration annulée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer l'amélioration
    bool success = false;

    switch (upgradeType)
    {
    case 1:
        success = ship->upgradeHull(1);
        break;
    case 2:
        success = ship->upgradeRigging(1);
        break;
    case 3:
        success = ship->upgradeCannons(1);
        break;
    }

    if (success)
    {
        m_player->removeGold(upgradeCost);
        m_ui.displayMessage("Amélioration réussie! " + upgradeName + " est maintenant au niveau " + std::to_string(currentLevel + 1) + ".");
    }
    else
    {
        m_ui.displayError("L'amélioration a échoué.");
    }

    m_ui.waitForKeyPress();
}

void FleetManager::manageFleetFormations()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() < 2)
    {
        m_ui.displayMessage("Vous avez besoin d'au moins deux navires pour utiliser les formations.");
        m_ui.waitForKeyPress();
        return;
    }

    bool exitMenu = false;
    while (!exitMenu)
    {
        m_ui.displayTitle("GESTION DES FORMATIONS");

        // Afficher la formation actuelle
        FleetFormation currentFormation = fleet->getFormation();
        m_ui.displayMessage("Formation actuelle: " + Fleet::getFormationDescription(currentFormation));
        m_ui.displayMessage("");

        // Créer le menu
        std::vector<ConsoleUI::MenuItem> menuItems = {
            {"Voir les détails de la formation", [this]()
             { showFormationDetails(); }},
            {"Changer de formation", [this]()
             { selectFormation(); }},
            {"Personnaliser la formation", [this]()
             { customizeFormation(); }},
            {"Entraîner la flotte", [this]()
             { trainFleetInFormation(); }},
            {"Retour au menu précédent", [&exitMenu]()
             { exitMenu = true; }}};

        m_ui.displayMenu("GESTION DES FORMATIONS", menuItems);
    }
}

void FleetManager::showFormationDetails()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("DÉTAILS DE LA FORMATION");

    FleetFormation currentFormation = fleet->getFormation();
    m_ui.displayMessage("Formation actuelle: " + Fleet::getFormationDescription(currentFormation));
    m_ui.displayMessage("");

    // Afficher les bonus de la formation
    m_ui.displayMessage("Bonus de la formation:");
    std::vector<std::pair<std::string, std::string>> bonusTypes = {
        {"attack", "Attaque"},
        {"defense", "Défense"},
        {"speed", "Vitesse"},
        {"range", "Portée"},
        {"evasion", "Évasion"}};

    for (const auto &[type, label] : bonusTypes)
    {
        float bonus = fleet->getFormationBonus(type);
        if (bonus != 0.0f)
        {
            std::stringstream ss;
            ss << label << ": " << (bonus > 0 ? "+" : "") << std::fixed << std::setprecision(0) << (bonus * 100) << "%";
            m_ui.displayMessage(ss.str());
        }
    }

    m_ui.displayMessage("");

    // Afficher la position des navires
    m_ui.displayMessage("Position des navires:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::string flagshipMark = (i == 0) ? " [AMIRAL]" : "";

        if (currentFormation == FleetFormation::CUSTOM)
        {
            auto [x, y] = fleet->getCustomPosition(i);
            m_ui.displayMessage(ship->getName() + flagshipMark + " - Position relative: (" +
                                std::to_string(x) + ", " + std::to_string(y) + ")");
        }
        else
        {
            m_ui.displayMessage(ship->getName() + flagshipMark + " - Position standard");
        }
    }

    m_ui.waitForKeyPress();
}

void FleetManager::selectFormation()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("SÉLECTION DE LA FORMATION");

    // Afficher toutes les formations disponibles
    m_ui.displayMessage("Formations disponibles:");
    m_ui.displayMessage("1. " + Fleet::getFormationDescription(FleetFormation::LINE));
    m_ui.displayMessage("2. " + Fleet::getFormationDescription(FleetFormation::WEDGE));
    m_ui.displayMessage("3. " + Fleet::getFormationDescription(FleetFormation::CIRCLE));
    m_ui.displayMessage("4. " + Fleet::getFormationDescription(FleetFormation::SCATTERED));
    m_ui.displayMessage("5. " + Fleet::getFormationDescription(FleetFormation::COLUMN));
    m_ui.displayMessage("6. " + Fleet::getFormationDescription(FleetFormation::CUSTOM));

    int choice = m_ui.getIntInput("Choisissez une formation (1-6): ", 1, 6);
    FleetFormation newFormation;

    switch (choice)
    {
    case 1:
        newFormation = FleetFormation::LINE;
        break;
    case 2:
        newFormation = FleetFormation::WEDGE;
        break;
    case 3:
        newFormation = FleetFormation::CIRCLE;
        break;
    case 4:
        newFormation = FleetFormation::SCATTERED;
        break;
    case 5:
        newFormation = FleetFormation::COLUMN;
        break;
    case 6:
        newFormation = FleetFormation::CUSTOM;
        break;
    default:
        return;
    }

    fleet->setFormation(newFormation);
    m_ui.displayMessage("Formation changée avec succès.");

    // Si on passe en formation personnalisée, proposer de la configurer
    if (newFormation == FleetFormation::CUSTOM)
    {
        m_ui.displayMessage("Voulez-vous configurer les positions personnalisées? (1: Oui, 2: Non)");
        if (m_ui.getIntInput("Votre choix: ", 1, 2) == 1)
        {
            customizeFormation();
        }
    }

    m_ui.waitForKeyPress();
}

void FleetManager::customizeFormation()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("PERSONNALISATION DE LA FORMATION");

    // Passer automatiquement en formation personnalisée
    fleet->setFormation(FleetFormation::CUSTOM);

    m_ui.displayMessage("Définissez la position relative de chaque navire.");
    m_ui.displayMessage("Le navire amiral est au centre (0,0).");
    m_ui.displayMessage("Coordonnées positives: droite/haut, négatives: gauche/bas");
    m_ui.displayMessage("");

    for (size_t i = 1; i < fleet->getSize(); ++i) // Commencer à 1 pour skip l'amiral
    {
        auto ship = fleet->getShip(i);
        m_ui.displayMessage("Position du navire: " + ship->getName());

        int x = m_ui.getIntInput("Position X (-5 à 5): ", -5, 5);
        int y = m_ui.getIntInput("Position Y (-5 à 5): ", -5, 5);

        fleet->setCustomPosition(i, x, y);
    }

    m_ui.displayMessage("Formation personnalisée configurée avec succès.");
    m_ui.waitForKeyPress();
}

void FleetManager::trainFleetInFormation()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("ENTRAÎNEMENT DE LA FLOTTE");

    // Calculer le coût de l'entraînement
    int baseCost = 100 * fleet->getSize();
    int trainingCost = baseCost * (1 + fleet->getFleetExperience() / 1000);

    m_ui.displayMessage("Coût de l'entraînement: " + std::to_string(trainingCost) + " pièces d'or");
    m_ui.displayMessage("L'entraînement améliorera la cohésion de la flotte et l'expérience des équipages.");

    if (m_player->getGold() < trainingCost)
    {
        m_ui.displayError("Vous n'avez pas assez d'or pour cet entraînement.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Confirmer l'entraînement? (1: Oui, 2: Non)");
    if (m_ui.getIntInput("Votre choix: ", 1, 2) == 2)
    {
        m_ui.displayMessage("Entraînement annulé.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer l'entraînement
    m_player->removeGold(trainingCost);

    // Améliorer la cohésion
    fleet->adjustCohesion(10.0f);

    // Ajouter de l'expérience
    fleet->addExperience(100);

    // Réduire légèrement la fatigue
    fleet->restCrew(5.0f);

    m_ui.displayMessage("Entraînement terminé avec succès!");
    m_ui.displayMessage("La cohésion et l'expérience de la flotte ont augmenté.");
    m_ui.waitForKeyPress();
}

void FleetManager::manageFleetSupplies()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    bool exitMenu = false;
    while (!exitMenu)
    {
        m_ui.displayTitle("GESTION DES PROVISIONS");

        // Afficher l'état actuel des provisions
        showSuppliesStatus();

        // Créer le menu
        std::vector<ConsoleUI::MenuItem> menuItems = {
            {"Réapprovisionner les provisions", [this]()
             { restockProvisions(); }},
            {"Distribuer les rations", [this]()
             { distributeRations(); }},
            {"Remonter le moral", [this]()
             { boostCrewMorale(); }},
            {"Retour au menu précédent", [&exitMenu]()
             { exitMenu = true; }}};

        m_ui.displayMenu("GESTION DES PROVISIONS", menuItems);
    }
}

void FleetManager::showSuppliesStatus()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    // Afficher les jauges d'état
    showProvisionsGauge();
    showMoraleGauge();
    showFatigueGauge();
    showCohesionGauge();
    m_ui.displayMessage("");

    // Afficher les détails des provisions
    m_ui.displayMessage("Provisions restantes: " + std::to_string(fleet->getProvisions()) + " jours");
    m_ui.displayMessage("Consommation quotidienne: " + std::to_string(fleet->getCurrentCrew() / 50 + 1) + " unités");

    // Calculer l'autonomie
    int daysRemaining = fleet->getProvisions() / (fleet->getCurrentCrew() / 50 + 1);
    m_ui.displayMessage("Autonomie estimée: " + std::to_string(daysRemaining) + " jours");
}

void FleetManager::restockProvisions()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("RÉAPPROVISIONNEMENT");

    // Calculer le coût des provisions
    int currentProvisions = fleet->getProvisions();
    int maxProvisions = 30; // Maximum 30 jours de provisions
    int neededProvisions = maxProvisions - currentProvisions;

    if (neededProvisions <= 0)
    {
        m_ui.displayMessage("Vos provisions sont déjà au maximum (30 jours).");
        m_ui.waitForKeyPress();
        return;
    }

    // Coût basé sur la taille de l'équipage
    int costPerDay = fleet->getCurrentCrew() / 10 + 5; // 5 or par jour + 1 par 10 marins
    int totalCost = neededProvisions * costPerDay;

    m_ui.displayMessage("Coût du réapprovisionnement: " + std::to_string(totalCost) + " pièces d'or");
    m_ui.displayMessage("Cela vous donnera " + std::to_string(maxProvisions) + " jours de provisions.");

    if (m_player->getGold() < totalCost)
    {
        m_ui.displayError("Vous n'avez pas assez d'or pour ce réapprovisionnement.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Confirmer l'achat? (1: Oui, 2: Non)");
    if (m_ui.getIntInput("Votre choix: ", 1, 2) == 2)
    {
        m_ui.displayMessage("Achat annulé.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer l'achat
    m_player->removeGold(totalCost);
    fleet->setProvisions(maxProvisions);

    m_ui.displayMessage("Réapprovisionnement effectué avec succès!");
    m_ui.waitForKeyPress();
}

void FleetManager::distributeRations()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("DISTRIBUTION DES RATIONS");

    if (fleet->getProvisions() < 2)
    {
        m_ui.displayError("Vous n'avez pas assez de provisions pour une distribution spéciale.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("Une distribution généreuse de rations améliorera le moral mais consommera plus de provisions.");
    m_ui.displayMessage("Coût: 2 jours de provisions");
    m_ui.displayMessage("Effet: +10 moral, -5 fatigue");

    m_ui.displayMessage("\nConfirmer la distribution? (1: Oui, 2: Non)");
    if (m_ui.getIntInput("Votre choix: ", 1, 2) == 2)
    {
        m_ui.displayMessage("Distribution annulée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer la distribution
    fleet->consumeProvisions(2);
    fleet->adjustMorale(10.0f);
    fleet->restCrew(5.0f);

    m_ui.displayMessage("Distribution effectuée! Le moral de l'équipage s'est amélioré.");
    m_ui.waitForKeyPress();
}

void FleetManager::boostCrewMorale()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayTitle("REMONTER LE MORAL");

    // Coût basé sur la taille de l'équipage
    int baseCost = 50 * fleet->getSize();
    int moraleCost = baseCost * (1 + static_cast<int>(fleet->getMorale()) / 50);

    m_ui.displayMessage("Organiser des activités pour l'équipage coûtera " +
                        std::to_string(moraleCost) + " pièces d'or");
    m_ui.displayMessage("Effet: +15 moral, +5 cohésion");

    if (m_player->getGold() < moraleCost)
    {
        m_ui.displayError("Vous n'avez pas assez d'or pour ces activités.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayMessage("\nConfirmer l'organisation des activités? (1: Oui, 2: Non)");
    if (m_ui.getIntInput("Votre choix: ", 1, 2) == 2)
    {
        m_ui.displayMessage("Activités annulées.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer les activités
    m_player->removeGold(moraleCost);
    fleet->adjustMorale(15.0f);
    fleet->adjustCohesion(5.0f);

    m_ui.displayMessage("Activités réussies! Le moral et la cohésion de l'équipage se sont améliorés.");
    m_ui.waitForKeyPress();
}

void FleetManager::showFleetStatusGauge()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    m_ui.displayMessage("\nÉTAT DE LA FLOTTE:");

    // Afficher les jauges d'état
    showMoraleGauge();
    showFatigueGauge();
    showProvisionsGauge();
    showCohesionGauge();
}

void FleetManager::showMoraleGauge()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    displayGauge("Moral", fleet->getMorale(), 100.0f);
}

void FleetManager::showFatigueGauge()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    displayGauge("Fatigue", fleet->getCrewFatigue(), 100.0f);
}

void FleetManager::showProvisionsGauge()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    displayGauge("Provisions", static_cast<float>(fleet->getProvisions()), 30.0f);
}

void FleetManager::showCohesionGauge()
{
    auto fleet = m_player->getFleet();
    if (!fleet)
        return;

    displayGauge("Cohésion", fleet->getCohesion(), 100.0f);
}

void FleetManager::displayGauge(const std::string &label, float value, float maxValue,
                                int width, char fillChar, char emptyChar)
{
    int fillWidth = static_cast<int>((value / maxValue) * width);
    fillWidth = std::min(width, std::max(0, fillWidth));

    std::stringstream ss;
    ss << std::setw(10) << std::left << label << " [";
    for (int i = 0; i < width; ++i)
    {
        ss << (i < fillWidth ? fillChar : emptyChar);
    }
    ss << "] " << std::fixed << std::setprecision(1) << value << "/" << maxValue;

    m_ui.displayMessage(ss.str());
}

void FleetManager::performEmergencyRepairs()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("RÉPARATIONS D'URGENCE");

    // Afficher l'état de tous les navires
    m_ui.displayMessage("État des navires:");
    bool needsRepair = false;
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        float durabilityPercent = ship->getDurabilityPercent();
        std::string status = durabilityPercent < 25.0f ? " [CRITIQUE]" : durabilityPercent < 50.0f ? " [ENDOMMAGÉ]"
                                                                     : durabilityPercent < 75.0f   ? " [ABÎMÉ]"
                                                                                                   : "";

        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName()
           << " - État: " << std::fixed << std::setprecision(1)
           << durabilityPercent << "%" << status;

        m_ui.displayMessage(ss.str());

        if (durabilityPercent < 100.0f)
            needsRepair = true;
    }

    if (!needsRepair)
    {
        m_ui.displayMessage("\nTous les navires sont en bon état.");
        m_ui.waitForKeyPress();
        return;
    }

    // Sélectionner le navire à réparer
    int shipIndex = m_ui.getIntInput("\nChoisissez le navire à réparer (1-" +
                                         std::to_string(fleet->getSize()) + "): ",
                                     1, fleet->getSize()) -
                    1;

    auto ship = fleet->getShip(shipIndex);
    if (ship->getDurabilityPercent() >= 100.0f)
    {
        m_ui.displayMessage("Ce navire n'a pas besoin de réparations.");
        m_ui.waitForKeyPress();
        return;
    }

    // Calculer les ressources nécessaires
    int damagePoints = ship->getDurability() - ship->getCurrentDurability();
    int requiredWood = damagePoints / 2;
    int requiredCrew = std::max(5, damagePoints / 10);
    float repairTime = damagePoints * 0.5f; // 0.5 minutes par point de dégât

    m_ui.displayMessage("\nRessources nécessaires pour la réparation:");
    m_ui.displayMessage("- Bois: " + std::to_string(requiredWood) + " unités");
    m_ui.displayMessage("- Équipage: " + std::to_string(requiredCrew) + " marins");
    m_ui.displayMessage("- Temps estimé: " + std::to_string(static_cast<int>(repairTime)) + " minutes");

    // Vérifier les ressources disponibles
    if (fleet->getWoodSupplies() < requiredWood)
    {
        m_ui.displayError("Vous n'avez pas assez de bois pour effectuer les réparations.");
        m_ui.waitForKeyPress();
        return;
    }

    if (ship->getCurrentCrew() < requiredCrew)
    {
        m_ui.displayError("Vous n'avez pas assez d'équipage disponible pour effectuer les réparations.");
        m_ui.waitForKeyPress();
        return;
    }

    // Confirmer la réparation
    m_ui.displayMessage("\nConfirmer les réparations d'urgence? (1: Oui, 2: Non)");
    if (m_ui.getIntInput("Votre choix: ", 1, 2) == 2)
    {
        m_ui.displayMessage("Réparations annulées.");
        m_ui.waitForKeyPress();
        return;
    }

    // Effectuer les réparations
    fleet->useWoodSupplies(requiredWood);
    ship->repair(damagePoints);
    ship->increaseFatigue(10.0f); // L'équipage est fatigué par les réparations

    m_ui.displayMessage("Réparations d'urgence effectuées!");
    m_ui.displayMessage("Nouvel état du navire: " +
                        std::to_string(static_cast<int>(ship->getDurabilityPercent())) + "%");
    m_ui.waitForKeyPress();
}

void FleetManager::reorganizeCrew()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() < 2)
    {
        m_ui.displayMessage("Vous avez besoin d'au moins deux navires pour réorganiser l'équipage.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("RÉORGANISATION DE L'ÉQUIPAGE");

    // Afficher l'état de l'équipage pour chaque navire
    m_ui.displayMessage("État de l'équipage par navire:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName()
           << "\n   Équipage: " << ship->getCurrentCrew() << "/" << ship->getMaxCrew()
           << " (min: " << ship->getRequiredCrew() << ")"
           << "\n   Moral: " << std::fixed << std::setprecision(1) << ship->getCrewMorale() << "%"
           << "\n   Fatigue: " << ship->getCrewFatigue() << "%";

        m_ui.displayMessage(ss.str());
    }

    // Sélectionner le navire source
    m_ui.displayMessage("\nSélectionnez le navire SOURCE:");
    int sourceIndex = m_ui.getIntInput("Navire (1-" + std::to_string(fleet->getSize()) + "): ",
                                       1, fleet->getSize()) -
                      1;

    auto sourceShip = fleet->getShip(sourceIndex);
    int maxTransfer = sourceShip->getCurrentCrew() - sourceShip->getRequiredCrew();

    if (maxTransfer <= 0)
    {
        m_ui.displayMessage("Ce navire ne peut pas transférer d'équipage sans compromettre son fonctionnement.");
        m_ui.waitForKeyPress();
        return;
    }

    // Sélectionner le navire destination
    m_ui.displayMessage("\nSélectionnez le navire DESTINATION:");
    int destIndex;
    do
    {
        destIndex = m_ui.getIntInput("Navire (1-" + std::to_string(fleet->getSize()) + "): ",
                                     1, fleet->getSize()) -
                    1;
    } while (destIndex == sourceIndex);

    auto destShip = fleet->getShip(destIndex);
    int maxReceive = destShip->getMaxCrew() - destShip->getCurrentCrew();

    if (maxReceive <= 0)
    {
        m_ui.displayMessage("Ce navire ne peut pas recevoir plus d'équipage.");
        m_ui.waitForKeyPress();
        return;
    }

    // Déterminer le nombre de marins à transférer
    int transferAmount = m_ui.getIntInput("Nombre de marins à transférer (1-" +
                                              std::to_string(std::min(maxTransfer, maxReceive)) + "): ",
                                          1, std::min(maxTransfer, maxReceive));

    // Effectuer le transfert
    sourceShip->removeCrew(transferAmount);
    destShip->addCrew(transferAmount);

    // Ajuster le moral et la fatigue
    float averageMorale = (sourceShip->getCrewMorale() + destShip->getCrewMorale()) / 2.0f;
    float averageFatigue = (sourceShip->getCrewFatigue() + destShip->getCrewFatigue()) / 2.0f;

    sourceShip->adjustMorale(-5.0f); // Pénalité de moral pour la séparation
    destShip->adjustMorale(-5.0f);
    sourceShip->increaseFatigue(5.0f); // Fatigue due à la réorganisation
    destShip->increaseFatigue(5.0f);

    m_ui.displayMessage("\nTransfert d'équipage effectué avec succès!");
    m_ui.displayMessage("N'oubliez pas que la réorganisation affecte temporairement le moral et la fatigue.");
    m_ui.waitForKeyPress();
}

void FleetManager::manageCrewStations()
{
    auto fleet = m_player->getFleet();
    if (!fleet || fleet->getSize() == 0)
    {
        m_ui.displayMessage("Vous n'avez pas de navire dans votre flotte.");
        m_ui.waitForKeyPress();
        return;
    }

    m_ui.displayTitle("POSTES DE L'ÉQUIPAGE");

    // Sélectionner le navire
    m_ui.displayMessage("Choisissez un navire:");
    for (size_t i = 0; i < fleet->getSize(); ++i)
    {
        auto ship = fleet->getShip(i);
        std::stringstream ss;
        ss << (i + 1) << ". " << ship->getName()
           << " - Équipage: " << ship->getCurrentCrew() << "/" << ship->getMaxCrew();
        m_ui.displayMessage(ss.str());
    }

    int shipIndex = m_ui.getIntInput("\nNavire (1-" + std::to_string(fleet->getSize()) + "): ",
                                     1, fleet->getSize()) -
                    1;

    auto ship = fleet->getShip(shipIndex);

    bool exitMenu = false;
    while (!exitMenu)
    {
        m_ui.displayTitle("GESTION DES POSTES - " + ship->getName());

        // Afficher la répartition actuelle
        m_ui.displayMessage("Répartition actuelle de l'équipage:");
        m_ui.displayMessage("Navigation: " + std::to_string(ship->getCrewAtStation("navigation")));
        m_ui.displayMessage("Canonniers: " + std::to_string(ship->getCrewAtStation("cannons")));
        m_ui.displayMessage("Voilure: " + std::to_string(ship->getCrewAtStation("sails")));
        m_ui.displayMessage("Réparations: " + std::to_string(ship->getCrewAtStation("repairs")));
        m_ui.displayMessage("Repos: " + std::to_string(ship->getCrewAtStation("resting")));
        m_ui.displayMessage("");

        std::vector<ConsoleUI::MenuItem> menuItems = {
            {"Modifier les postes", [this, ship]()
             { reassignCrewStations(ship); }},
            {"Rotation des équipes", [this, ship]()
             { rotateCrewShifts(ship); }},
            {"Retour au menu précédent", [&exitMenu]()
             { exitMenu = true; }}};

        m_ui.displayMenu("GESTION DES POSTES", menuItems);
    }
}

void FleetManager::reassignCrewStations(std::shared_ptr<Ship> ship)
{
    if (!ship)
        return;

    m_ui.displayTitle("RÉAFFECTATION DES POSTES");

    int totalCrew = ship->getCurrentCrew();
    int unassigned = totalCrew;
    std::map<std::string, int> assignments;

    // Assigner l'équipage aux différents postes
    std::vector<std::pair<std::string, std::string>> stations = {
        {"navigation", "Navigation"},
        {"cannons", "Canonniers"},
        {"sails", "Voilure"},
        {"repairs", "Réparations"},
        {"resting", "Repos"}};

    for (const auto &[key, label] : stations)
    {
        if (unassigned <= 0)
            break;

        int current = ship->getCrewAtStation(key);
        m_ui.displayMessage(label + " actuel: " + std::to_string(current));

        int max = (key == "resting") ? unassigned : std::min(unassigned, static_cast<int>(totalCrew * 0.4f));

        int assigned = m_ui.getIntInput("Assigner combien de marins? (0-" +
                                            std::to_string(max) + "): ",
                                        0, max);

        assignments[key] = assigned;
        unassigned -= assigned;

        m_ui.displayMessage("Reste à assigner: " + std::to_string(unassigned));
        m_ui.displayMessage("");
    }

    // Vérifier que tout l'équipage est assigné
    if (unassigned > 0)
    {
        assignments["resting"] += unassigned; // Mettre le reste en repos
    }

    // Appliquer les changements
    for (const auto &[station, count] : assignments)
    {
        ship->setCrewAtStation(station, count);
    }

    m_ui.displayMessage("Réaffectation terminée!");
    m_ui.waitForKeyPress();
}

void FleetManager::rotateCrewShifts(std::shared_ptr<Ship> ship)
{
    if (!ship)
        return;

    m_ui.displayTitle("ROTATION DES ÉQUIPES");

    // Calculer la fatigue moyenne
    float avgFatigue = ship->getCrewFatigue();
    m_ui.displayMessage("Fatigue moyenne de l'équipage: " +
                        std::to_string(static_cast<int>(avgFatigue)) + "%");

    // Suggérer une rotation basée sur la fatigue
    m_ui.displayMessage("\nRotation recommandée:");
    if (avgFatigue > 75.0f)
    {
        m_ui.displayMessage("- Augmenter l'équipe au repos de 50%");
        m_ui.displayMessage("- Réduire les équipes actives proportionnellement");
    }
    else if (avgFatigue > 50.0f)
    {
        m_ui.displayMessage("- Augmenter l'équipe au repos de 25%");
        m_ui.displayMessage("- Réduire les équipes actives légèrement");
    }
    else
    {
        m_ui.displayMessage("- Maintenir la répartition actuelle");
        m_ui.displayMessage("- Rotation normale des équipes");
    }

    // Proposer des options de rotation
    m_ui.displayMessage("\nOptions de rotation:");
    m_ui.displayMessage("1. Rotation standard (8 heures)");
    m_ui.displayMessage("2. Rotation intensive (4 heures)");
    m_ui.displayMessage("3. Rotation de repos (12 heures)");
    m_ui.displayMessage("4. Annuler");

    int choice = m_ui.getIntInput("Votre choix: ", 1, 4);

    if (choice == 4)
    {
        m_ui.displayMessage("Rotation annulée.");
        m_ui.waitForKeyPress();
        return;
    }

    // Appliquer la rotation
    float restBonus = 0.0f;
    float fatigueReduction = 0.0f;

    switch (choice)
    {
    case 1: // Standard
        restBonus = 10.0f;
        fatigueReduction = 15.0f;
        break;
    case 2: // Intensive
        restBonus = 5.0f;
        fatigueReduction = 10.0f;
        break;
    case 3: // Repos
        restBonus = 20.0f;
        fatigueReduction = 25.0f;
        break;
    }

    // Appliquer les effets
    ship->restCrew(fatigueReduction);
    ship->adjustMorale(restBonus);

    m_ui.displayMessage("\nRotation effectuée!");
    m_ui.displayMessage("L'équipage est plus reposé et le moral s'est amélioré.");
    m_ui.waitForKeyPress();
}