#include "Entity.h"

namespace ECS {
Entity::Entity() : id_(reinterpret_cast<Entity::ID>(this)) {}

Entity::ID Entity::GetID() const { return id_; }

bool Entity::IsActive() const { return active_; }

void Entity::Destroy() { active_ = false; }
void Entity::SetActive() { active_ = true; }
}  // namespace ECS
