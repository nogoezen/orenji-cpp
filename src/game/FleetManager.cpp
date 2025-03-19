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