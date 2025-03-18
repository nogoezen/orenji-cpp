# Guide d'utilisation du projet UnchartedWaters avec Cursor IDE

Ce guide explique comment configurer, compiler et exécuter le projet UnchartedWaters dans Cursor IDE.

## Prérequis

- [Cursor IDE](https://cursor.sh/) installé
- [MSYS2/UCRT64](https://www.msys2.org/) avec GCC 8.1.0 ou supérieur
- Support de C++17 avec `std::filesystem`

## Configuration initiale

Deux options s'offrent à vous pour configurer ce projet :

### Option 1 : Configuration automatique (recommandée)

1. Exécutez le script `cursor_setup.bat` pour générer les fichiers de configuration
2. Ce script va créer les fichiers `.vscode/` nécessaires avec les bons chemins

### Option 2 : Configuration manuelle

1. Ouvrez le dossier du projet dans Cursor
2. Créez un dossier `.vscode` avec les fichiers suivants :
   - `c_cpp_properties.json` : Définit les chemins d'inclusion
   - `tasks.json` : Définit la tâche de compilation
   - `launch.json` : Configure le débogage
   - `settings.json` : Paramètres supplémentaires

## Compilation et exécution

### Compiler le projet

Trois méthodes sont disponibles pour compiler :

1. **Avec le script de compilation :**
   - Exécutez `compiler_fix.bat` depuis un terminal
   - Ce script compile directement avec G++ en spécifiant les bons chemins d'inclusion

2. **Depuis Cursor :**
   - Appuyez sur `Ctrl+Shift+B` 
   - Ou allez dans le menu Terminal -> Run Build Task

3. **Par ligne de commande :**
   - Ouvrez un terminal dans Cursor (Ctrl+`)
   - Exécutez : `g++ -std=c++17 -I. -I./include [... fichiers sources ...] -lstdc++fs -o bin/UnchartedWaters.exe`

### Exécuter le programme

1. **Depuis Cursor :**
   - Appuyez sur `F5` pour exécuter en mode débogage
   - Ou utilisez le menu "Run" -> "Start Debugging"

2. **Par ligne de commande :**
   - Exécutez : `.\bin\UnchartedWaters.exe`

## Résolution des problèmes

### Problème : "nlohmann/json.hpp: No such file or directory"

Ce problème est causé par des chemins d'inclusion incorrects.

**Solution :**
- Vérifiez que le fichier `include/nlohmann/json.hpp` existe
- Assurez-vous d'utiliser le flag `-I./include` lors de la compilation
- Utilisez le script `compiler_fix.bat` qui inclut tous les chemins nécessaires

### Problème : Erreurs liées à std::filesystem

Si vous rencontrez des erreurs avec `std::filesystem` :

**Solution :**
- Vérifiez que vous utilisez C++17 avec le flag `-std=c++17`
- Assurez-vous que `FileUtils.h` contient les opérateurs de comparaison pour MinGW
- Utilisez `-lstdc++fs` lors de la compilation pour lier la bibliothèque filesystem

## Structure du projet

- `src/` : Fichiers source
  - `src/utils/` : Utilitaires (FileUtils, JsonLoader, etc.)
  - `src/models/` : Modèles (Player, Ship, etc.)
  - `src/game/` : Logique du jeu (Game, World, etc.)
- `include/` : Bibliothèques externes (nlohmann/json)
- `data/` : Données du jeu
- `bin/` : Exécutables compilés
- `.vscode/` : Configuration de Cursor/VS Code

## Conseils pour Cursor

- Utilisez l'AI Assistant de Cursor pour vous aider avec le code
- Les raccourcis clavier sont les mêmes que VS Code
- Vous pouvez personnaliser davantage les paramètres dans `.vscode/settings.json` 