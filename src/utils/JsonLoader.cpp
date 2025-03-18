#include "JsonLoader.h"

// Initialisation des variables statiques
std::unordered_map<std::string, nlohmann::json> JsonLoader::s_jsonCache;
std::string JsonLoader::s_basePath = "data"; 