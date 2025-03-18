#include "GameData.h"
#include <iostream>

// Initialisation de l'instance unique
std::unique_ptr<GameData> GameData::s_instance = nullptr; 