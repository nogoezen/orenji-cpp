#include "GameConfig.h"
#include <iostream>

// Initialisation de l'instance unique
std::unique_ptr<GameConfig> GameConfig::s_instance = nullptr; 