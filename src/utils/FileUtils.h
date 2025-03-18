#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <sstream>

// Fix pour les problèmes de compatibilité std::filesystem avec MinGW
#if defined(__MINGW32__) || defined(__MINGW64__)
namespace std
{
    namespace filesystem
    {
        // Opérateurs de comparaison pour path
        inline bool operator==(const path &lhs, const path &rhs)
        {
            return lhs.compare(rhs) == 0;
        }

        inline bool operator!=(const path &lhs, const path &rhs)
        {
            return lhs.compare(rhs) != 0;
        }

        // Opérateurs de comparaison path-string
        inline bool operator==(const path &lhs, const std::string &rhs)
        {
            return lhs.string() == rhs;
        }

        inline bool operator==(const std::string &lhs, const path &rhs)
        {
            return lhs == rhs.string();
        }

        inline bool operator!=(const path &lhs, const std::string &rhs)
        {
            return lhs.string() != rhs;
        }

        inline bool operator!=(const std::string &lhs, const path &rhs)
        {
            return lhs != rhs.string();
        }

        // Opérateurs de comparaison path-c_str
        inline bool operator==(const path &lhs, const char *rhs)
        {
            return lhs.string() == rhs;
        }

        inline bool operator==(const char *lhs, const path &rhs)
        {
            return std::string(lhs) == rhs.string();
        }

        inline bool operator!=(const path &lhs, const char *rhs)
        {
            return lhs.string() != rhs;
        }

        inline bool operator!=(const char *lhs, const path &rhs)
        {
            return std::string(lhs) != rhs.string();
        }
    }
}
#endif

namespace FileUtils
{
    /**
     * Vérifier si un fichier existe
     * @param path Chemin du fichier
     * @return true si le fichier existe, false sinon
     */
    inline bool fileExists(const std::string &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    /**
     * Vérifier si un répertoire existe
     * @param path Chemin du répertoire
     * @return true si le répertoire existe, false sinon
     */
    inline bool directoryExists(const std::string &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    /**
     * Créer un répertoire (et ses parents si nécessaire)
     * @param path Chemin du répertoire à créer
     * @return true si le répertoire a été créé ou existait déjà, false sinon
     */
    inline bool createDirectory(const std::string &path)
    {
        try
        {
            if (directoryExists(path))
            {
                return true;
            }
            return std::filesystem::create_directories(path);
        }
        catch (...)
        {
            return false;
        }
    }

    /**
     * Lire tout le contenu d'un fichier texte
     * @param path Chemin du fichier
     * @return Contenu du fichier
     * @throw std::runtime_error si le fichier ne peut pas être ouvert ou lu
     */
    inline std::string readTextFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Impossible d'ouvrir le fichier: " + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /**
     * Écrire du texte dans un fichier
     * @param path Chemin du fichier
     * @param content Contenu à écrire
     * @param append Ajouter au fichier existant (true) ou écraser (false)
     * @return true si l'écriture a réussi, false sinon
     */
    inline bool writeTextFile(const std::string &path, const std::string &content, bool append = false)
    {
        try
        {
            std::ofstream file(path, append ? std::ios::app : std::ios::trunc);
            if (!file.is_open())
            {
                return false;
            }

            file << content;
            return !file.fail();
        }
        catch (...)
        {
            return false;
        }
    }

    /**
     * Obtenir la liste des fichiers dans un répertoire
     * @param path Chemin du répertoire
     * @param extension Extension à filtrer (vide pour tous les fichiers)
     * @param recursive Parcourir les sous-répertoires
     * @return Liste des chemins de fichiers
     */
    inline std::vector<std::string> listFiles(const std::string &path,
                                              const std::string &extension = "",
                                              bool recursive = false)
    {
        std::vector<std::string> files;

        try
        {
            if (recursive)
            {
                for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
                {
                    if (entry.is_regular_file())
                    {
                        if (extension.empty() || entry.path().extension() == extension)
                        {
                            files.push_back(entry.path().string());
                        }
                    }
                }
            }
            else
            {
                for (const auto &entry : std::filesystem::directory_iterator(path))
                {
                    if (entry.is_regular_file())
                    {
                        if (extension.empty() || entry.path().extension() == extension)
                        {
                            files.push_back(entry.path().string());
                        }
                    }
                }
            }
        }
        catch (...)
        {
            // Ignorer les erreurs et retourner la liste partielle
        }

        return files;
    }

    /**
     * Obtenir la liste des répertoires dans un répertoire
     * @param path Chemin du répertoire
     * @param recursive Parcourir les sous-répertoires
     * @return Liste des chemins de répertoires
     */
    inline std::vector<std::string> listDirectories(const std::string &path, bool recursive = false)
    {
        std::vector<std::string> directories;

        try
        {
            if (recursive)
            {
                for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
                {
                    if (entry.is_directory())
                    {
                        directories.push_back(entry.path().string());
                    }
                }
            }
            else
            {
                for (const auto &entry : std::filesystem::directory_iterator(path))
                {
                    if (entry.is_directory())
                    {
                        directories.push_back(entry.path().string());
                    }
                }
            }
        }
        catch (...)
        {
            // Ignorer les erreurs et retourner la liste partielle
        }

        return directories;
    }

    /**
     * Joindre des chemins de manière portable
     * @param paths Liste des segments de chemin à joindre
     * @return Chemin joint
     */
    inline std::string joinPaths(const std::vector<std::string> &paths)
    {
        if (paths.empty())
        {
            return "";
        }

        std::filesystem::path result(paths[0]);

        for (size_t i = 1; i < paths.size(); ++i)
        {
            result /= paths[i];
        }

        return result.string();
    }

    /**
     * Obtenir l'extension d'un fichier
     * @param path Chemin du fichier
     * @return Extension (avec le point) ou chaîne vide
     */
    inline std::string getExtension(const std::string &path)
    {
        return std::filesystem::path(path).extension().string();
    }

    /**
     * Obtenir le nom du fichier sans le chemin
     * @param path Chemin du fichier
     * @return Nom du fichier
     */
    inline std::string getFileName(const std::string &path)
    {
        return std::filesystem::path(path).filename().string();
    }

    /**
     * Obtenir le chemin du répertoire parent
     * @param path Chemin
     * @return Chemin du répertoire parent
     */
    inline std::string getParentDirectory(const std::string &path)
    {
        return std::filesystem::path(path).parent_path().string();
    }
};