@echo off
echo Installation de SFML pour MinGW UCRT64
echo =====================================
echo.

:: Vérifier si MSYS2 est installé au bon endroit
if not exist "C:\msys64\msys2_shell.cmd" (
    echo ERREUR: MSYS2 n'est pas installé dans C:\msys64
    echo Veuillez installer MSYS2 ou modifier ce script pour pointer vers le bon chemin.
    pause
    exit /b 1
)

echo Lancement du shell MSYS2 UCRT64 pour installer SFML...
echo Veuillez confirmer l'installation en tapant 'y' lorsque demandé.
echo.
echo Ce script va exécuter : pacman -S mingw-w64-ucrt-x86_64-sfml
echo.
pause

:: Lancer MSYS2 avec la commande d'installation de SFML
start "" "C:\msys64\msys2_shell.cmd" -ucrt64 -here -c "pacman -S mingw-w64-ucrt-x86_64-sfml && echo 'Installation terminée. Vous pouvez fermer cette fenêtre.' && sleep 10"

echo.
echo Une fenêtre de terminal MSYS2 va s'ouvrir.
echo Suivez les instructions dans cette fenêtre pour installer SFML.
echo.
echo Après l'installation, vous pourrez compiler votre projet avec SFML.
echo. 