@echo off
echo Compilation du projet
echo ====================

:: Définir le chemin de MinGW
set MINGW_PATH=c:\msys64\ucrt64
set PATH=%MINGW_PATH%\bin;%PATH%

:: Définir les chemins pour SFML
set SFML_PATH=c:\msys64\ucrt64
set SFML_INCLUDE=%SFML_PATH%\include
set SFML_LIB=%SFML_PATH%\lib

:: Vérifier l'existence du fichier json.hpp
if not exist "include\nlohmann\json.hpp" (
    echo ERREUR: Le fichier include\nlohmann\json.hpp est introuvable!
    echo Vérifiez que le répertoire include\nlohmann existe et contient json.hpp.
    pause
    exit /b 1
)

:: Vérifier si le compilateur est disponible
g++ --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Le compilateur g++ n'est pas disponible.
    echo Vérifiez que MinGW est correctement installé.
    pause
    exit /b 1
)

:: Vérifier si SFML est disponible
if not exist "%SFML_INCLUDE%\SFML\Graphics.hpp" (
    echo AVERTISSEMENT: La bibliothèque SFML n'est pas trouvée.
    echo Pour utiliser l'interface graphique, veuillez installer SFML.
    echo Compilation en mode console uniquement...
    set USE_CONSOLE=1
)

:: Créer les répertoires nécessaires
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj
if not exist "obj\utils" mkdir obj\utils
if not exist "obj\models" mkdir obj\models
if not exist "obj\core" mkdir obj\core
if not exist "obj\data" mkdir obj\data
if not exist "obj\ui" mkdir obj\ui
if not exist "obj\game" mkdir obj\game
if not exist "bin\data" mkdir bin\data

:: Compiler les fichiers sources avec des chemins d'inclusion explicites
echo Compilation en cours...

:: Définir le chemin absolu du projet
set PROJECT_PATH=%CD%

:: Afficher les répertoires pour le débogage
echo Répertoire du projet: %PROJECT_PATH%
echo Vérification des fichiers:
dir src\main.cpp
echo.

:: Compiler les fichiers individuellement pour identifier les erreurs
echo Compilation des fichiers individuels...
g++ -std=c++17 -Wall -Wextra -c -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" -I"%SFML_INCLUDE%" src/main.cpp -o obj/main.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de main.cpp a échoué.
    pause
    exit /b 1
)

g++ -std=c++17 -Wall -Wextra -c -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" -I"%SFML_INCLUDE%" src/utils/JsonLoader.cpp -o obj/utils/JsonLoader.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de JsonLoader.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de tous les fichiers...
g++ -std=c++17 -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 ^
    -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" -I"%SFML_INCLUDE%" ^
    src/main.cpp ^
    src/utils/JsonLoader.cpp ^
    src/utils/FileUtils.cpp ^
    src/models/Player.cpp ^
    src/models/Character.cpp ^
    src/models/Fleet.cpp ^
    src/models/Ship.cpp ^
    src/models/TradeGood.cpp ^
    src/core/Game.cpp ^
    src/core/World.cpp ^
    src/core/TradingSystem.cpp ^
    src/data/GameData.cpp ^
    src/ui/GuiUI.cpp ^
    src/game/MapScene.cpp ^
    -o bin/UnchartedWaters_new.exe ^
    -L"%SFML_LIB%" -lsfml-graphics -lsfml-window -lsfml-system ^
    -lstdc++fs

:: Vérifier si la compilation a réussi
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: La compilation a échoué.
    pause
    exit /b 1
) else (
    echo Compilation réussie!
)

:: Copier les fichiers de données si nécessaire
if exist "data" (
    echo Copie des fichiers de données...
    xcopy /E /Y /I "data" "bin\data"
)

:: Copier les ressources graphiques si nécessaires
if exist "assets" (
    echo Copie des assets graphiques...
    xcopy /E /Y /I "assets\font" "bin\assets\font"
    xcopy /E /Y /I "assets\img" "bin\assets\img"
    
    :: Créer les répertoires pour les maps s'ils n'existent pas
    if not exist "bin\assets\maps" mkdir bin\assets\maps
    xcopy /E /Y /I "assets\maps" "bin\assets\maps"
)

:: Copier les DLLs de SFML nécessaires
if not "%USE_CONSOLE%"=="1" (
    echo Copie des DLLs SFML...
    copy "%SFML_PATH%\bin\libsfml-graphics-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-window-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-system-2.dll" "bin\"
)

echo L'exécutable se trouve dans bin\UnchartedWaters_new.exe
dir bin
echo.
pause 