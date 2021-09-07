#pragma once
#include "../ECS/System.h"

class LifetimeSystem : public ECS::System<LifetimeSystem> {
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
};
