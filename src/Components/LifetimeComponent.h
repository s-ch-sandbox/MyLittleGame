#pragma once
#include "../ECS/Component.h"

struct LifetimeComponent : public ECS::Component<LifetimeComponent> {
    LifetimeComponent(float lifetime) : time_remaining_(lifetime) {}
    float time_remaining_;
};
