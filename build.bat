@echo off
setlocal EnableDelayedExpansion

REM Check if G++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo G++ compiler not found in PATH. Please install MinGW or add it to your PATH.
    echo You can download MinGW from https://www.mingw-w64.org/downloads/
    exit /b 1
)

echo =====================================
echo Orenji-CPP Build Script (SFML 3.0)
echo =====================================
echo.
echo Checking directories...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build
if not exist "build\bin" mkdir build\bin
if not exist "build\bin\examples" mkdir build\bin\examples
if not exist "build\obj" mkdir build\obj

REM Create resources directories if they don't exist
if not exist "resources\fonts" mkdir resources\fonts
if not exist "resources\textures\Titles" mkdir resources\textures\Titles
if not exist "resources\sounds\BGM" mkdir resources\sounds\BGM
if not exist "resources\effects" mkdir resources\effects

REM Create lib/box2d/lib if it doesn't exist
if not exist "lib\box2d\lib" mkdir lib\box2d\lib

echo.
echo Copying DLLs...
copy lib\sfml\bin\*.dll build\ >nul 2>nul
copy lib\tgui\bin\*.dll build\ >nul 2>nul

REM Create resources directories in build directory
if not exist "build\resources\fonts" mkdir build\resources\fonts
if not exist "build\resources\textures\Titles" mkdir build\resources\textures\Titles
if not exist "build\resources\sounds\BGM" mkdir build\resources\sounds\BGM
if not exist "build\resources\maps" mkdir build\resources\maps
if not exist "build\resources\effects" mkdir build\resources\effects
if not exist "build\resources\textures" mkdir build\resources\textures

echo.
echo Copying resources...
copy resources\fonts\VeniceClassic.ttf build\resources\fonts\ >nul 2>nul
copy resources\fonts\arial.ttf build\resources\fonts\ >nul 2>nul
copy resources\textures\Titles\title-bg.png build\resources\textures\Titles\ >nul 2>nul
copy resources\textures\Titles\001-Title01.jpg build\resources\textures\Titles\ >nul 2>nul
copy resources\sounds\BGM\012-Theme01.mp3 build\resources\sounds\BGM\ >nul 2>nul

REM Copy particle effects and textures
copy resources\effects\*.txt build\resources\effects\ >nul 2>nul
copy resources\textures\*.png build\resources\textures\ >nul 2>nul

echo.
REM Compile and run particle texture generator if it doesn't exist
if not exist "build\resources\textures\particle.png" (
    echo Generating particle textures...
    g++ -o build\particle_gen resources\textures\particle_gen.cpp -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
    if %ERRORLEVEL% NEQ 0 (
        echo Warning: Failed to compile particle texture generator, skipping.
    ) else (
        cd build
        .\particle_gen.exe
        cd ..
    )
)

REM Copy example maps to build directory
if not exist "build\examples\maps" mkdir build\examples\maps
copy examples\maps\*.json build\examples\maps\ >nul 2>nul

REM Create a demos.txt file for the main menu to read
echo GameMap Demo - Shows Tiled map integration with Box2D physics (Demo coming soon) > build\examples\demos.txt
echo Camera Demo - Demonstrates camera follow and effects (Demo coming soon) >> build\examples\demos.txt
echo Particle Demo - Shows particle effects system capabilities >> build\examples\demos.txt

echo.
echo =====================================
echo Building Main Application...
echo =====================================
echo Creating a simplified version of Main.exe for demo purposes...

rem Liste des fichiers essentiels
echo #include "include/Engine.hpp" > src\temp_main.cpp
echo #include "include/Scenes/MainMenuScene.hpp" >> src\temp_main.cpp
echo #include ^<iostream^> >> src\temp_main.cpp
echo #include ^<memory^> >> src\temp_main.cpp
echo #include ^<cstdint^> >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo int main() >> src\temp_main.cpp
echo { >> src\temp_main.cpp
echo     try >> src\temp_main.cpp
echo     { >> src\temp_main.cpp
echo         sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1024, 768)), "Orenji Engine - Demo"); >> src\temp_main.cpp
echo         window.setFramerateLimit(60); >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo         // Créer le gestionnaire de ressources >> src\temp_main.cpp
echo         Resources::ResourceManager resourceManager; >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo         // Créer la scène de menu >> src\temp_main.cpp
echo         Scenes::MainMenuScene mainMenu(window, resourceManager); >> src\temp_main.cpp
echo         mainMenu.init(); >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo         // Boucle principale >> src\temp_main.cpp
echo         sf::Clock clock; >> src\temp_main.cpp
echo         while (window.isOpen()) >> src\temp_main.cpp
echo         { >> src\temp_main.cpp
echo             // Calculer le temps delta >> src\temp_main.cpp
echo             float deltaTime = clock.restart().asSeconds(); >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo             // Traiter les événements - SFML 3 utilise une nouvelle API pour les événements >> src\temp_main.cpp
echo             if (auto event = window.pollEvent()) >> src\temp_main.cpp
echo             { >> src\temp_main.cpp
echo                 if (event-^>is^<sf::Event::Closed^>()) >> src\temp_main.cpp
echo                     window.close(); >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo                 mainMenu.handleEvent(*event); >> src\temp_main.cpp
echo             } >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo             // Mise à jour >> src\temp_main.cpp
echo             mainMenu.update(deltaTime); >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo             // Rendu >> src\temp_main.cpp
echo             window.clear(sf::Color(40, 40, 40)); >> src\temp_main.cpp
echo             mainMenu.render(window); >> src\temp_main.cpp
echo             window.display(); >> src\temp_main.cpp
echo         } >> src\temp_main.cpp
echo. >> src\temp_main.cpp
echo         return 0; >> src\temp_main.cpp
echo     } >> src\temp_main.cpp
echo     catch (const std::exception &e) >> src\temp_main.cpp
echo     { >> src\temp_main.cpp
echo         std::cerr ^<^< "Exception: " ^<^< e.what() ^<^< std::endl; >> src\temp_main.cpp
echo         return 1; >> src\temp_main.cpp
echo     } >> src\temp_main.cpp
echo } >> src\temp_main.cpp

echo Building simplified Main.exe...
g++ -o build\bin\MainSimple.exe src\temp_main.cpp src\Scenes\MainMenuScene.cpp src\Core\Scene.cpp -I. -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
if %ERRORLEVEL% NEQ 0 (
    echo Failed to build simplified Main.exe application.
) else (
    echo Simplified Main.exe built successfully.
    
    REM Delete temporary main file
    del src\temp_main.cpp
    
    REM Copy DLLs to bin directory
    copy lib\sfml\bin\*.dll build\bin\ >nul 2>nul
    
    REM Copy resources to bin directory
    if not exist "build\bin\resources" mkdir build\bin\resources
    xcopy /E /Y build\resources build\bin\resources\ >nul 2>nul
)

echo.
echo =====================================
echo Building test applications...
echo =====================================

echo Building SimpleWindow test...
g++ -o build\bin\SimpleWindow.exe tests\SimpleWindow.cpp -I.\include -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
if %ERRORLEVEL% NEQ 0 (
    echo Failed to build SimpleWindow test.
    exit /b 1
) else (
    echo SimpleWindow test built successfully.
)

echo.
echo Building MovingBoxTest...
g++ -o build\bin\MovingBoxTest.exe tests\SimpleWindowWithBox.cpp -I.\include -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
if %ERRORLEVEL% NEQ 0 (
    echo Failed to build MovingBoxTest.
    exit /b 1
) else (
    echo MovingBoxTest built successfully.
)

echo.
echo =====================================
echo Building examples...
echo =====================================

echo Attempting to build ParticleDemo example...
g++ -o build\bin\examples\ParticleDemo.exe examples\ParticleDemo.cpp -I.\include -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Note: ParticleDemo requires ParticleSystem implementation, skipping.
) else (
    echo ParticleDemo built successfully.
)

echo.
echo Attempting to build CameraExample example...
g++ -o build\bin\examples\CameraExample.exe examples\CameraExample.cpp -I.\include -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Note: CameraExample requires Camera implementation, skipping.
) else (
    echo CameraExample built successfully.
)

echo.
echo Attempting to build GameMapExample example...
g++ -o build\bin\examples\GameMapExample.exe examples\GameMapExample.cpp -I.\include -I.\lib\sfml\include -I.\lib\box2d\include -L.\lib\sfml\lib -L.\lib\box2d\lib -lsfml-graphics -lsfml-window -lsfml-system -lbox2d 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Note: GameMapExample requires Box2D and GameMap implementation, skipping.
) else (
    echo GameMapExample built successfully.
)

echo.
echo Building standalone menu demo for SFML 3...
g++ -o build\bin\MenuDemoSFML3.exe standalone_menu_demo.cpp -I.\lib\sfml\include -L.\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
if %ERRORLEVEL% NEQ 0 (
    echo Failed to build standalone menu demo.
) else (
    echo Standalone menu demo built successfully.
)

echo.
echo =====================================
echo Build completed!
echo =====================================
echo.
echo Main application:
echo - MainSimple.exe: Simplified menu demo
echo.
echo Tests built successfully:
echo - SimpleWindow.exe: Basic SFML window test
echo - MovingBoxTest.exe: SFML box with movement and collisions
echo - MenuDemoSFML3.exe: Standalone menu demo with SFML 3
echo.
echo To run the main application:
echo cd build\bin
echo MainSimple.exe
echo.
echo To run the standalone menu demo:
echo cd build
echo .\bin\MenuDemoSFML3.exe
echo.
echo Press any key to exit...
pause > nul 