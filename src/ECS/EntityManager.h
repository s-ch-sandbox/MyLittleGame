#pragma once
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Entity.h"

namespace ECS {
class Entity;

class EntityManager {
   public:
    using GroupID = unsigned int;
    void Refresh();
    void AddToGroup(Entity::ID entity_id, GroupID group_id);
    std::unordered_set<Entity*>& GetGroup(GroupID group_id);
    Entity& AddEntity();
    Entity& GetEntity(Entity::ID) const;
    template <typename... TComps>
    void ForEachWith(std::function<void(Entity&, TComps&...)> f);

   private:
    std::unordered_map<Entity::ID, std::unique_ptr<Entity>> entities_;
    std::unordered_map<GroupID, std::unordered_set<Entity*>> entity_groups_;
};

template <typename... TComps>
void EntityManager::ForEachWith(std::function<void(Entity&, TComps&...)> f) {
    for (auto& it : entities_) {
        std::array<bool, sizeof...(TComps)> has_comp{it.second->HasComponent<TComps>()...};
        if (std::all_of(has_comp.begin(), has_comp.end(), [](bool i) { return i; })) {
            f(*it.second, it.second->GetComponent<TComps>()...);
        }
    }
}
}  // namespace ECS
