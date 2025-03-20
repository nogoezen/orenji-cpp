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

echo %vert%Compilation du projet%reset%
echo ====================

:: Préparation des ressources
echo %bleu%Préparation des répertoires de ressources...%reset%
if not exist "%BIN_DIR%\resources" mkdir "%BIN_DIR%\resources"
if not exist "%BIN_DIR%\resources\textures" mkdir "%BIN_DIR%\resources\textures"
if not exist "%BIN_DIR%\resources\sounds" mkdir "%BIN_DIR%\resources\sounds"
if not exist "%BIN_DIR%\resources\fonts" mkdir "%BIN_DIR%\resources\fonts"

:: Lister les fichiers source
set "SOURCES="
for /R "%SRC_DIR%" %%F in (*.cpp) do (
    set "SOURCES=!SOURCES! "%%F""
)

:: Variables de compilation
set "COMPILER=g++"
set "STD_VERSION=-std=c++17"
set "OPTIMIZATION=-O2"
set "WARNINGS=-Wall -Wextra -Wno-unused-parameter"
set "DEBUG=-g"
set "INCLUDES=-I"%INCLUDE_DIR%" -I"%WORKING_DIR%\lib\sfml\include" -I"%WORKING_DIR%\lib\tmxlite\include" -I"%WORKING_DIR%\lib\box2d\include" -I"%WORKING_DIR%\lib\thor\include" -IC:\msys64\ucrt64\include -IC:\msys64\ucrt64\include\Thor"
set "LIBS=-L"%WORKING_DIR%\lib\sfml\lib" -L"%WORKING_DIR%\lib\tmxlite\lib" -L"%WORKING_DIR%\lib\box2d\lib" -L"%WORKING_DIR%\lib\thor\lib" -LC:\msys64\ucrt64\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -ltmxlite-s -lbox2d -lthor"

:: Commande de compilation
set "COMPILE_CMD=%COMPILER% %STD_VERSION% %OPTIMIZATION% %WARNINGS% %DEBUG% %INCLUDES% -o %BIN_DIR%\OrenjicGame.exe main.cpp %SOURCES% %LIBS%"

:: Exécution de la compilation
echo %bleu%Compilation de tous les fichiers...%reset%
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

:: Copier les cartes Tiled
if exist "%RESOURCES_DIR%\maps" (
    echo Copie des cartes Tiled...
    xcopy /E /Y /I "%RESOURCES_DIR%\maps" "%BIN_DIR%\resources\maps"
)

:: Copie des DLLs
echo Copie des DLLs SFML...
copy "%WORKING_DIR%\lib\sfml\bin\sfml-graphics-2.dll" "%BIN_DIR%\"
copy "%WORKING_DIR%\lib\sfml\bin\sfml-window-2.dll" "%BIN_DIR%\"
copy "%WORKING_DIR%\lib\sfml\bin\sfml-system-2.dll" "%BIN_DIR%\"
copy "%WORKING_DIR%\lib\sfml\bin\sfml-audio-2.dll" "%BIN_DIR%\"

echo Copie de la DLL TMXLite...
copy "%WORKING_DIR%\lib\tmxlite\bin\tmxlite.dll" "%BIN_DIR%\"

echo Copie de la DLL Box2D...
copy "%WORKING_DIR%\lib\box2d\bin\box2d.dll" "%BIN_DIR%\"

echo Copie de la DLL Thor...
copy "%WORKING_DIR%\lib\thor\bin\thor.dll" "%BIN_DIR%\"

echo %vert%Exécutable créé dans %BIN_DIR%\OrenjicGame.exe%reset%

:: Compilation des exemples
if exist "%EXAMPLES_DIR%" (
    echo.
    echo %jaune%Construction de l'exemple de particules...%reset%
    
    if not exist "%BIN_DIR%\example" mkdir "%BIN_DIR%\example"
    if not exist "%BIN_DIR%\example\resources" mkdir "%BIN_DIR%\example\resources"
    
    :: Commande de compilation des exemples
    set "EXAMPLE_CMD=%COMPILER% %STD_VERSION% %OPTIMIZATION% %WARNINGS% %DEBUG% %INCLUDES% -o %BIN_DIR%\example\ParticleExample.exe %EXAMPLES_DIR%\ParticleExample.cpp %SOURCES% %LIBS%"
    
    :: Exécution de la compilation des exemples
    call %EXAMPLE_CMD%
    
    :: Vérification du résultat
    if %ERRORLEVEL% NEQ 0 (
        echo %rouge%AVERTISSEMENT: La compilation de l'exemple de particules a échoué.%reset%
        pause
    ) else (
        echo %vert%Exemple de particules créé dans %BIN_DIR%\example\ParticleExample.exe%reset%
        
        :: Copie des ressources pour les exemples si elles existent
        if exist "%RESOURCES_DIR%\particles" (
            echo Copie des assets pour l'exemple...
            xcopy /E /Y /I "%RESOURCES_DIR%\particles" "%BIN_DIR%\example\resources\particles"
        )
    )
)

:fin
echo %vert%Terminé.%reset%
endlocal 