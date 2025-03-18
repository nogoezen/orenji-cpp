#include "DataManager.h"
#include <iostream>

// Initialisation de l'instance unique
std::unique_ptr<DataManager> DataManager::s_instance = nullptr; 