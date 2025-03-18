# Uncharted Waters: Nouveau Horizons

Un jeu en mode console inspiré d'Uncharted Waters et développé en C++.

## Description

Ce jeu est une simulation de commerce maritime et d'exploration se déroulant à l'ère des Grandes Découvertes. En tant que capitaine de navire, vous devrez naviguer sur les mers du monde, commercer entre différentes villes, améliorer votre navire et éventuellement constituer une flotte pour dominer les mers.

## Fonctionnalités

- Navigation sur une carte du monde
- Commerce entre différentes villes avec fluctuation des prix
- Gestion d'inventaire et de cargaison
- Système de progression du personnage
- Interface en mode console

## Installation

### Prérequis

- CMake 3.10 ou supérieur
- Un compilateur C++ supportant C++17 (GCC, Clang, MSVC)

### Compilation

```bash
# Cloner le dépôt
git clone https://github.com/yourusername/orenji-cpp.git
cd orenji-cpp

# Créer un dossier de build
mkdir build
cd build

# Générer les fichiers de build
cmake ..

# Compiler
cmake --build .
```

Le jeu exécutable sera généré dans le dossier `bin`.

## Comment jouer

1. Lancez l'exécutable `unchartedwaters`
2. Créez un nouveau personnage
3. Naviguez sur la carte en utilisant les commandes de direction
4. Visitez les villes pour commercer, améliorer votre navire et recruter un équipage
5. Achetez des marchandises à bas prix et revendez-les plus cher dans d'autres villes

### Commandes principales

Le jeu est entièrement navigable via les menus. Utilisez les chiffres pour sélectionner les options de menu.

- Menu principal: Navigation, inventaire, statut...
- Navigation: Déplacement sur la carte (Nord, Sud, Est, Ouest)
- Villes: Commerce, chantier naval, port, taverne...

## Structure du projet

- `src/` - Code source
  - `models/` - Classes de données (Ship, TradeGood, Player...)
  - `game/` - Logique du jeu
  - `utils/` - Utilitaires (chargement JSON...)
- `data/` - Données du jeu au format JSON

## Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.

## Crédits

Inspiré par le jeu "Uncharted Waters: New Horizons" développé par KOEI. 