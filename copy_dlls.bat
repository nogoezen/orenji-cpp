@echo off
setlocal enabledelayedexpansion

echo ===== Script de copie des DLL SFML pour le projet Orenji-CPP =====

:: Définir les chemins
set "WORKING_DIR=%CD%"
set "BIN_DIR=%WORKING_DIR%\bin"
set "LIB_DIR=%WORKING_DIR%\lib"
set "SFML_DIR=%LIB_DIR%\sfml"

:: Vérifier si le dossier des DLL existe
if not exist "%SFML_DIR%\bin" (
    echo Erreur: Le dossier %SFML_DIR%\bin n'existe pas.
    echo Veuillez vous assurer que SFML est correctement installé dans %SFML_DIR%.
    goto :error
)

:: Créer le dossier bin s'il n'existe pas
if not exist "%BIN_DIR%" (
    echo Création du dossier bin...
    mkdir "%BIN_DIR%"
)

:: Copier les DLL SFML (version debug et release)
echo Copie des DLL SFML...

:: Version Debug
if exist "%SFML_DIR%\bin\sfml-graphics-d-2.dll" (
    echo Copie des DLL Debug...
    copy "%SFML_DIR%\bin\sfml-graphics-d-2.dll" "%BIN_DIR%\"
    copy "%SFML_DIR%\bin\sfml-window-d-2.dll" "%BIN_DIR%\"
    copy "%SFML_DIR%\bin\sfml-system-d-2.dll" "%BIN_DIR%\"
    copy "%SFML_DIR%\bin\sfml-audio-d-2.dll" "%BIN_DIR%\"
)

:: Version Release
if exist "%SFML_DIR%\bin\sfml-graphics-2.dll" (
    echo Copie des DLL Release...
    copy "%SFML_DIR%\bin\sfml-graphics-2.dll" "%BIN_DIR%\"
    copy "%SFML_DIR%\bin\sfml-window-2.dll" "%BIN_DIR%\"
    copy "%SFML_DIR%\bin\sfml-system-2.dll" "%BIN_DIR%\"
    copy "%SFML_DIR%\bin\sfml-audio-2.dll" "%BIN_DIR%\"
)

:: Si aucune DLL n'a été trouvée, essayer un autre chemin standard
if not exist "%BIN_DIR%\sfml-graphics-2.dll" if not exist "%BIN_DIR%\sfml-graphics-d-2.dll" (
    echo Aucune DLL SFML trouvée dans %SFML_DIR%\bin. Tentative de recherche dans des emplacements standards...
    
    :: Essayer le chemin via vcpkg
    if exist "C:\vcpkg\installed\x64-windows\bin" (
        echo Tentative avec vcpkg...
        copy "C:\vcpkg\installed\x64-windows\bin\sfml-graphics-2.dll" "%BIN_DIR%\"
        copy "C:\vcpkg\installed\x64-windows\bin\sfml-window-2.dll" "%BIN_DIR%\"
        copy "C:\vcpkg\installed\x64-windows\bin\sfml-system-2.dll" "%BIN_DIR%\"
        copy "C:\vcpkg\installed\x64-windows\bin\sfml-audio-2.dll" "%BIN_DIR%\"
    )
)

:: Vérification finale
if exist "%BIN_DIR%\sfml-graphics-2.dll" (
    echo [SUCCÈS] Les DLL SFML ont été copiées avec succès dans %BIN_DIR%
) else if exist "%BIN_DIR%\sfml-graphics-d-2.dll" (
    echo [SUCCÈS] Les DLL SFML (Debug) ont été copiées avec succès dans %BIN_DIR%
) else (
    echo [ÉCHEC] Aucune DLL SFML n'a pu être copiée.
    echo Veuillez copier manuellement les DLL SFML suivantes dans le dossier %BIN_DIR% :
    echo - sfml-graphics-2.dll
    echo - sfml-window-2.dll
    echo - sfml-system-2.dll
    echo - sfml-audio-2.dll
    goto :error
)

:: Vérifier Box2D également
if exist "%LIB_DIR%\box2d\bin\box2d.dll" (
    echo Copie de la DLL Box2D...
    copy "%LIB_DIR%\box2d\bin\box2d.dll" "%BIN_DIR%\"
    echo [SUCCÈS] DLL Box2D copiée.
) else (
    echo [INFO] DLL Box2D non trouvée. Si nécessaire, veuillez la copier manuellement.
)

echo ===== Terminé =====
goto :end

:error
echo ===== Une erreur s'est produite =====
exit /b 1

:end
endlocal 