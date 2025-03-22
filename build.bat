@echo off
setlocal

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Create resources directories if they don't exist
if not exist "resources\fonts" mkdir resources\fonts
if not exist "resources\textures\Titles" mkdir resources\textures\Titles
if not exist "resources\sounds\BGM" mkdir resources\sounds\BGM

REM Create examples directory if it doesn't exist
if not exist "build\examples" mkdir build\examples

REM Navigate to build directory
cd build

REM Configure project with CMake
cmake -G "MinGW Makefiles" ..

REM Build project
cmake --build . --config Release

REM Copy required DLLs to output directory
copy ..\lib\sfml\bin\*.dll .\
copy ..\lib\tgui\bin\*.dll .\

REM Create resources directories in build directory
if not exist "resources\fonts" mkdir resources\fonts
if not exist "resources\textures\Titles" mkdir resources\textures\Titles
if not exist "resources\sounds\BGM" mkdir resources\sounds\BGM
if not exist "resources\maps" mkdir resources\maps

REM Copy resources to build directory
copy ..\resources\fonts\VeniceClassic.ttf .\resources\fonts\
copy ..\resources\textures\Titles\title-bg.png .\resources\textures\Titles\
copy ..\resources\sounds\BGM\012-Theme01.mid .\resources\sounds\BGM\

REM Copy example maps to build directory
mkdir .\examples\maps
copy ..\examples\maps\*.json .\examples\maps\

REM Create a demos.txt file for the main menu to read
echo GameMap Demo - Shows Tiled map integration with Box2D physics (Demo coming soon) > .\examples\demos.txt
echo Camera Demo - Demonstrates camera follow and effects (Demo coming soon) >> .\examples\demos.txt

REM Return to root directory
cd ..

echo Build completed. Executable is in the build directory.
echo To run the main game: cd build && orenji-cpp.exe
pause 