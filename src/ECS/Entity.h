#pragma once
#include <memory>
#include <unordered_map>

#include "Component.h"

namespace ECS {
class Entity {
   public:
    using ID = uintptr_t;
    Entity();
    bool IsActive() const;
    void Destroy();
    void SetActive();
    ID GetID() const;

    template <typename T>
    bool HasComponent() const;
    template <typename TComponent, typename... TArgs>
    TComponent& AddComponent(TArgs&&... args);
    template <typename T>
    T& GetComponent();

   private:
    bool active_ = true;
    std::unordered_map<BaseComponent::ComponentID, std::unique_ptr<BaseComponent>> components_;
    const ID id_;
};

template <typename T>
bool Entity::HasComponent() const {
    return components_.find(T::GetComponentTypeID()) != components_.end();
}

template <typename TComponent, typename... TArgs>
TComponent& Entity::AddComponent(TArgs&&... args) {
    auto it = components_.emplace(TComponent::GetComponentTypeID(),
                                  std::make_unique<TComponent>(std::forward<TArgs>(args)...));
    if (!it.second) {
        throw std::exception("Couldn't add component to entity!");
    }
    return *static_cast<TComponent*>(it.first->second.get());
}

template <typename T>
T& Entity::GetComponent() {
    auto it = components_.find(T::GetComponentTypeID());
    if (it == components_.end()) {
        throw std::exception("Component not found!");
    }
    return *static_cast<T*>(it->second.get());
}
}  // namespace ECS
