#pragma once
#include "../ECS/Component.h"
#include "../utils/Rectangle.h"

struct TransformComponent : public ECS::Component<TransformComponent> {
    utils::Rectangle<float> bbox_;
    utils::Vec2Df velocity_;
    utils::Vec2Df orientation_;
    float speed_;

    TransformComponent(const utils::Rectangle<float>& bbox = {}, const utils::Vec2Df& velocity = {},
                       const utils::Vec2Df& orientation = {}, float speed = 0.f)
        : bbox_(bbox), velocity_(velocity), orientation_(orientation), speed_(speed) {}
};
