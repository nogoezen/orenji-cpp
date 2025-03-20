#include "../../include/States/GameState.hpp"
#include <iostream>

namespace Orenji
{
    GameState::GameState() : m_isPaused(false)
    {
    }

    GameState::~GameState()
    {
    }

    bool GameState::initialize()
    {
#ifdef DISABLE_GAMESTATE
        std::cout << "GameState is disabled in this build." << std::endl;
        return false;
#else
        // La vraie implémentation serait ici
        return true;
#endif
    }

    bool GameState::onEnter()
    {
#ifdef DISABLE_GAMESTATE
        std::cout << "GameState::onEnter - Not implemented in this build." << std::endl;
        return false;
#else
        return initialize();
#endif
    }

    bool GameState::onExit()
    {
#ifdef DISABLE_GAMESTATE
        std::cout << "GameState::onExit - Not implemented in this build." << std::endl;
#endif
        return true;
    }

    bool GameState::update(float deltaTime)
    {
#ifdef DISABLE_GAMESTATE
        std::cout << "GameState::update - Not implemented in this build." << std::endl;
#endif
        return true;
    }

    bool GameState::render()
    {
#ifdef DISABLE_GAMESTATE
        std::cout << "GameState::render - Not implemented in this build." << std::endl;
#endif
        return true;
    }

    bool GameState::handleInput()
    {
#ifdef DISABLE_GAMESTATE
        std::cout << "GameState::handleInput - Not implemented in this build." << std::endl;
#endif
        return true;
    }

} // namespace Orenji