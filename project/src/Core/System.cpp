#include "Core/System.hpp"
#include "Core/EntityManager.hpp"

namespace Orenji
{
    System::System(const std::string &name)
        : m_name(name), m_active(true), m_entityManager(nullptr)
    {
    }

    void System::initialize(EntityManager *entityManager)
    {
        m_entityManager = entityManager;
    }

    std::vector<EntityPtr> System::getCompatibleEntities() const
    {
        std::vector<EntityPtr> compatibleEntities;

        if (!m_entityManager)
            return compatibleEntities;

        for (auto &entity : m_entityManager->getAllEntities())
        {
            if (isCompatibleWithEntity(entity))
            {
                compatibleEntities.push_back(entity);
            }
        }

        return compatibleEntities;
    }

} // namespace Orenji