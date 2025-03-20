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
if not exist "obj\Core" mkdir obj\Core
if not exist "obj\States" mkdir obj\States
if not exist "obj\Utilities" mkdir obj\Utilities

:: Définir le chemin absolu du projet
set PROJECT_PATH=%CD%

echo Préparation des répertoires de ressources...
if not exist "bin\resources" mkdir bin\resources
if not exist "bin\resources\fonts" mkdir bin\resources\fonts
if not exist "bin\resources\textures" mkdir bin\resources\textures
if not exist "bin\resources\sounds" mkdir bin\resources\sounds

echo Compilation de tous les fichiers...
g++ -std=c++17 -Wall -Wextra -D__USE_MINGW_ANSI_STDIO=1 -D_GLIBCXX_USE_CXX11_ABI=1 ^
    -I"%PROJECT_PATH%" -I"%PROJECT_PATH%\include" -I"%SFML_INCLUDE%" ^
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
    src\Game.cpp ^
    -o bin\OrenjicGame.exe ^
    -L"%SFML_LIB%" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio ^
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

:: Copier les DLLs de SFML nécessaires
if not "%USE_CONSOLE%"=="1" (
    echo Copie des DLLs SFML...
    copy "%SFML_PATH%\bin\libsfml-graphics-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-window-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-system-2.dll" "bin\"
    copy "%SFML_PATH%\bin\libsfml-audio-2.dll" "bin\"
)

echo Exécutable créé dans bin\OrenjicGame.exe
echo.
pause 