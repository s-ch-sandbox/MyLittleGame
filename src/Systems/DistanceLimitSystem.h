#pragma once
#include "../ECS/System.h"

class DistanceLimitSystem : public ECS::System<DistanceLimitSystem> {
    void Update(ECS::EntityManager& entity_manager, float time_delta);
};
