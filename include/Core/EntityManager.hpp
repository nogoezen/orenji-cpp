#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "Entity.hpp"

namespace Orenji {

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    std::shared_ptr<Entity> createEntity(const std::string& name = "");
    void removeEntity(std::shared_ptr<Entity> entity);
    void removeEntity(unsigned int entityId);
    void removeAllEntities();

    std::shared_ptr<Entity> getEntity(unsigned int entityId) const;
    std::vector<std::shared_ptr<Entity>> getAllEntities() const;
    std::vector<std::shared_ptr<Entity>> getEntitiesByName(const std::string& name) const;

    void update(float deltaTime);

private:
    std::unordered_map<unsigned int, std::shared_ptr<Entity>> m_entities;
    unsigned int m_nextEntityId;
};

} // namespace Orenji 