# Uncharted Waters: New Horizons

Un jeu de simulation maritime et commerciale inspiré de la série Uncharted Waters, développé en C++ avec SFML.

## Fonctionnalités

### Système de Navigation
- Navigation en temps réel avec SFML
- Système de flotte avec gestion des navires
- Système de voyage avec calcul de distance et de coût
- Gestion des provisions et de l'équipage

### Système Commercial
- Marchés dynamiques avec prix fluctuants
- Système de cargaison et de stockage
- Gestion des biens et des ressources
- Système de commerce entre villes et royaumes

### Système de Combat
- Combat naval en temps réel
- Gestion des dégâts et des réparations
- Système d'équipement et d'armement
- Tactiques de combat et formations

### Système de Diplomatie
- Relations entre royaumes
- Système d'alliances et de guerres
- Gestion des traités commerciaux
- Système de réputation et d'influence

### Système d'IA
- **DiplomaticAI** : Gestion des relations diplomatiques entre royaumes
  - Évaluation des actions diplomatiques
  - Calcul des relations et des influences
  - Gestion des alliances et des conflits
  - Système de cache pour les performances

- **CityAI** : Gestion du développement des villes
  - Gestion des ressources et de la production
  - Développement des infrastructures
  - Politiques commerciales
  - Gestion de la population

- **EventAI** : Gestion des événements aléatoires
  - Catastrophes naturelles
  - Crises politiques
  - Événements économiques
  - Découvertes et explorations

- **NPCAI** : Gestion des personnages non-joueurs
  - Comportements autonomes
  - Système de prise de décision
  - Gestion des relations entre NPCs
  - Actions spécifiques (voyage, commerce, combat, etc.)

### Système de Personnages
- Création et personnalisation des personnages
- Système de compétences et d'expérience
- Gestion des relations entre personnages
- Système de réputation et d'influence

### Système de Quêtes
- Quêtes principales et secondaires
- Système de récompenses
- Événements aléatoires
- Découvertes et explorations

## Structure du Projet

```
src/
├── core/           # Classes de base et utilitaires
├── game/           # Logique principale du jeu
├── ui/             # Interface utilisateur
├── world/          # Gestion du monde de jeu
├── ai/             # Systèmes d'intelligence artificielle
│   ├── DiplomaticAI.h/cpp
│   ├── CityAI.h/cpp
│   ├── EventAI.h/cpp
│   └── NPCAI.h/cpp
└── main.cpp        # Point d'entrée
```

## Prérequis

- C++17 ou supérieur
- SFML 2.6.0 ou supérieur
- CMake 3.15 ou supérieur
- Compilateur compatible C++17 (MSVC, GCC, Clang)

## Installation

1. Cloner le dépôt :
```bash
git clone https://github.com/votre-username/orenji-cpp.git
cd orenji-cpp
```

2. Créer un dossier de build :
```bash
mkdir build
cd build
```

3. Configurer avec CMake :
```bash
cmake ..
```

4. Compiler :
```bash
cmake --build .
```

## Utilisation

Lancer le jeu :
```bash
./orenji-cpp
```

## Contribution

Les contributions sont les bienvenues ! N'hésitez pas à :
1. Fork le projet
2. Créer une branche pour votre fonctionnalité
3. Commiter vos changements
4. Pousser vers la branche
5. Ouvrir une Pull Request

## Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

## Crédits

- Inspiré de la série Uncharted Waters de Koei
- Développé avec SFML (Simple and Fast Multimedia Library)
- Créé par [Votre Nom]

# Uncharted Waters - Clone

Un jeu de commerce maritime inspiré par la série Uncharted Waters (Koei).

## Description

Uncharted Waters est un jeu de rôle et de simulation commerciale se déroulant durant l'Age des Découvertes. Le joueur incarne un capitaine de navire qui explore le monde, commerce entre différents ports, et constitue sa flotte.

Caractéristiques principales:
- Système de commerce dynamique avec variation des prix
- Navigation sur une carte du monde
- Gestion de flotte et amélioration des navires
- Système de cargo et inventaire
- Personnalisation du personnage

## Prérequis

- Un compilateur C++ supportant C++17 (GCC, MinGW, MSVC, etc.)
- CMake 3.10 ou supérieur
- [nlohmann/json](https://github.com/nlohmann/json) (inclus dans le dépôt)

## Installation

### Windows (avec MinGW)

1. Clonez le dépôt:
   ```bash
   git clone https://github.com/votre-user/orenji-cpp.git
   cd orenji-cpp
   ```

2. Utilisez le script de compilation:
   ```bash
   build.bat
   ```

3. L'exécutable sera généré dans le dossier `bin/`.

### Linux

1. Clonez le dépôt:
   ```bash
   git clone https://github.com/votre-user/orenji-cpp.git
   cd orenji-cpp
   ```

2. Créez et accédez au dossier de build:
   ```bash
   mkdir build
   cd build
   ```

3. Configurez et compilez avec CMake:
   ```bash
   cmake ..
   make
   ```

4. L'exécutable sera généré dans le dossier `bin/`.

## Utilisation

Lancez l'exécutable dans le dossier `bin/`:

```bash
# Windows
.\bin\UnchartedWaters.exe

# Linux
./bin/UnchartedWaters
```

## Structure du Projet

Pour une description détaillée de la structure du projet, consultez le fichier [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md).

## Fonctionnalités

### Système de Commerce
- Achat et vente de marchandises dans différents ports
- Prix dynamiques influencés par l'offre, la demande et les événements
- Spécialités régionales affectant les prix
- Interface utilisateur dédiée au commerce
- Routes commerciales avec estimation des profits
- Relations avec les différents royaumes et bonus commerciaux

### Gestion de Cargo
- Système d'inventaire pour stocker les marchandises
- Gestion du poids et de la capacité des navires
- Visualisation détaillée du contenu de la cargaison

### Navigation
- Déplacement entre différentes villes du monde
- Gestion du temps de voyage
- Découverte de nouvelles régions

### Gestion de Flotte
- Achat, vente et réparation de navires
- Recrutement d'équipage
- Amélioration des navires
- Navire amiral avec capacités spéciales

### Personnalisation
- Création de personnage avec attributs personnalisables
- Progression de niveau et d'expérience
- Acquisition de compétences

## Configuration

Le jeu utilise un fichier de configuration (`data/game_config.json`) qui permet d'ajuster:
- Les paramètres économiques
- Les réglages du joueur
- Les paramètres de voyage
- Divers aspects de la difficulté du jeu

## Licence

Ce projet est distribué sous la licence MIT. Voir le fichier `LICENSE` pour plus d'informations.

## Remerciements

- Koei pour la série originale Uncharted Waters qui a inspiré ce projet
- [nlohmann/json](https://github.com/nlohmann/json) pour la bibliothèque JSON

# Orenji-CPP - Optimisation du projet

## Problèmes identifiés et solutions

Après analyse du code, voici les problèmes d'optimisation identifiés et les solutions proposées:

### 1. Duplication de classes et de fonctionnalités

**Problème**: Il existe plusieurs classes ayant des fonctionnalités similaires dans différents dossiers:
- `src/utils/GameData.h` vs `src/game/GameData.h`
- Duplication de logique de chargement de données dans plusieurs classes

**Solution**:
- Consolidation des classes GameData en une seule implémentation
- Adoption d'une structure hiérarchique claire pour éviter les redondances

### 2. Gestion des données inefficace

**Problème**:
- Chargement multiple des mêmes données
- Utilisation de conteneurs sous-optimaux (vector pour la recherche par ID)
- Stockage redondant des données JSON

**Solution**:
- Utiliser des `unordered_map` pour une recherche O(1) par ID
- Centraliser le chargement des données dans DataManager
- Implémenter un système de cache efficace pour éviter les recharges inutiles

### 3. Architecture confuse

**Problème**:
- Séparation peu claire entre les modèles, le gameplay et les utilitaires
- Trop de singletons interdépendants créant un couplage fort

**Solution**:
- Revoir l'architecture en adoptant une approche plus modulaire
- Utiliser l'injection de dépendances plutôt que des singletons partout
- Séparer clairement la logique métier, l'interface utilisateur et les données

### 4. Structure de fichiers désorganisée

**Problème**:
- Fichiers avec des responsabilités trop larges
- Organisation des dossiers peu cohérente

**Solution**:
- Restructurer les dossiers selon leur responsabilité:
  - `core/`: Fonctionnalités centrales du jeu
  - `ui/`: Interface utilisateur
  - `data/`: Gestion des données
  - `models/`: Modèles de données
  - `utils/`: Utilitaires génériques

## Nouvelle structure proposée

```
src/
├── core/             # Logique centrale du jeu
│   ├── Game.h/cpp    # Classe principale du jeu
│   ├── World.h/cpp   # Gestion du monde de jeu
│   └── ...
├── data/             # Gestion des données
│   ├── DataManager.h/cpp
│   ├── GameData.h/cpp
│   └── ...
├── models/           # Modèles de données
│   ├── Player.h/cpp
│   ├── Ship.h/cpp
│   └── ...
├── ui/               # Interface utilisateur
│   ├── MainMenu.h/cpp
│   ├── ConsoleUI.h/cpp
│   └── ...
└── utils/            # Utilitaires génériques
    ├── JsonLoader.h/cpp
    ├── FileUtils.h/cpp
    └── ...
```

## Plan d'implémentation

1. Consolider les classes dupliquées
2. Optimiser la gestion des données avec des structures appropriées
3. Réorganiser les fichiers selon la nouvelle structure
4. Mettre à jour les dépendances et inclusions
5. Refactoriser le système de singleton pour réduire le couplage
6. Ajouter une documentation complète sur l'architecture

## Améliorations de performance

1. Utiliser des références aux données plutôt que des copies
2. Implémenter un système de cache intelligent
3. Optimiser les recherches avec des structures de données appropriées
4. Réduire les allocations mémoire inutiles
5. Utiliser des algorithmes plus efficaces pour les opérations courantes

## Instructions de construction

Le projet continuera d'utiliser CMake pour la génération des fichiers de construction. Les scripts batch existants seront mis à jour pour refléter la nouvelle structure.

```bash
# Génération avec CMake
cmake -B build -S .

# Construction
cmake --build build

# Exécution
./build/bin/UnchartedWaters
``` 