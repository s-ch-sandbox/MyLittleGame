#include "TransformSystem.h"

#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"

TransformSystem::TransformSystem(float x_limit, float y_limit) : x_limit_(x_limit), y_limit_(y_limit) {}

void TransformSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    entity_manager.ForEachWith<TransformComponent>(
        [time_delta, this](ECS::Entity& entity, TransformComponent& transform) {
            transform.bbox_.top_left_ += transform.velocity_.GetNormalized() * transform.speed_ * time_delta / 1000.f;
            if (transform.bbox_.top_left_.x < 0.f) {
                transform.bbox_.top_left_.x = 0.f;
            }
            if (transform.bbox_.top_left_.y < 0.f) {
                transform.bbox_.top_left_.y = 0.f;
            }
            if (transform.bbox_.top_left_.x + transform.bbox_.width_ > x_limit_) {
                transform.bbox_.top_left_.x = x_limit_ - transform.bbox_.width_;
            }
            if (transform.bbox_.top_left_.y + transform.bbox_.height_ > y_limit_) {
                transform.bbox_.top_left_.y = y_limit_ - transform.bbox_.height_;
            }
        });
}
