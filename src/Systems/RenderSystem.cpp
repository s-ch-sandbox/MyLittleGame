#include "RenderSystem.h"

#include <sstream>

#include "../Components/RenderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../EntityGroups.h"
#include "../Game.h"

RenderSystem::RenderSystem(const std::vector<ECS::EntityManager::GroupID>& drawing_order,
                           SDLProxy::SDLFontSharedPtr fps_font, SDLProxy::SDLFontSharedPtr gameover_font,
                           const Color& fps_color, const Color& gameover_color,
                           const utils::Vector2D<int>& fps_position, const utils::Vector2D<int>& gameover_position,
                           float fps_draw_period)
    : drawing_order_(drawing_order),
      fps_font_(fps_font),
      gameover_font_(gameover_font),
      fps_color_(fps_color),
      gameover_color_(gameover_color),
      fps_position_(fps_position),
      gameover_position_(gameover_position),
      fps_draw_period_(fps_draw_period) {}

void RenderSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    Game::GetInstance()->GetDisplay().Clear();
    RenderGroups(entity_manager, time_delta);
    RenderFPS(time_delta);
    RenderGameover(entity_manager);
    Game::GetInstance()->GetDisplay().Present();
}

void RenderSystem::RenderGroups(ECS::EntityManager& entity_manager, float time_delta) {
    for (ECS::EntityManager::GroupID group_id : drawing_order_) {
        for (auto& entity : entity_manager.GetGroup(group_id)) {
            if (entity->HasComponent<RenderComponent>() && entity->HasComponent<TransformComponent>()) {
                auto& render = entity->GetComponent<RenderComponent>();
                auto& transform = entity->GetComponent<TransformComponent>();
                if (render.animated_) {
                    render.time_since_change_ += time_delta;
                    if (render.time_since_change_ > render.frame_duration_) {
                        render.current_frame_ =
                            (render.current_frame_ + static_cast<unsigned int>(std::floorf(render.time_since_change_ /
                                                                                           render.frame_duration_))) %
                            render.frames_;
                        render.src_rect_.top_left_.x = render.src_rect_.width_ * render.current_frame_;
                        render.time_since_change_ = 0.f;
                    }
                }

                Game::GetInstance()->GetDisplay().Draw(render.texture_, render.src_rect_, transform.bbox_,
                                                       transform.velocity_.x < 0.f);
            }
        }
    }
}

void RenderSystem::RenderFPS(float time_delta) {
    time_since_fps_draw_ += time_delta;
    ++fps_updates_;
    if (time_since_fps_draw_ > fps_draw_period_) {
        fps_ = static_cast<float>(fps_updates_) / time_since_fps_draw_ * 1000.f;
        fps_updates_ = 0;
        time_since_fps_draw_ = 0.f;
    }
    std::stringstream sstream;
    sstream << "FPS: " << static_cast<int>(fps_);
    Game::GetInstance()->GetDisplay().DrawMessage(sstream.str(), fps_font_, fps_color_.r, fps_color_.g, fps_color_.b,
                                                  fps_position_);
}

void RenderSystem::RenderGameover(ECS::EntityManager& entity_manager) {
    if (entity_manager.GetGroup(GroupID::PLAYER).empty()) {
        Game::GetInstance()->GetDisplay().DrawMessage("Game Over!", gameover_font_, gameover_color_.r,
                                                      gameover_color_.g, gameover_color_.b, gameover_position_);
    }
}
