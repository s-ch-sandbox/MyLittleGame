#include "LifetimeSystem.h"

#include "../Components/LifetimeComponent.h"
#include "../ECS/EntityManager.h"

void LifetimeSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    entity_manager.ForEachWith<LifetimeComponent>([time_delta](ECS::Entity& entity, LifetimeComponent& lifetime) {
        if (lifetime.time_remaining_ <= 0.f) {
            entity.Destroy();
        } else {
            lifetime.time_remaining_ -= time_delta;
        }
    });
}
