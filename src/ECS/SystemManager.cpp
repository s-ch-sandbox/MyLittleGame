#include "SystemManager.h"

#include "System.h"

namespace ECS {
SystemManager::SystemManager(EntityManager* entity_manager) : entity_manager_(entity_manager) {}

void SystemManager::Update(float time_delta) {
    for (auto& it : update_priorities_) {
        systems_[it.second]->Update(*entity_manager_, time_delta);
    }
}
}  // namespace ECS
