#pragma once
#include <memory>
#include <vector>
#include <string>

namespace Orenji {

class Entity;
class EntityManager;

class System {
public:
    System() = default;
    virtual ~System() = default;

    virtual void initialize() = 0;
    virtual void update(float deltaTime) = 0;
    
    void setEntityManager(std::shared_ptr<EntityManager> entityManager) {
        m_entityManager = entityManager;
    }

    std::shared_ptr<EntityManager> getEntityManager() const {
        return m_entityManager;
    }

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

protected:
    std::shared_ptr<EntityManager> m_entityManager;
    std::string m_name;
};

} // namespace Orenji 