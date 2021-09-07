#pragma once
#include <map>
#include <memory>
#include <unordered_map>

#include "System.h"

namespace ECS {
class EntityManager;

class SystemManager {
   public:
    SystemManager(EntityManager* entity_manager);
    void Update(float time_delta);
    template <class TSystem, class... TArgs>
    TSystem& AddSystem(TArgs&&... args);
    template <class T>
    void AssignPriority(int priority);
    template <class T>
    T& GetSystem() const;

   private:
    std::unordered_map<BaseSystem::SystemID, std::unique_ptr<BaseSystem>> systems_;
    EntityManager* const entity_manager_;
    std::multimap<int, BaseSystem::SystemID> update_priorities_;
};

template <class TSystem, class... TArgs>
TSystem& SystemManager::AddSystem(TArgs&&... args) {
    auto it = systems_.emplace(TSystem::GetSystemTypeID(), std::make_unique<TSystem>(std::forward<TArgs>(args)...));
    if (!it.second) {
        throw std::exception("Couldn't create system!");
    }
    update_priorities_.emplace(0, TSystem::GetSystemTypeID());
    return *static_cast<TSystem*>(it.first->second.get());
}

template <class T>
T& SystemManager::GetSystem() const {
    auto it = systems_.find(T::GetSystemTypeID());
    if (it == systems_.end()) {
        throw std::exception("System not found!");
    }
    return *static_cast<T*>(it->second.get());
}

template <class T>
void SystemManager::AssignPriority(int priority) {
    for (auto it = update_priorities_.begin(); it != update_priorities_.end(); ++it) {
        if (it->second == T::GetSystemTypeID()) {
            update_priorities_.erase(it);
            break;
        }
    }
    update_priorities_.emplace(priority, T::GetSystemTypeID());
}
}  // namespace ECS
