#include "EntityManager.h"

#include <algorithm>
#include <experimental/unordered_map>
#include <experimental/unordered_set>

#include "easylogging++.h"

namespace ECS {
void EntityManager::Refresh() {
    for (auto& it : entity_groups_) {
        std::experimental::erase_if(it.second, [](const Entity* entity) { return !entity->IsActive(); });
    }
    std::experimental::erase_if(entities_, [](const auto& it) { return !it.second->IsActive(); });
}

void EntityManager::AddToGroup(Entity::ID entity_id, GroupID group_id) {
    entity_groups_[group_id].insert(&GetEntity(entity_id));
}

std::unordered_set<Entity*>& EntityManager::GetGroup(GroupID group_id) {
    auto it = entity_groups_.find(group_id);
    if (it == entity_groups_.end()) {
        LOG(WARNING) << "Group (" << group_id << ") is not found! Creating a new group";
        return entity_groups_[group_id];
    }
    return it->second;
}

Entity& EntityManager::AddEntity() {
    auto entity_ptr = std::make_unique<Entity>();
    auto it = entities_.emplace(entity_ptr->GetID(), std::move(entity_ptr));
    if (!it.second) {
        throw std::exception("Couldn't add entity!");
    }
    return *it.first->second;
}

Entity& EntityManager::GetEntity(Entity::ID id) const {
    auto it = entities_.find(id);
    if (it == entities_.end()) {
        throw std::exception("Entity not found!");
    }
    return *it->second;
}
}  // namespace ECS
