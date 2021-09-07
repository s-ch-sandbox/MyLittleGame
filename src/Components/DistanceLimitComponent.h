#pragma once
#include "../ECS/Component.h"

struct DistanceLimitComponent : public ECS::Component<DistanceLimitComponent> {
    DistanceLimitComponent(float distance_limit) : distance_remaining_(distance_limit) {}
    float distance_remaining_;
};
