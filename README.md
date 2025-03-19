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