@echo off
setlocal enabledelayedexpansion

:: Couleurs des messages
set "rouge=[31m"
set "vert=[32m"
set "jaune=[33m"
set "bleu=[34m"
set "blanc=[37m"
set "reset=[0m"

:: Emplacement des répertoires
set "WORKING_DIR=%CD%"
set "BIN_DIR=%WORKING_DIR%\bin"
set "OBJ_DIR=%WORKING_DIR%\obj"
set "SRC_DIR=%WORKING_DIR%\src"
set "INCLUDE_DIR=%WORKING_DIR%\include"
set "EXAMPLES_DIR=%WORKING_DIR%\examples"
set "RESOURCES_DIR=%WORKING_DIR%\resources"
set "DATA_DIR=%WORKING_DIR%\resources\data"

:: Vérification des arguments
if "%1"=="clean" (
    echo %jaune%Nettoyage des fichiers de compilation...%reset%
    if exist "%BIN_DIR%" rmdir /S /Q "%BIN_DIR%"
    if exist "%OBJ_DIR%" rmdir /S /Q "%OBJ_DIR%"
    echo %vert%Nettoyage terminé.%reset%
    goto :fin
)

:: Création des répertoires si nécessaires
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"
if not exist "%DATA_DIR%" mkdir "%DATA_DIR%"

echo %vert%Compilation du projet%reset%
echo ====================

:: Préparation des ressources
echo %bleu%Préparation des répertoires de ressources...%reset%
if not exist "%BIN_DIR%\resources" mkdir "%BIN_DIR%\resources"
if not exist "%BIN_DIR%\resources\textures" mkdir "%BIN_DIR%\resources\textures"
if not exist "%BIN_DIR%\resources\sounds" mkdir "%BIN_DIR%\resources\sounds"
if not exist "%BIN_DIR%\resources\fonts" mkdir "%BIN_DIR%\resources\fonts"
if not exist "%BIN_DIR%\resources\data" mkdir "%BIN_DIR%\resources\data"

:: Fichiers essentiels spécifiques
set "ESSENTIAL_FILES="
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Game.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\State.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\StateMachine.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\TitleState.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\GameState.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\Entity.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\EntityManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\RenderSystem.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\SystemManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\Component.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Utilities\ResourceManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Utilities\InputHandler.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\ui\GuiUI.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Physics\PhysicsWorld.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Physics\PhysicsComponent.cpp""

:: Variables de compilation
set "COMPILER=g++"
set "STD_VERSION=-std=c++17"
set "OPTIMIZATION=-O2"
set "WARNINGS=-Wall -Wextra -Wno-unused-parameter"
set "DEBUG=-g"
set "INCLUDES=-I"%INCLUDE_DIR%" -I"%WORKING_DIR%\lib\sfml\include" -I"%WORKING_DIR%\lib\box2d\include""
set "LIBS=-L"%WORKING_DIR%\lib\sfml\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -L"%WORKING_DIR%\lib\box2d\lib" -lbox2d"

:: Ajout des directives pour désactiver les fonctionnalités problématiques
set "DEFINES=-DDISABLE_THOR -DDISABLE_GAMESTATE"

:: Commande de compilation principale - Compiler uniquement main.cpp et les fichiers essentiels
set "COMPILE_CMD=%COMPILER% %STD_VERSION% %OPTIMIZATION% %WARNINGS% %DEBUG% %INCLUDES% %DEFINES% -o %BIN_DIR%\OrenjicGame.exe main.cpp %ESSENTIAL_FILES% %LIBS%"

:: Exécution de la compilation
echo %bleu%Compilation des fichiers essentiels...%reset%
call %COMPILE_CMD%

:: Vérification du résultat
if %ERRORLEVEL% NEQ 0 (
    echo %rouge%ERREUR: La compilation a échoué.%reset%
    pause
    goto :fin
)

:: Copie des ressources
if exist "%RESOURCES_DIR%\sounds" (
    echo Copie des sons...
    xcopy /E /Y /I "%RESOURCES_DIR%\sounds" "%BIN_DIR%\resources\sounds"
)

if exist "%RESOURCES_DIR%\textures" (
    echo Copie des textures...
    xcopy /E /Y /I "%RESOURCES_DIR%\textures" "%BIN_DIR%\resources\textures"
)

if exist "%RESOURCES_DIR%\fonts" (
    echo Copie des polices...
    xcopy /E /Y /I "%RESOURCES_DIR%\fonts" "%BIN_DIR%\resources\fonts"
)

:: Copie des DLLs
echo Copie des DLLs SFML...
copy "%WORKING_DIR%\lib\sfml\bin\sfml-graphics-2.dll" "%BIN_DIR%\"
copy "%WORKING_DIR%\lib\sfml\bin\sfml-window-2.dll" "%BIN_DIR%\"
copy "%WORKING_DIR%\lib\sfml\bin\sfml-system-2.dll" "%BIN_DIR%\"
copy "%WORKING_DIR%\lib\sfml\bin\sfml-audio-2.dll" "%BIN_DIR%\"

:: Copie des DLLs Box2D
echo Copie des DLLs Box2D...
if exist "%WORKING_DIR%\lib\box2d\bin\box2d.dll" (
    copy "%WORKING_DIR%\lib\box2d\bin\box2d.dll" "%BIN_DIR%\"
)

echo %vert%Exécutable créé dans %BIN_DIR%\OrenjicGame.exe%reset%

:: Désactivation de la compilation des exemples pour se concentrer sur l'écran titre
echo %jaune%Compilation des exemples désactivée pour cette version%reset%

:fin
echo %vert%Terminé.%reset%
endlocal 