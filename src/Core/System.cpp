#include "../../include/Core/System.hpp"

namespace Core
{

    System::System(EntityManager &entityManager)
        : m_entityManager(entityManager)
    {
    }

    System::~System()
    {
    }

} // namespace Core