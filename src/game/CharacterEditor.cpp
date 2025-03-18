#include "CharacterEditor.h"
#include <iomanip>
#include <sstream>
#include <iostream>

// Méthode principale de l'éditeur de personnage
std::shared_ptr<Character> CharacterEditor::startEditor(const std::string &playerName)
{
    m_ui.displayTitle("CRÉATION DE PERSONNAGE");
    m_ui.displayMessage("Bienvenue dans l'éditeur de personnage, Capitaine " + playerName + "!");
    m_ui.displayMessage("Vous allez pouvoir personnaliser votre aventurier avant de prendre la mer.");
    m_ui.waitForKeyPress();

    // Choisir un personnage de base
    m_ui.displayMessage("Étape 1: Choisir un modèle de personnage");
    m_character = chooseCharacterTemplate();
    if (!m_character)
    {
        m_ui.displayError("Erreur lors de la sélection du personnage.");
        return nullptr;
    }
    m_ui.displayMessage("Personnage de base sélectionné : " + m_character->getName());

    // Éditer le nom du personnage
    m_ui.displayMessage("Étape 2: Personnaliser le nom");
    editCharacterName();
    m_ui.displayMessage("Nom défini : " + m_character->getName());

    // Choisir une classe principale
    m_ui.displayMessage("Étape 3: Choisir la classe principale");
    chooseMainClass();
    m_ui.displayMessage("Classe principale : " + getClassName(m_character->getClassId()));

    // Choisir des classes secondaires
    m_ui.displayMessage("Étape 4: Choisir des classes secondaires (optionnel)");
    chooseSecondaryClasses();

    // Afficher les classes secondaires
    const auto &secClasses = m_character->getSecondaryClasses();
    if (!secClasses.empty())
    {
        std::string secondaryClassesStr = "Classes secondaires : ";
        for (size_t i = 0; i < secClasses.size(); ++i)
        {
            secondaryClassesStr += getClassName(secClasses[i]);
            if (i < secClasses.size() - 1)
            {
                secondaryClassesStr += ", ";
            }
        }
        m_ui.displayMessage(secondaryClassesStr);
    }
    else
    {
        m_ui.displayMessage("Aucune classe secondaire sélectionnée.");
    }

    // Attribuer les points de caractéristiques
    m_ui.displayMessage("Étape 5: Attribuer les points de caractéristiques");
    assignAttributePoints();

    // Afficher le récapitulatif
    m_ui.displayTitle("PERSONNAGE CRÉÉ");
    displayCharacterDetails(*m_character);
    m_ui.displayMessage("\nVotre personnage est prêt à prendre la mer !");
    m_ui.waitForKeyPress();

    return m_character;
}

// Afficher les détails d'un personnage
void CharacterEditor::displayCharacterDetails(const Character &character) const
{
    m_ui.displayMessage("Nom : " + character.getName());
    m_ui.displayMessage("Surnom : " + character.getNickname());
    m_ui.displayMessage("Titre : " + character.getTitle().current + " (Rang " + std::to_string(character.getTitle().rank) + ")");

    // Afficher la classe principale
    std::string mainClassName = getClassName(character.getClassId());
    m_ui.displayMessage("Classe principale : " + mainClassName);

    // Afficher les classes secondaires
    std::string secondaryClasses = "Classes secondaires : ";
    const auto &secClasses = character.getSecondaryClasses();
    if (secClasses.empty())
    {
        secondaryClasses += "Aucune";
    }
    else
    {
        for (size_t i = 0; i < secClasses.size(); ++i)
        {
            secondaryClasses += getClassName(secClasses[i]);
            if (i < secClasses.size() - 1)
            {
                secondaryClasses += ", ";
            }
        }
    }
    m_ui.displayMessage(secondaryClasses);

    // Afficher les attributs
    m_ui.displayMessage("\nAttributs :");
    const auto &params = character.getParams();
    if (params.size() >= 8)
    {
        m_ui.displayMessage("Santé : " + std::to_string(params[0]));
        m_ui.displayMessage("Endurance : " + std::to_string(params[1]));
        m_ui.displayMessage("Force : " + std::to_string(params[2]));
        m_ui.displayMessage("Dextérité : " + std::to_string(params[3]));
        m_ui.displayMessage("Intelligence : " + std::to_string(params[4]));
        m_ui.displayMessage("Sagesse : " + std::to_string(params[5]));
        m_ui.displayMessage("Agilité : " + std::to_string(params[6]));
        m_ui.displayMessage("Chance : " + std::to_string(params[7]));
    }

    // Afficher la moralité
    const auto &morality = character.getMorality();
    std::string alignmentStr = "Moralité : " + std::to_string(morality.value) + " (" + morality.alignment + ")";
    m_ui.displayMessage("\n" + alignmentStr);

    // Afficher quelques réputations
    m_ui.displayMessage("Réputation :");
    for (const auto &[nation, rep] : morality.reputation)
    {
        m_ui.displayMessage("  " + nation + ": " + std::to_string(rep));
    }

    // Afficher le profil
    m_ui.displayMessage("\nProfil : " + character.getProfile());

    m_ui.displayMessage("\n");
}

// Afficher les détails d'une classe
void CharacterEditor::displayClassDetails(int classId) const
{
    for (const auto &classData : m_classesData)
    {
        if (classData["id"] == classId)
        {
            m_ui.displayMessage("Nom : " + classData["name"].get<std::string>());
            m_ui.displayMessage("Description : " + classData["description"].get<std::string>());

            // Afficher quelques statistiques de base
            m_ui.displayMessage("Statistiques de base :");
            const auto &params = classData["params"];
            if (params.size() >= 8 && params[0].size() >= 1)
            {
                m_ui.displayMessage("  Santé : " + std::to_string(params[0][0].get<int>()));
                m_ui.displayMessage("  Endurance : " + std::to_string(params[1][0].get<int>()));
                m_ui.displayMessage("  Force : " + std::to_string(params[2][0].get<int>()));
                m_ui.displayMessage("  Dextérité : " + std::to_string(params[3][0].get<int>()));
                m_ui.displayMessage("  Intelligence : " + std::to_string(params[4][0].get<int>()));
                m_ui.displayMessage("  Sagesse : " + std::to_string(params[5][0].get<int>()));
                m_ui.displayMessage("  Agilité : " + std::to_string(params[6][0].get<int>()));
                m_ui.displayMessage("  Chance : " + std::to_string(params[7][0].get<int>()));
            }

            // Afficher quelques compétences débloquées
            m_ui.displayMessage("Compétences débloquées au niveau 1 :");
            for (const auto &learning : classData["learnings"])
            {
                if (learning["level"] == 1)
                {
                    m_ui.displayMessage("  Compétence ID : " + std::to_string(learning["skillId"].get<int>()));
                }
            }

            return;
        }
    }

    m_ui.displayMessage("Classe non trouvée (ID : " + std::to_string(classId) + ")");
}

// Obtenir le nom d'une classe à partir de son ID
std::string CharacterEditor::getClassName(int classId) const
{
    for (const auto &classData : m_classesData)
    {
        if (classData["id"] == classId)
        {
            return classData["name"].get<std::string>();
        }
    }
    return "Classe inconnue";
}

// Choisir un modèle de personnage prédéfini
std::shared_ptr<Character> CharacterEditor::chooseCharacterTemplate() const
{
    m_ui.displayTitle("CHOIX DU MODÈLE DE PERSONNAGE");
    m_ui.displayMessage("Choisissez un archétype de personnage comme point de départ :");

    const auto &actorsData = GameData::getInstance().getActors();
    std::vector<std::shared_ptr<Character>> availableCharacters;

    // Créer des objets Character à partir des données JSON
    for (const auto &actorData : actorsData)
    {
        availableCharacters.push_back(std::make_shared<Character>(actorData));
    }

    // Boucle de sélection
    int selectedIndex = 0;
    bool selecting = true;
    while (selecting)
    {
        // Effacer l'écran et afficher l'en-tête
        m_ui.clearScreen();
        m_ui.displayTitle("SÉLECTION DE PERSONNAGE");

        // Afficher les détails du personnage actuellement sélectionné
        displayCharacterDetails(*availableCharacters[selectedIndex]);

        // Afficher les options directement
        m_ui.displayMessage("\nOptions disponibles :");
        m_ui.displayMessage("1. Voir le personnage précédent");
        m_ui.displayMessage("2. Voir le personnage suivant");
        m_ui.displayMessage("3. Afficher les détails de la classe");
        m_ui.displayMessage("4. Sélectionner ce personnage");

        int choice = m_ui.getIntInput("Votre choix : ", 1, 4);

        switch (choice)
        {
        case 1: // Personnage précédent
            selectedIndex = (selectedIndex + availableCharacters.size() - 1) % availableCharacters.size();
            break;
        case 2: // Personnage suivant
            selectedIndex = (selectedIndex + 1) % availableCharacters.size();
            break;
        case 3: // Détails de la classe
        {
            m_ui.displayTitle("DÉTAILS DE LA CLASSE");
            int classId = availableCharacters[selectedIndex]->getClassId();
            displayClassDetails(classId);
            m_ui.waitForKeyPress();
        }
        break;
        case 4: // Sélectionner
            selecting = false;
            break;
        }
    }

    // Créer une copie du personnage sélectionné
    auto selectedCharacter = std::make_shared<Character>(*availableCharacters[selectedIndex]);
    m_ui.displayMessage("Vous avez choisi : " + selectedCharacter->getName() + " - " + getClassName(selectedCharacter->getClassId()));
    m_ui.waitForKeyPress();

    return selectedCharacter;
}

// Modifier le nom du personnage
void CharacterEditor::editCharacterName()
{
    m_ui.displayTitle("MODIFICATION DU NOM");
    std::string currentName = m_character->getName();
    m_ui.displayMessage("Nom actuel : " + currentName);

    std::string newName = m_ui.getInput("Entrez le nouveau nom (ou laissez vide pour garder l'actuel) : ");

    if (!newName.empty())
    {
        m_character->setName(newName);
        m_ui.displayMessage("Nom changé en : " + newName);
    }
    else
    {
        m_ui.displayMessage("Nom inchangé.");
    }

    m_ui.waitForKeyPress();
}

// Choisir la classe principale
void CharacterEditor::chooseMainClass()
{
    m_ui.displayTitle("CHOIX DE LA CLASSE PRINCIPALE");

    // Afficher la classe actuelle
    m_ui.displayMessage("Classe actuelle : " + getClassName(m_character->getClassId()));

    // Afficher toutes les classes disponibles
    m_ui.displayMessage("\nClasses disponibles :");

    std::vector<std::pair<int, std::string>> availableClasses;
    for (const auto &classData : m_classesData)
    {
        int classId = classData["id"];
        std::string className = classData["name"].get<std::string>();
        availableClasses.push_back({classId, className});

        m_ui.displayMessage(std::to_string(availableClasses.size()) + ". " + className);
    }

    bool selecting = true;
    while (selecting)
    {
        m_ui.displayMessage("\nOptions :");
        m_ui.displayMessage("1. Afficher les détails d'une classe");
        m_ui.displayMessage("2. Sélectionner une classe");
        m_ui.displayMessage("3. Retour (garder la classe actuelle)");

        int option = m_ui.getIntInput("Votre choix : ", 1, 3);

        switch (option)
        {
        case 1: // Afficher les détails
        {
            int classChoice = m_ui.getIntInput("Numéro de la classe à afficher : ", 1, availableClasses.size());
            int classId = availableClasses[classChoice - 1].first;

            m_ui.displayTitle("DÉTAILS DE LA CLASSE");
            displayClassDetails(classId);
            m_ui.waitForKeyPress();
            m_ui.displayTitle("CHOIX DE LA CLASSE PRINCIPALE");
            m_ui.displayMessage("Classe actuelle : " + getClassName(m_character->getClassId()));
            m_ui.displayMessage("\nClasses disponibles :");
            for (size_t i = 0; i < availableClasses.size(); ++i)
            {
                m_ui.displayMessage(std::to_string(i + 1) + ". " + availableClasses[i].second);
            }
        }
        break;
        case 2: // Sélectionner la classe
        {
            int classChoice = m_ui.getIntInput("Numéro de la classe à sélectionner : ", 1, availableClasses.size());
            int newClassId = availableClasses[classChoice - 1].first;

            // Mettre à jour la classe du personnage
            m_character->setClassId(newClassId);

            // Mettre à jour les paramètres du personnage en fonction de la nouvelle classe
            for (const auto &classData : m_classesData)
            {
                if (classData["id"] == newClassId)
                {
                    const auto &params = classData["params"];
                    if (params.size() >= 8 && params[0].size() >= 1)
                    {
                        for (int i = 0; i < 8; ++i)
                        {
                            m_character->setParam(i, params[i][0].get<int>());
                        }
                    }
                    break;
                }
            }

            m_ui.displayMessage("Classe principale changée en : " + getClassName(newClassId));
            m_ui.waitForKeyPress();
            selecting = false;
        }
        break;
        case 3: // Retour
            selecting = false;
            break;
        }
    }
}

// Choisir les classes secondaires
void CharacterEditor::chooseSecondaryClasses()
{
    bool editing = true;

    while (editing)
    {
        m_ui.displayTitle("CHOIX DES CLASSES SECONDAIRES");

        // Afficher les classes secondaires actuelles
        std::string currentSecondaryClasses = "Classes secondaires actuelles : ";
        const auto &secClasses = m_character->getSecondaryClasses();
        if (secClasses.empty())
        {
            currentSecondaryClasses += "Aucune";
        }
        else
        {
            for (size_t i = 0; i < secClasses.size(); ++i)
            {
                currentSecondaryClasses += getClassName(secClasses[i]);
                if (i < secClasses.size() - 1)
                {
                    currentSecondaryClasses += ", ";
                }
            }
        }
        m_ui.displayMessage(currentSecondaryClasses);

        // Afficher les options directement
        m_ui.displayMessage("\nOptions :");
        m_ui.displayMessage("1. Ajouter une classe secondaire");
        m_ui.displayMessage("2. Supprimer une classe secondaire");
        m_ui.displayMessage("3. Terminer et revenir");

        int choice = m_ui.getIntInput("Votre choix : ", 1, 3);

        switch (choice)
        {
        case 1: // Ajouter
        {
            // Créer une liste de classes disponibles (excluant la classe principale)
            std::vector<std::pair<int, std::string>> availableClasses;
            for (const auto &classData : m_classesData)
            {
                int classId = classData["id"];
                if (classId != m_character->getClassId())
                {
                    availableClasses.push_back({classId, classData["name"].get<std::string>()});
                }
            }

            // Filtrer les classes déjà sélectionnées
            std::vector<std::pair<int, std::string>> filteredClasses;
            const auto &secClasses = m_character->getSecondaryClasses();
            for (const auto &[id, name] : availableClasses)
            {
                bool alreadySelected = false;
                for (int secClassId : secClasses)
                {
                    if (id == secClassId)
                    {
                        alreadySelected = true;
                        break;
                    }
                }
                if (!alreadySelected)
                {
                    filteredClasses.push_back({id, name});
                }
            }

            if (filteredClasses.empty())
            {
                m_ui.displayMessage("Toutes les classes sont déjà sélectionnées.");
                m_ui.waitForKeyPress();
                break;
            }

            // Menu pour choisir une classe secondaire
            m_ui.displayTitle("AJOUT D'UNE CLASSE SECONDAIRE");
            m_ui.displayMessage("Choisissez une classe secondaire :");

            for (size_t i = 0; i < filteredClasses.size(); ++i)
            {
                m_ui.displayMessage(std::to_string(i + 1) + ". " + filteredClasses[i].second);
            }

            int addChoice = m_ui.getIntInput("Votre choix : ", 1, filteredClasses.size());
            int selectedClassId = filteredClasses[addChoice - 1].first;

            // Ajouter la classe sélectionnée
            m_character->addSecondaryClass(selectedClassId);
            m_ui.displayMessage("Classe secondaire ajoutée : " + getClassName(selectedClassId));
            m_ui.waitForKeyPress();
        }
        break;
        case 2: // Supprimer
        {
            const auto &secClasses = m_character->getSecondaryClasses();
            if (secClasses.empty())
            {
                m_ui.displayMessage("Aucune classe secondaire à supprimer.");
                m_ui.waitForKeyPress();
                break;
            }

            // Menu pour choisir une classe secondaire à supprimer
            m_ui.displayTitle("SUPPRESSION D'UNE CLASSE SECONDAIRE");
            m_ui.displayMessage("Choisissez une classe secondaire à supprimer :");

            for (size_t i = 0; i < secClasses.size(); ++i)
            {
                m_ui.displayMessage(std::to_string(i + 1) + ". " + getClassName(secClasses[i]));
            }

            int removeChoice = m_ui.getIntInput("Votre choix : ", 1, secClasses.size());

            // Créer une nouvelle liste sans la classe sélectionnée
            std::vector<int> newSecClasses;
            for (size_t i = 0; i < secClasses.size(); ++i)
            {
                if (static_cast<size_t>(i) != static_cast<size_t>(removeChoice) - 1)
                {
                    newSecClasses.push_back(secClasses[i]);
                }
            }

            // Mettre à jour les classes secondaires du personnage directement
            m_character->clearSecondaryClasses();
            for (int classId : newSecClasses)
            {
                m_character->addSecondaryClass(classId);
            }

            m_ui.displayMessage("Classe secondaire supprimée.");
            m_ui.waitForKeyPress();
        }
        break;
        case 3: // Terminer
            editing = false;
            break;
        }
    }
}

// Attribuer les points de caractéristiques
void CharacterEditor::assignAttributePoints()
{
    m_ui.clearScreen();
    m_ui.displayTitle("ATTRIBUTION DES POINTS DE CARACTÉRISTIQUES");

    const int POINTS_TOTAUX = 30; // Points à distribuer

    // Stocker les valeurs actuelles des attributs
    std::vector<int> baseValues = m_character->getParams();
    while (baseValues.size() < 8)
    {
        baseValues.push_back(0);
    }

    // Copier les valeurs de base pour modification
    std::vector<int> currentValues = baseValues;

    // Points disponibles = POINTS_TOTAUX - somme des points ajoutés aux valeurs de base
    int pointsUtilises = 0;
    for (int i = 0; i < 8; ++i)
    {
        pointsUtilises += std::max(0, currentValues[i] - baseValues[i]);
    }
    int pointsDisponibles = std::max(0, POINTS_TOTAUX - pointsUtilises);

    // Noms des attributs
    std::vector<std::string> attributNames = {
        "Santé", "Endurance", "Force", "Dextérité",
        "Intelligence", "Sagesse", "Agilité", "Chance"};

    bool editing = true;
    while (editing)
    {
        m_ui.clearScreen();
        m_ui.displayTitle("ATTRIBUTION DES POINTS DE CARACTÉRISTIQUES");

        m_ui.displayMessage("Points disponibles : " + std::to_string(pointsDisponibles) + " sur " + std::to_string(POINTS_TOTAUX));
        m_ui.displayMessage("Valeurs actuelles :");

        for (int i = 0; i < 8; ++i)
        {
            std::stringstream ss;
            ss << std::setw(2) << (i + 1) << ". " << std::setw(12) << std::left << attributNames[i] << " : ";
            ss << std::setw(3) << std::right << currentValues[i];
            if (currentValues[i] > baseValues[i])
            {
                ss << " (+" << (currentValues[i] - baseValues[i]) << ")";
            }
            m_ui.displayMessage(ss.str());
        }

        m_ui.displayMessage("\nOptions :");
        m_ui.displayMessage("1. Augmenter un attribut");
        m_ui.displayMessage("2. Diminuer un attribut");
        m_ui.displayMessage("3. Réinitialiser aux valeurs de base");
        m_ui.displayMessage("4. Terminer l'édition");

        int choice = m_ui.getIntInput("Votre choix : ", 1, 4);

        switch (choice)
        {
        case 1: // Augmenter
        {
            if (pointsDisponibles <= 0)
            {
                m_ui.displayMessage("Vous n'avez plus de points disponibles !");
                m_ui.waitForKeyPress();
                break;
            }

            m_ui.displayMessage("Quel attribut souhaitez-vous augmenter ? (1-8) :");
            for (int i = 0; i < 8; ++i)
            {
                m_ui.displayMessage(std::to_string(i + 1) + ". " + attributNames[i] + " (" + std::to_string(currentValues[i]) + ")");
            }

            int attrIndex = m_ui.getIntInput("Votre choix : ", 1, 8) - 1;

            int maxToAdd = std::min(pointsDisponibles, 10); // Maximum 10 points à la fois
            int toAdd = m_ui.getIntInput("Combien de points ajouter ? (1-" + std::to_string(maxToAdd) + ") : ", 1, maxToAdd);

            currentValues[attrIndex] += toAdd;
            pointsDisponibles -= toAdd;

            m_ui.displayMessage(attributNames[attrIndex] + " augmenté de " + std::to_string(toAdd) + " points.");
            m_ui.waitForKeyPress();
        }
        break;
        case 2: // Diminuer
        {
            m_ui.displayMessage("Quel attribut souhaitez-vous diminuer ? (1-8) :");
            for (int i = 0; i < 8; ++i)
            {
                int bonus = currentValues[i] - baseValues[i];
                m_ui.displayMessage(std::to_string(i + 1) + ". " + attributNames[i] + " (" +
                                    std::to_string(currentValues[i]) +
                                    (bonus > 0 ? ", +" + std::to_string(bonus) + " points ajoutés)" : ")"));
            }

            int attrIndex = m_ui.getIntInput("Votre choix : ", 1, 8) - 1;

            // Ne pas aller en dessous de la valeur de base
            int maxToRemove = std::max(0, currentValues[attrIndex] - baseValues[attrIndex]);
            if (maxToRemove <= 0)
            {
                m_ui.displayMessage("Cet attribut est déjà à sa valeur minimale !");
                m_ui.waitForKeyPress();
                break;
            }

            int toRemove = m_ui.getIntInput("Combien de points retirer ? (1-" + std::to_string(maxToRemove) + ") : ", 1, maxToRemove);

            currentValues[attrIndex] -= toRemove;
            pointsDisponibles += toRemove;

            m_ui.displayMessage(attributNames[attrIndex] + " diminué de " + std::to_string(toRemove) + " points.");
            m_ui.waitForKeyPress();
        }
        break;
        case 3: // Réinitialiser
        {
            currentValues = baseValues;
            pointsDisponibles = POINTS_TOTAUX;
            m_ui.displayMessage("Valeurs réinitialisées aux valeurs de base.");
            m_ui.waitForKeyPress();
        }
        break;
        case 4: // Terminer
        {
            editing = false;
        }
        break;
        }
    }

    // Mettre à jour les attributs du personnage
    for (int i = 0; i < 8; ++i)
    {
        m_character->setParam(i, currentValues[i]);
    }

    m_ui.displayMessage("Points de caractéristiques attribués avec succès !");
    m_ui.waitForKeyPress();
}