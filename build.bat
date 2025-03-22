@echo off
setlocal

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Navigate to build directory
cd build

REM Configure project with CMake
cmake -G "MinGW Makefiles" ..

REM Build project
cmake --build . --config Release

REM Copy required DLLs to output directory
copy ..\lib\sfml\bin\*.dll .\
copy ..\lib\tgui\bin\*.dll .\

REM Return to root directory
cd ..

echo Build completed. Executable is in the build directory.
pause 