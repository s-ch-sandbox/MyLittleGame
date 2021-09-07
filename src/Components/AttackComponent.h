#pragma once
#include "../ECS/Component.h"

struct AttackComponent : ECS::Component<AttackComponent> {
    float value_ = 0.f;  // how long attack key is being pressed (ms)
};
