#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <stdexcept>

/**
 * @brief Classe utilitaire pour la gestion des fichiers
 */
namespace FileSystem
{
    /**
     * @brief Vérifie si un fichier existe
     * @param filePath Chemin du fichier
     * @return true si le fichier existe, false sinon
     */
    inline bool fileExists(const std::string &filePath)
    {
        return std::filesystem::exists(filePath);
    }

    /**
     * @brief Créer un répertoire et ses parents si nécessaire
     * @param dirPath Chemin du répertoire
     * @return true si la création a réussi ou si le répertoire existe déjà
     */
    inline bool createDirectory(const std::string &dirPath)
    {
        try
        {
            return std::filesystem::create_directories(dirPath);
        }
        catch (const std::exception &)
        {
            return false;
        }
    }

    /**
     * @brief Liste les fichiers d'un répertoire
     * @param dirPath Chemin du répertoire
     * @return Vecteur des noms de fichiers
     */
    inline std::vector<std::string> listFiles(const std::string &dirPath)
    {
        std::vector<std::string> files;

        try
        {
            for (const auto &entry : std::filesystem::directory_iterator(dirPath))
            {
                if (entry.is_regular_file())
                {
                    files.push_back(entry.path().filename().string());
                }
            }
        }
        catch (const std::exception &)
        {
            // Retourne une liste vide en cas d'erreur
        }

        return files;
    }

    /**
     * @brief Liste les répertoires d'un répertoire
     * @param dirPath Chemin du répertoire
     * @return Vecteur des noms de répertoires
     */
    inline std::vector<std::string> listDirectories(const std::string &dirPath)
    {
        std::vector<std::string> directories;

        try
        {
            for (const auto &entry : std::filesystem::directory_iterator(dirPath))
            {
                if (entry.is_directory())
                {
                    directories.push_back(entry.path().filename().string());
                }
            }
        }
        catch (const std::exception &)
        {
            // Retourne une liste vide en cas d'erreur
        }

        return directories;
    }

    /**
     * @brief Obtient le chemin absolu d'un fichier ou répertoire
     * @param path Chemin relatif ou absolu
     * @return Chemin absolu
     */
    inline std::string getAbsolutePath(const std::string &path)
    {
        try
        {
            return std::filesystem::absolute(path).string();
        }
        catch (const std::exception &)
        {
            return path; // En cas d'erreur, retourne le chemin d'origine
        }
    }

    /**
     * @brief Copie un fichier
     * @param sourcePath Chemin du fichier source
     * @param destPath Chemin du fichier destination
     * @return true si la copie a réussi, false sinon
     */
    inline bool copyFile(const std::string &sourcePath, const std::string &destPath)
    {
        try
        {
            std::filesystem::copy_file(
                sourcePath,
                destPath,
                std::filesystem::copy_options::overwrite_existing);
            return true;
        }
        catch (const std::exception &)
        {
            return false;
        }
    }
};