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