@echo off
echo Mise à jour du fichier compile_commands.json avec les chemins absolus
echo ==================================================================

:: Récupération du chemin absolu du répertoire de travail
set "WORKSPACE_PATH=%CD%"

:: Conversion des backslashes en forward slashes pour le JSON
set "WORKSPACE_PATH_SLASH=%WORKSPACE_PATH:\=/%"

:: Lecture du fichier compile_commands.json
echo Traitement du fichier compile_commands.json...
if not exist "compile_commands.json" (
    echo Erreur: compile_commands.json n'existe pas!
    exit /b 1
)

:: Création d'un fichier temporaire
type compile_commands.json > temp_compile_commands.json

:: Remplacement de ${workspaceFolder} par le chemin absolu
powershell -Command "(Get-Content temp_compile_commands.json) -replace '\$\{workspaceFolder\}', '%WORKSPACE_PATH_SLASH%' | Set-Content updated_compile_commands.json"

:: Remplacement du fichier original
move /y updated_compile_commands.json compile_commands.json

:: Nettoyage
del temp_compile_commands.json

echo.
echo Le fichier compile_commands.json a été mis à jour avec succès!
echo Les chemins absolus ont été remplacés pour une meilleure compatibilité 
echo avec Cursor et les outils d'analyse.
echo.

pause 