@echo off
echo Compilation du projet
echo ====================

:: Définir le chemin de MinGW
set MINGW_PATH=c:\msys64\ucrt64
set PATH=%MINGW_PATH%\bin;%PATH%

:: Définir les chemins pour SFML, TMXLite et Box2D
set SFML_PATH=c:\msys64\ucrt64
set SFML_INCLUDE=%SFML_PATH%\include
set SFML_LIB=%SFML_PATH%\lib
set TMXLITE_INCLUDE=%MINGW_PATH%\include
set TMXLITE_LIB=%MINGW_PATH%\lib
set BOX2D_INCLUDE=%MINGW_PATH%\include
set BOX2D_LIB=%MINGW_PATH%\lib

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

:: Vérifier si TMXLite est disponible
if not exist "%TMXLITE_INCLUDE%\tmxlite\Map.hpp" (
    echo AVERTISSEMENT: La bibliothèque TMXLite n'est pas trouvée.
    echo Pour utiliser le module TiledMap, veuillez installer TMXLite.
    echo Compilation sans le support des cartes Tiled...
    set NO_TMXLITE=1
)

:: Vérifier si Box2D est disponible
if not exist "%BOX2D_INCLUDE%\box2d\box2d.h" (
    echo AVERTISSEMENT: La bibliothèque Box2D n'est pas trouvée.
    echo Pour utiliser le module Physics, veuillez installer Box2D.
    echo Compilation sans le support de la physique...
    set NO_BOX2D=1
)

:: Créer les répertoires nécessaires
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj
if not exist "obj\Core" mkdir obj\Core
if not exist "obj\States" mkdir obj\States
if not exist "obj\Utilities" mkdir obj\Utilities
if not exist "obj\TiledMap" mkdir obj\TiledMap
if not exist "obj\Physics" mkdir obj\Physics

:: Définir le chemin absolu du projet
set PROJECT_PATH=%CD%

echo Préparation des répertoires de ressources...
if not exist "bin\resources" mkdir bin\resources
if not exist "bin\resources\fonts" mkdir bin\resources\fonts
if not exist "bin\resources\textures" mkdir bin\resources\textures
if not exist "bin\resources\sounds" mkdir bin\resources\sounds
if not exist "bin\resources\maps" mkdir bin\resources\maps

echo Compilation de tous les fichiers...
g++ -std=c++17 -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 ^
    -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" -I"%SFML_INCLUDE%" -I"%TMXLITE_INCLUDE%" -I"%BOX2D_INCLUDE%" ^
    main.cpp ^
    src\Core\Entity.cpp ^
    src\Core\Component.cpp ^
    src\Core\EntityManager.cpp ^
    src\Core\SystemManager.cpp ^
    src\Core\RenderSystem.cpp ^
    src\States\State.cpp ^
    src\States\StateMachine.cpp ^
    src\States\MainMenuState.cpp ^
    src\States\GameState.cpp ^
    src\Utilities\ResourceManager.cpp ^
    src\Utilities\InputHandler.cpp ^
    src\TiledMap\TiledMap.cpp ^
    src\TiledMap\TiledMapLoader.cpp ^
    src\Game.cpp ^
    -o bin\OrenjicGame.exe ^
    -L"%SFML_LIB%" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio ^
    -L"%TMXLITE_LIB%" -ltmxlite ^
    -lstdc++fs

:: Vérifier si la compilation a réussi
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: La compilation a échoué.
    pause
    exit /b 1
) else (
    echo Compilation réussie!
)

:: Copier les ressources
if exist "resources" (
    echo Copie des ressources...
    xcopy /E /Y /I "resources" "bin\resources"
)

:: Copier les assets de cartes
if exist "assets\maps" (
    echo Copie des cartes Tiled...
    xcopy /E /Y /I "assets\maps" "bin\resources\maps"
)

:: Copier les DLLs de SFML nécessaires
if not "%USE_CONSOLE%"=="1" (
    echo Copie des DLLs SFML...
    copy "%SFML_PATH%\bin\libsfml-graphics-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-window-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-system-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-audio-2.dll" "bin\"
)

:: Copier la DLL de TMXLite
if not "%NO_TMXLITE%"=="1" (
    echo Copie de la DLL TMXLite...
    copy "%MINGW_PATH%\bin\libtmxlite.dll" "bin\"
)

:: Copier la DLL de Box2D
echo Copie de la DLL Box2D...
copy "%MINGW_PATH%\bin\libbox2d.dll" "bin\"

echo Exécutable créé dans bin\OrenjicGame.exe
echo.
pause 