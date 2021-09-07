#include "DistanceLimitSystem.h"

#include "../Components/DistanceLimitComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"

void DistanceLimitSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    entity_manager.ForEachWith<TransformComponent, DistanceLimitComponent>(
        [time_delta](ECS::Entity& entity, TransformComponent& transform, DistanceLimitComponent& dlimit) {
            if (dlimit.distance_remaining_ <= 0.f) {
                transform.velocity_.x = 0.f;
                transform.velocity_.y = 0.f;
            } else {
                dlimit.distance_remaining_ -= time_delta * transform.speed_ / 1000.f;
            }
        });
}
