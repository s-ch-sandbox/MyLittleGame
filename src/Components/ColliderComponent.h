#pragma once
#include <string>

#include "../ECS/Component.h"
#include "../utils/Rectangle.h"

struct ColliderComponent : public ECS::Component<ColliderComponent> {
    utils::Rectangle<float> collider_bbox_;
    std::string tag_;

    ColliderComponent(const utils::Rectangle<float>& bbox = {}, const std::string& tag = "")
        : collider_bbox_(bbox), tag_(tag) {}
};
