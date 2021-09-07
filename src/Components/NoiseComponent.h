#pragma once
#include "../ECS/Component.h"

struct NoiseComponent : public ECS::Component<NoiseComponent> {
    NoiseComponent(float intensity = 0.f) : intensity_(intensity) {}
    float intensity_;
};
