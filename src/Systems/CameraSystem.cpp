#include "CameraSystem.h"

#include "../Components/TransformComponent.h"
#include "../Display.h"
#include "../EntityGroups.h"
#include "../Game.h"

CameraSystem::CameraSystem(int world_width, int world_height)
    : world_width_(world_width), world_height_(world_height) {}

void CameraSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    auto& player_group = entity_manager.GetGroup(GroupID::PLAYER);
    if (player_group.empty()) {
        return;
    }
    utils::Vec2Df player_center = (*player_group.begin())->GetComponent<TransformComponent>().bbox_.GetCenterPoint();
    utils::Rectangle<int>& camera = Game::GetInstance()->GetDisplay().GetCamera();
    camera.top_left_.x = static_cast<int>(player_center.x - camera.width_ * 0.5);
    camera.top_left_.y = static_cast<int>(player_center.y - camera.height_ * 0.5);
    if (camera.top_left_.x + camera.width_ > world_width_) {
        camera.top_left_.x = world_width_ - camera.width_;
    }
    if (camera.top_left_.y + camera.height_ > world_height_) {
        camera.top_left_.y = world_height_ - camera.height_;
    }
    if (camera.top_left_.x < 0) {
        camera.top_left_.x = 0;
    }
    if (camera.top_left_.y < 0) {
        camera.top_left_.y = 0;
    }
}
