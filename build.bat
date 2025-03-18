@echo off
echo Compilation du projet
echo ====================

:: Définir le chemin de MinGW
set MINGW_PATH=c:\msys64\ucrt64
set PATH=%MINGW_PATH%\bin;%PATH%

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

:: Créer les répertoires nécessaires
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj
if not exist "obj\utils" mkdir obj\utils
if not exist "obj\models" mkdir obj\models
if not exist "obj\game" mkdir obj\game
if not exist "bin\data" mkdir bin\data

:: Compiler les fichiers sources avec des chemins d'inclusion explicites
echo Compilation en cours...

:: Définir le chemin absolu du projet
set PROJECT_PATH=%CD%

:: Compiler avec des chemins d'inclusion explicites
g++ -std=c++17 -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 ^
    -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" ^
    src/main.cpp ^
    src/utils/JsonLoader.cpp ^
    src/utils/GameConfig.cpp ^
    src/utils/DataSystem.cpp ^
    src/utils/DataManager.cpp ^
    src/models/Player.cpp ^
    src/game/Game.cpp ^
    src/game/World.cpp ^
    src/game/LanguageManager.cpp ^
    src/game/GameData.cpp ^
    src/game/CharacterEditor.cpp ^
    src/game/ConsoleUI.cpp ^
    -o bin/UnchartedWaters.exe ^
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

echo L'exécutable se trouve dans bin\UnchartedWaters.exe
dir bin
echo.
pause 