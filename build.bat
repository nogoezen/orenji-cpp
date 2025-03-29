@echo off
setlocal

REM Check if G++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo G++ compiler not found in PATH. Please install MinGW or add it to your PATH.
    echo You can download MinGW from https://www.mingw-w64.org/downloads/
    exit /b 1
)

echo =====================================
echo Orenji-CPP Build Script
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
copy resources\textures\Titles\title-bg.png build\resources\textures\Titles\ >nul 2>nul
copy resources\sounds\BGM\012-Theme01.mid build\resources\sounds\BGM\ >nul 2>nul

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
echo =====================================
echo Build completed!
echo =====================================
echo.
echo Tests built successfully:
echo - SimpleWindow.exe: Basic SFML window test
echo - MovingBoxTest.exe: SFML box with movement and collisions
echo.
echo To run a test:
echo cd build\bin
echo SimpleWindow.exe
echo   or
echo MovingBoxTest.exe
echo.
echo Press any key to exit...
pause > nul 