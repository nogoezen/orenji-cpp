@echo off
setlocal

REM Check if CMake is available
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo CMake not found in PATH. Please install CMake or add it to your PATH.
    echo You can download it from https://cmake.org/download/
    exit /b 1
)

REM Check if G++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo G++ compiler not found in PATH. Please install MinGW or add it to your PATH.
    echo You can download MinGW from https://www.mingw-w64.org/downloads/
    exit /b 1
)

echo Building Box2D library...
if not exist "lib\box2d\lib" (
    echo Creating Box2D library directories...
    mkdir lib\box2d\lib
    
    if exist "lib\box2d\CMakeLists.txt" (
        echo Configuring Box2D...
        cd lib\box2d
        mkdir build
        cd build
        cmake -G "MinGW Makefiles" ..
        if %ERRORLEVEL% NEQ 0 (
            echo Failed to configure Box2D with CMake.
            cd ..\..\..
            exit /b 1
        )
        
        echo Building Box2D...
        cmake --build . --config Release
        if %ERRORLEVEL% NEQ 0 (
            echo Failed to build Box2D.
            cd ..\..\..
            exit /b 1
        )
        
        echo Copying Box2D libraries...
        copy bin\*.dll ..\lib\
        copy bin\*.a ..\lib\
        
        cd ..\..\..
    ) else (
        echo Warning: Box2D source not found. Please download Box2D and extract it to lib\box2d
        echo You can download it from https://github.com/erincatto/box2d/releases
        echo Continuing build without Box2D...
    )
)

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Create resources directories if they don't exist
if not exist "resources\fonts" mkdir resources\fonts
if not exist "resources\textures\Titles" mkdir resources\textures\Titles
if not exist "resources\sounds\BGM" mkdir resources\sounds\BGM
if not exist "resources\effects" mkdir resources\effects

REM Create examples directory if it doesn't exist
if not exist "build\examples" mkdir build\examples

REM Navigate to build directory
cd build

REM Configure project with CMake
echo Configuring project with CMake...
cmake -G "MinGW Makefiles" ..
if %ERRORLEVEL% NEQ 0 (
    echo Failed to configure project with CMake.
    cd ..
    exit /b 1
)

REM Build project
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo Failed to build project.
    cd ..
    exit /b 1
)

REM Copy required DLLs to output directory
echo Copying DLLs...
copy ..\lib\sfml\bin\*.dll .\
copy ..\lib\tgui\bin\*.dll .\

REM Create resources directories in build directory
if not exist "resources\fonts" mkdir resources\fonts
if not exist "resources\textures\Titles" mkdir resources\textures\Titles
if not exist "resources\sounds\BGM" mkdir resources\sounds\BGM
if not exist "resources\maps" mkdir resources\maps
if not exist "resources\effects" mkdir resources\effects
if not exist "resources\textures" mkdir resources\textures

REM Copy resources to build directory
echo Copying resources...
copy ..\resources\fonts\VeniceClassic.ttf .\resources\fonts\
copy ..\resources\textures\Titles\title-bg.png .\resources\textures\Titles\
copy ..\resources\sounds\BGM\012-Theme01.mid .\resources\sounds\BGM\

REM Copy particle effects and textures
copy ..\resources\effects\*.txt .\resources\effects\
copy ..\resources\textures\*.png .\resources\textures\

REM Compile and run particle texture generator if it doesn't exist
if not exist ".\resources\textures\particle.png" (
    echo Generating particle textures...
    g++ -o particle_gen ..\resources\textures\particle_gen.cpp -I..\lib\sfml\include -L..\lib\sfml\lib -lsfml-graphics -lsfml-window -lsfml-system
    .\particle_gen.exe
)

REM Copy example maps to build directory
mkdir .\examples\maps
copy ..\examples\maps\*.json .\examples\maps\

REM Create a demos.txt file for the main menu to read
echo GameMap Demo - Shows Tiled map integration with Box2D physics (Demo coming soon) > .\examples\demos.txt
echo Camera Demo - Demonstrates camera follow and effects (Demo coming soon) >> .\examples\demos.txt
echo Particle Demo - Shows particle effects system capabilities >> .\examples\demos.txt

REM Build the examples
echo Building examples...
mkdir .\bin\examples
cd ..\examples
for %%f in (*.cpp) do (
    echo Building example: %%~nf
    g++ -o ..\build\bin\examples\%%~nf.exe %%f -I..\include -I..\lib\sfml\include -I..\lib\box2d\include -L..\lib\sfml\lib -L..\lib\box2d\lib -lsfml-graphics -lsfml-window -lsfml-system -lbox2d
)
cd ..\build

REM Return to root directory
cd ..

echo Build completed. Executable is in the build directory.
echo To run the main game: cd build && orenji-cpp.exe
pause 