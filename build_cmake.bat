@echo off
echo Compilation avec CMake
echo ====================

:: Vérifier si CMake est installé
cmake --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: CMake n'est pas installé ou n'est pas dans le PATH.
    echo Veuillez installer CMake depuis https://cmake.org/download/
    pause
    exit /b 1
)

:: Vérifier si le compilateur est disponible
set MINGW_PATH=c:\msys64\ucrt64
set PATH=%MINGW_PATH%\bin;%PATH%

g++ --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: Le compilateur g++ n'est pas disponible.
    echo Vérifiez que MinGW est correctement installé.
    pause
    exit /b 1
)

:: Vérifier l'existence du fichier json.hpp
if not exist "include\nlohmann\json.hpp" (
    echo ATTENTION: Le fichier include\nlohmann\json.hpp est introuvable!
    echo La compilation pourrait échouer.
)

:: Créer le répertoire de build
if not exist "build" mkdir build

:: Se déplacer dans le répertoire de build
cd build

:: Configurer avec CMake
echo Configuration du projet avec CMake...
cmake -G "MinGW Makefiles" ..

if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: La configuration CMake a échoué.
    cd ..
    pause
    exit /b 1
)

:: Compiler le projet
echo Compilation du projet...
cmake --build .

if %ERRORLEVEL% NEQ 0 (
    echo ERREUR: La compilation a échoué.
    cd ..
    pause
    exit /b 1
) else (
    echo Compilation réussie!
    echo L'exécutable se trouve dans build\bin\UnchartedWaters.exe
)

:: Revenir au répertoire principal
cd ..

pause 