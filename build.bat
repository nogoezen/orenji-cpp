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
set "MAPS_DIR=%WORKING_DIR%\resources\maps"

:: Emplacement des bibliothèques MSYS2 UCRT64
set "MSYS2_DIR=C:\msys64\ucrt64"
set "MSYS2_BIN=%MSYS2_DIR%\bin"

:: Vérification des arguments
if "%1"=="clean" (
    echo %jaune%Nettoyage des fichiers de compilation...%reset%
    if exist "%BIN_DIR%" rmdir /S /Q "%BIN_DIR%"
    if exist "%OBJ_DIR%" rmdir /S /Q "%OBJ_DIR%"
    echo %vert%Nettoyage terminé.%reset%
    goto :fin
)

if "%1"=="title_example" (
    echo %jaune%Compilation de l'exemple de menu titre...%reset%
    set "MAIN_FILE=src\main_title_example.cpp"
    goto :compile_example
)

:: Création des répertoires si nécessaires
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"
if not exist "%DATA_DIR%" mkdir "%DATA_DIR%"
if not exist "%MAPS_DIR%" mkdir "%MAPS_DIR%"

echo %vert%Compilation du projet%reset%
echo ====================

:: Préparation des ressources
echo %bleu%Préparation des répertoires de ressources...%reset%
if not exist "%BIN_DIR%\resources" mkdir "%BIN_DIR%\resources"
if not exist "%BIN_DIR%\resources\textures" mkdir "%BIN_DIR%\resources\textures"
if not exist "%BIN_DIR%\resources\sounds" mkdir "%BIN_DIR%\resources\sounds"
if not exist "%BIN_DIR%\resources\fonts" mkdir "%BIN_DIR%\resources\fonts"
if not exist "%BIN_DIR%\resources\data" mkdir "%BIN_DIR%\resources\data"
if not exist "%BIN_DIR%\resources\maps" mkdir "%BIN_DIR%\resources\maps"

:: Fichiers essentiels spécifiques
set "ESSENTIAL_FILES="
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Game.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\State.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\StateMachine.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\TitleState.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\States\GameState.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\Entity.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\Component.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\EntityManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\RenderSystem.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Core\SystemManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Utilities\ResourceManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Utilities\InputHandler.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\ui\GuiUI.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Physics\PhysicsWorld.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Physics\PhysicsComponent.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Particles\ParticleSystem.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Particles\ParticleComponent.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\models\Ship.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\models\City.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\models\TradeGood.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\models\TradingSystem.cpp""
:: Nouveaux fichiers de Scene
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Scene\SceneNode.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Scene\Scene.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Scene\SceneManager.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Scene\TextNode.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Scene\SpriteNode.cpp""
set "ESSENTIAL_FILES=!ESSENTIAL_FILES! "%SRC_DIR%\Scene\TitleScene.cpp""

:: Variables de compilation
set "COMPILER=g++"
set "STD_VERSION=-std=c++17"
set "OPTIMIZATION=-O2"
set "WARNINGS=-Wall -Wextra -Wno-unused-parameter"
set "DEBUG=-g"

:: Vérification des bibliothèques installées via MSYS2
set "SFML_VERSION=3"
set "BOX2D_VERSION=3"

:: Chemins d'inclusion pour les bibliothèques
:: Vérifie d'abord le chemin MSYS2, sinon utilise le chemin local
if exist "%MSYS2_DIR%\include\SFML" (
    echo %vert%Utilisation des bibliothèques MSYS2 UCRT64%reset%
    
    :: Vérification de la version SFML
    if exist "%MSYS2_DIR%\include\SFML\Window\Window.hpp" (
        echo %vert%SFML trouvé dans MSYS2 UCRT64%reset%
        set "SFML_INCLUDES=-I"%MSYS2_DIR%\include""
        set "SFML_LIBS=-L"%MSYS2_DIR%\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio"
    ) else (
        echo %rouge%SFML non trouvé dans MSYS2 UCRT64, vérifiez l'installation%reset%
        set "SFML_INCLUDES="
        set "SFML_LIBS="
    )
    
    :: Vérification de Box2D
    if exist "%MSYS2_DIR%\include\box2d\box2d.h" (
        echo %vert%Box2D trouvé dans MSYS2 UCRT64%reset%
        set "BOX2D_INCLUDES=-I"%MSYS2_DIR%\include\box2d""
        set "BOX2D_LIBS=-L"%MSYS2_DIR%\lib" -lbox2d"
    ) else (
        echo %rouge%Box2D non trouvé dans MSYS2 UCRT64, vérifiez l'installation%reset%
        set "BOX2D_INCLUDES="
        set "BOX2D_LIBS="
    )
    
    set "USING_MSYS2=1"
) else (
    echo %jaune%Bibliothèques MSYS2 non trouvées, utilisation des bibliothèques locales%reset%
    
    :: Configuration SFML locale
    if exist "%WORKING_DIR%\lib\sfml\include" (
        set "SFML_INCLUDES=-I"%WORKING_DIR%\lib\sfml\include""
        set "SFML_LIBS=-L"%WORKING_DIR%\lib\sfml\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio"
    ) else (
        echo %rouge%SFML non trouvé localement, vérifiez l'installation%reset%
        set "SFML_INCLUDES="
        set "SFML_LIBS="
    )
    
    :: Configuration Box2D locale
    if exist "%WORKING_DIR%\lib\box2d\include" (
        set "BOX2D_INCLUDES=-I"%WORKING_DIR%\lib\box2d\include""
        set "BOX2D_LIBS=-L"%WORKING_DIR%\lib\box2d\lib" -lbox2d"
    ) else (
        echo %rouge%Box2D non trouvé localement, vérifiez l'installation%reset%
        set "BOX2D_INCLUDES="
        set "BOX2D_LIBS="
    )
    
    set "USING_MSYS2=0"
)

:: Rassemblement des chemins d'inclusion et des bibliothèques
set "INCLUDES=-I"%INCLUDE_DIR%" %SFML_INCLUDES% %BOX2D_INCLUDES%"
set "LIBS=%SFML_LIBS% %BOX2D_LIBS%"

:: Vérification de Thor
set "USE_THOR=0"
if "%USING_MSYS2%"=="1" (
    if exist "%MSYS2_DIR%\include\Thor" (
        echo %vert%Bibliothèque Thor trouvée, ajout aux dépendances%reset%
        set "INCLUDES=%INCLUDES% -I"%MSYS2_DIR%\include\Thor""
        set "LIBS=%LIBS% -lthor"
        set "USE_THOR=1"
    )
) else (
    if exist "%WORKING_DIR%\lib\thor\include" (
        echo %vert%Bibliothèque Thor trouvée, ajout aux dépendances%reset%
        set "INCLUDES=%INCLUDES% -I"%WORKING_DIR%\lib\thor\include""
        set "LIBS=%LIBS% -L"%WORKING_DIR%\lib\thor\lib" -lthor"
        set "USE_THOR=1"
    )
)

:: Vérification de TMXLite
set "USE_TMXLITE=0"
if "%USING_MSYS2%"=="1" (
    if exist "%MSYS2_DIR%\include\tmxlite" (
        echo %vert%Bibliothèque TMXLite trouvée, ajout aux dépendances%reset%
        set "INCLUDES=%INCLUDES% -I"%MSYS2_DIR%\include\tmxlite""
        set "LIBS=%LIBS% -ltmxlite"
        set "USE_TMXLITE=1"
    )
) else (
    if exist "%WORKING_DIR%\lib\tmxlite\include" (
        echo %vert%Bibliothèque TMXLite trouvée, ajout aux dépendances%reset%
        set "INCLUDES=%INCLUDES% -I"%WORKING_DIR%\lib\tmxlite\include""
        set "LIBS=%LIBS% -L"%WORKING_DIR%\lib\tmxlite\lib" -ltmxlite"
        set "USE_TMXLITE=1"
    )
)

:: Ajout des directives de compilation conditionnelles
set "DEFINES="
if "%USE_THOR%"=="0" (
    set "DEFINES=%DEFINES% -DDISABLE_THOR"
    echo %jaune%Compilation sans support Thor%reset%
)

:: Complémentaires pour Box2D v3 et SFML v3
if "%BOX2D_VERSION%"=="3" (
    set "DEFINES=%DEFINES% -DBOX2D_V3"
    echo %vert%Utilisation de Box2D version 3%reset%
)

if "%SFML_VERSION%"=="3" (
    set "DEFINES=%DEFINES% -DSFML_V3"
    echo %vert%Utilisation de SFML version 3%reset%
)

goto :compile_main

:compile_example
:: Commande de compilation pour l'exemple spécifié
set "COMPILE_CMD=%COMPILER% %STD_VERSION% %OPTIMIZATION% %WARNINGS% %DEBUG% %INCLUDES% %DEFINES% -o %BIN_DIR%\title_example.exe !MAIN_FILE! %ESSENTIAL_FILES% %LIBS%"

:: Exécution de la compilation
echo %bleu%Compilation de l'exemple...%reset%
echo %bleu%Commande: %COMPILE_CMD%%reset%
call %COMPILE_CMD%

:: Vérification du résultat
if %ERRORLEVEL% NEQ 0 (
    echo %rouge%ERREUR: La compilation a échoué.%reset%
    pause
    goto :fin
)

echo %vert%Exemple compilé dans %BIN_DIR%\title_example.exe%reset%
goto :copy_resources

:compile_main
:: Commande de compilation principale
set "COMPILE_CMD=%COMPILER% %STD_VERSION% %OPTIMIZATION% %WARNINGS% %DEBUG% %INCLUDES% %DEFINES% -o %BIN_DIR%\OrenjicGame.exe main.cpp %ESSENTIAL_FILES% %LIBS%"

:: Exécution de la compilation
echo %bleu%Compilation des fichiers essentiels...%reset%
echo %bleu%Commande: %COMPILE_CMD%%reset%
call %COMPILE_CMD%

:: Vérification du résultat
if %ERRORLEVEL% NEQ 0 (
    echo %rouge%ERREUR: La compilation a échoué.%reset%
    pause
    goto :fin
)

echo %vert%Exécutable créé dans %BIN_DIR%\OrenjicGame.exe%reset%

:copy_resources
:: Copie des ressources
if exist "%RESOURCES_DIR%\sounds" (
    echo Copie des sons...
    xcopy /E /Y /I "%RESOURCES_DIR%\sounds" "%BIN_DIR%\resources\sounds" > nul
)

if exist "%RESOURCES_DIR%\textures" (
    echo Copie des textures...
    xcopy /E /Y /I "%RESOURCES_DIR%\textures" "%BIN_DIR%\resources\textures" > nul
)

if exist "%RESOURCES_DIR%\fonts" (
    echo Copie des polices...
    xcopy /E /Y /I "%RESOURCES_DIR%\fonts" "%BIN_DIR%\resources\fonts" > nul
)

if exist "%RESOURCES_DIR%\maps" (
    echo Copie des cartes...
    xcopy /E /Y /I "%RESOURCES_DIR%\maps" "%BIN_DIR%\resources\maps" > nul
)

:: Copie des DLLs en fonction de la source (MSYS2 ou locale)
echo Copie des DLLs nécessaires...

if "%USING_MSYS2%"=="1" (
    :: Copie des DLLs depuis MSYS2
    echo Copie des DLLs depuis MSYS2 UCRT64...
    
    :: DLLs SFML
    if "%SFML_VERSION%"=="3" (
        :: SFML 3.x
        copy "%MSYS2_BIN%\libsfml-graphics-3.dll" "%BIN_DIR%\" > nul
        copy "%MSYS2_BIN%\libsfml-window-3.dll" "%BIN_DIR%\" > nul
        copy "%MSYS2_BIN%\libsfml-system-3.dll" "%BIN_DIR%\" > nul
        copy "%MSYS2_BIN%\libsfml-audio-3.dll" "%BIN_DIR%\" > nul
    ) else (
        :: SFML 2.x
        copy "%MSYS2_BIN%\libsfml-graphics-2.dll" "%BIN_DIR%\" > nul
        copy "%MSYS2_BIN%\libsfml-window-2.dll" "%BIN_DIR%\" > nul
        copy "%MSYS2_BIN%\libsfml-system-2.dll" "%BIN_DIR%\" > nul
        copy "%MSYS2_BIN%\libsfml-audio-2.dll" "%BIN_DIR%\" > nul
    )
    
    :: Dépendances de SFML
    copy "%MSYS2_BIN%\libfreetype-6.dll" "%BIN_DIR%\" > nul
    copy "%MSYS2_BIN%\libbz2-1.dll" "%BIN_DIR%\" > nul
    copy "%MSYS2_BIN%\libpng16-16.dll" "%BIN_DIR%\" > nul
    copy "%MSYS2_BIN%\libwinpthread-1.dll" "%BIN_DIR%\" > nul
    copy "%MSYS2_BIN%\zlib1.dll" "%BIN_DIR%\" > nul
    copy "%MSYS2_BIN%\libopenal-1.dll" "%BIN_DIR%\" > nul
    
    :: Nouvelles dépendances pour SFML 3
    if "%SFML_VERSION%"=="3" (
        copy "%MSYS2_BIN%\libminiaudio.dll" "%BIN_DIR%\" > nul 2>&1
        copy "%MSYS2_BIN%\libvorbis-0.dll" "%BIN_DIR%\" > nul 2>&1
        copy "%MSYS2_BIN%\libvorbisenc-2.dll" "%BIN_DIR%\" > nul 2>&1
        copy "%MSYS2_BIN%\libvorbisfile-3.dll" "%BIN_DIR%\" > nul 2>&1
        copy "%MSYS2_BIN%\libogg-0.dll" "%BIN_DIR%\" > nul 2>&1
        copy "%MSYS2_BIN%\libFLAC-8.dll" "%BIN_DIR%\" > nul 2>&1
    )
    
    :: Dépendances générales
    copy "%MSYS2_BIN%\libgcc_s_seh-1.dll" "%BIN_DIR%\" > nul
    copy "%MSYS2_BIN%\libstdc++-6.dll" "%BIN_DIR%\" > nul
    
    :: DLL Box2D
    if "%BOX2D_VERSION%"=="3" (
        if exist "%MSYS2_BIN%\libbox2d-3.dll" (
            copy "%MSYS2_BIN%\libbox2d-3.dll" "%BIN_DIR%\" > nul
        ) else if exist "%MSYS2_BIN%\libbox2d.dll" (
            copy "%MSYS2_BIN%\libbox2d.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: libbox2d.dll non trouvée dans MSYS2%reset%
        )
    ) else (
        if exist "%MSYS2_BIN%\libbox2d.dll" (
            copy "%MSYS2_BIN%\libbox2d.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: libbox2d.dll non trouvée dans MSYS2%reset%
        )
    )
    
    :: DLL Thor si disponible
    if "%USE_THOR%"=="1" (
        if exist "%MSYS2_BIN%\libthor.dll" (
            copy "%MSYS2_BIN%\libthor.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: libthor.dll non trouvée dans MSYS2%reset%
        )
    )
    
    :: DLL TMXLite si disponible
    if "%USE_TMXLITE%"=="1" (
        if exist "%MSYS2_BIN%\libtmxlite.dll" (
            copy "%MSYS2_BIN%\libtmxlite.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: libtmxlite.dll non trouvée dans MSYS2%reset%
        )
        
        :: Dépendances spéciales pour tmxlite
        copy "%MSYS2_BIN%\libpugixml.dll" "%BIN_DIR%\" > nul 2>&1
        copy "%MSYS2_BIN%\libzstd.dll" "%BIN_DIR%\" > nul 2>&1
    )
) else (
    :: Copie des DLLs depuis les dossiers locaux
    echo Copie des DLLs depuis les dossiers locaux...
    
    :: DLLs SFML
    if exist "%WORKING_DIR%\lib\sfml\bin" (
        if "%SFML_VERSION%"=="3" (
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-graphics-3.dll" "%BIN_DIR%\" > nul
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-window-3.dll" "%BIN_DIR%\" > nul
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-system-3.dll" "%BIN_DIR%\" > nul
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-audio-3.dll" "%BIN_DIR%\" > nul
        ) else (
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-graphics-2.dll" "%BIN_DIR%\" > nul
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-window-2.dll" "%BIN_DIR%\" > nul
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-system-2.dll" "%BIN_DIR%\" > nul
            copy "%WORKING_DIR%\lib\sfml\bin\sfml-audio-2.dll" "%BIN_DIR%\" > nul
        )
    ) else (
        echo %jaune%Avertissement: Dossier des DLLs SFML non trouvé%reset%
    )
    
    :: DLL Box2D
    if "%BOX2D_VERSION%"=="3" (
        if exist "%WORKING_DIR%\lib\box2d\bin\box2d-3.dll" (
            copy "%WORKING_DIR%\lib\box2d\bin\box2d-3.dll" "%BIN_DIR%\" > nul
        ) else if exist "%WORKING_DIR%\lib\box2d\bin\box2d.dll" (
            copy "%WORKING_DIR%\lib\box2d\bin\box2d.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: box2d.dll non trouvée%reset%
        )
    ) else (
        if exist "%WORKING_DIR%\lib\box2d\bin\box2d.dll" (
            copy "%WORKING_DIR%\lib\box2d\bin\box2d.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: box2d.dll non trouvée%reset%
        )
    )
    
    :: DLL Thor si disponible
    if "%USE_THOR%"=="1" (
        if exist "%WORKING_DIR%\lib\thor\bin\thor.dll" (
            copy "%WORKING_DIR%\lib\thor\bin\thor.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: thor.dll non trouvée%reset%
        )
    )
    
    :: DLL TMXLite si disponible
    if "%USE_TMXLITE%"=="1" (
        if exist "%WORKING_DIR%\lib\tmxlite\bin\tmxlite.dll" (
            copy "%WORKING_DIR%\lib\tmxlite\bin\tmxlite.dll" "%BIN_DIR%\" > nul
        ) else (
            echo %jaune%Avertissement: tmxlite.dll non trouvée%reset%
        )
    )
)

:fin
echo %vert%Terminé.%reset%
endlocal 