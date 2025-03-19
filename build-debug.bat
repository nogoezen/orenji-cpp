@echo off
echo Compilation Debug
echo ================

:: Définir le chemin de MinGW
set MINGW_PATH=c:\msys64\ucrt64
set PATH=%MINGW_PATH%\bin;%PATH%

:: Créer les répertoires nécessaires
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj
if not exist "obj\utils" mkdir obj\utils
if not exist "obj\models" mkdir obj\models
if not exist "obj\game" mkdir obj\game

:: Définir le chemin absolu du projet
set PROJECT_PATH=%CD%

set COMPILER_OPTIONS=-std=c++17 -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1
set INCLUDE_PATHS=-I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include"

echo Compilation des fichiers individuels...

:: Compiler chaque fichier séparément
echo Compilation de main.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/main.cpp -o obj/main.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de main.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de JsonLoader.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/utils/JsonLoader.cpp -o obj/utils/JsonLoader.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de JsonLoader.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de GameConfig.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/utils/GameConfig.cpp -o obj/utils/GameConfig.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de GameConfig.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de DataSystem.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/utils/DataSystem.cpp -o obj/utils/DataSystem.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de DataSystem.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de DataManager.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/utils/DataManager.cpp -o obj/utils/DataManager.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de DataManager.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de Player.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/models/Player.cpp -o obj/models/Player.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de Player.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de Game.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/game/Game.cpp -o obj/game/Game.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de Game.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de World.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/game/World.cpp -o obj/game/World.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de World.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de LanguageManager.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/game/LanguageManager.cpp -o obj/game/LanguageManager.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de LanguageManager.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de GameData.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/game/GameData.cpp -o obj/game/GameData.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de GameData.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de CharacterEditor.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/game/CharacterEditor.cpp -o obj/game/CharacterEditor.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de CharacterEditor.cpp a échoué.
    pause
    exit /b 1
)

echo Compilation de ConsoleUI.cpp...
g++ %COMPILER_OPTIONS% -c %INCLUDE_PATHS% src/game/ConsoleUI.cpp -o obj/game/ConsoleUI.o
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Compilation de ConsoleUI.cpp a échoué.
    pause
    exit /b 1
)

echo Édition de liens...
g++ -o bin/UnchartedWaters.exe ^
    obj/main.o ^
    obj/utils/JsonLoader.o ^
    obj/utils/GameConfig.o ^
    obj/utils/DataSystem.o ^
    obj/utils/DataManager.o ^
    obj/models/Player.o ^
    obj/game/Game.o ^
    obj/game/World.o ^
    obj/game/LanguageManager.o ^
    obj/game/GameData.o ^
    obj/game/CharacterEditor.o ^
    obj/game/ConsoleUI.o ^
    -lstdc++fs

:: Vérifier si la compilation a réussi
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: L'édition de liens a échoué.
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
dir bin\UnchartedWaters.exe
echo.
pause 