#include "MouseControllerSystem.h"

#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"
#include "../EntityGroups.h"
#include "../Game.h"
#include "SDL.h"

void MouseControllerSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    auto& player_group = entity_manager.GetGroup(GroupID::PLAYER);
    if (player_group.empty()) {
        return;
    }
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    utils::Rectangle<int> camera = Game::GetInstance()->GetDisplay().GetCamera();
    utils::Vec2Df mouse_coords(static_cast<float>(mouse_x + camera.top_left_.x),
                               static_cast<float>(mouse_y + camera.top_left_.y));
    TransformComponent& transform = (**player_group.begin()).GetComponent<TransformComponent>();
    transform.orientation_ = (mouse_coords - transform.bbox_.GetCenterPoint()).GetNormalized();
}
