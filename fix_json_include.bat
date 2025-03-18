@echo off
echo Compilation avec options avancées pour nlohmann/json.hpp
echo ===================================================

:: Ajouter le chemin du compilateur
set MINGW_PATH=c:\msys64\ucrt64
set PATH=%MINGW_PATH%\bin;%PATH%

:: Obtenir le chemin absolu du projet
set PROJECT_PATH=%CD%

:: Vérifier l'existence du fichier json.hpp
if not exist "%PROJECT_PATH%\include\nlohmann\json.hpp" (
    echo ERREUR: Le fichier %PROJECT_PATH%\include\nlohmann\json.hpp est introuvable!
    echo Vérifiez que le dossier include et ses sous-dossiers sont présents.
    pause
    exit /b 1
)

:: Vérifier que le compilateur est disponible
g++ --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Impossible de trouver g++. Vérifiez que MinGW est correctement installé.
    pause
    exit /b 1
)

:: Création des dossiers nécessaires
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj
if not exist "obj\utils" mkdir obj\utils
if not exist "obj\models" mkdir obj\models
if not exist "obj\game" mkdir obj\game

:: Afficher le contenu du fichier problématique
echo Contenu de src\models\Ship.h :
type src\models\Ship.h

:: Compiler avec des options explicites et en mode verbose
echo.
echo Compilation en cours avec options étendues...
echo.

g++ -std=c++17 -v -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 ^
    -I"%PROJECT_PATH%" ^
    -I"%PROJECT_PATH%\include" ^
    src/main.cpp ^
    src/utils/JsonLoader.cpp ^
    src/utils/GameConfig.cpp ^
    src/utils/DataSystem.cpp ^
    src/utils/DataManager.cpp ^
    src/models/Player.cpp ^
    src/game/World.cpp ^
    src/game/LanguageManager.cpp ^
    src/game/GameData.cpp ^
    src/game/Game.cpp ^
    -lstdc++fs -o bin/UnchartedWaters.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Échec de la compilation. Tentative avec une approche alternative...
    echo.
    
    :: Créer un fichier temporaire avec un include correct
    echo #pragma once > temp_ship.h
    echo. >> temp_ship.h
    echo #include ^<string^> >> temp_ship.h
    echo #include ^<vector^> >> temp_ship.h
    echo #include "%PROJECT_PATH:\=/%/include/nlohmann/json.hpp" >> temp_ship.h
    echo. >> temp_ship.h
    type src\models\Ship.h | findstr /v "pragma once" | findstr /v "include" >> temp_ship.h
    
    :: Remplacer temporairement le fichier original
    move /y temp_ship.h src\models\Ship.h
    
    :: Essayer de compiler à nouveau
    echo Tentative de compilation avec chemin absolu...
    g++ -std=c++17 -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" ^
        -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 ^
        src/main.cpp ^
        src/utils/JsonLoader.cpp ^
        src/utils/GameConfig.cpp ^
        src/utils/DataSystem.cpp ^
        src/utils/DataManager.cpp ^
        src/models/Player.cpp ^
        src/game/World.cpp ^
        src/game/LanguageManager.cpp ^
        src/game/GameData.cpp ^
        src/game/Game.cpp ^
        -lstdc++fs -o bin/UnchartedWaters.exe
        
    if %ERRORLEVEL% NEQ 0 (
        echo.
        echo La compilation a échoué même avec l'approche alternative.
        echo Veuillez vérifier manuellement les chemins d'inclusion et la structure du projet.
    ) else (
        echo.
        echo Compilation réussie avec l'approche alternative!
        echo L'exécutable se trouve dans bin/UnchartedWaters.exe
        echo.
        echo NOTE: Le fichier Ship.h a été modifié avec un chemin absolu.
        echo Pour restaurer l'include original, éditez src/models/Ship.h et remplacez
        echo #include "%PROJECT_PATH:\=/%/include/nlohmann/json.hpp" par #include ^<nlohmann/json.hpp^>
        
        :: Copie des fichiers de données
        if exist "data" (
            if not exist "bin\data" mkdir bin\data
            xcopy /E /Y /I "data" "bin\data" > nul
            echo Données copiées dans bin/data/
        )
    )
) else (
    echo.
    echo Compilation réussie!
    echo L'exécutable se trouve dans bin/UnchartedWaters.exe
    
    :: Copie des fichiers de données
    if exist "data" (
        if not exist "bin\data" mkdir bin\data
        xcopy /E /Y /I "data" "bin\data" > nul
        echo Données copiées dans bin/data/
    )
)

pause 