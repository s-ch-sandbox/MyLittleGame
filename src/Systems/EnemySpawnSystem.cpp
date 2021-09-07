#include "EnemySpawnSystem.h"

#include "../Components/ColliderComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/TransformComponent.h"
#include "../EntityGroups.h"
#include "../Game.h"

EnemySpawnSystem::EnemySpawnSystem(const EnemyEntityParameters& entity_params, float spawn_period,
                                   SDLProxy::SDLMixChunkSharedPtr zombie_born_sound)
    : spawn_period_(spawn_period),
      time_since_spawn_(0.f),
      generator_(std::random_device()()),
      distribution_x_(entity_params.spawn_area.top_left_.x,
                      entity_params.spawn_area.top_left_.x + entity_params.spawn_area.width_),
      distribution_y_(entity_params.spawn_area.top_left_.y,
                      entity_params.spawn_area.top_left_.y + entity_params.spawn_area.height_),
      entity_params_(entity_params),
      zombie_born_sound_(zombie_born_sound) {}

void EnemySpawnSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    time_since_spawn_ += time_delta;
    if (time_since_spawn_ > spawn_period_) {
        size_t num_enemies = static_cast<size_t>(time_since_spawn_ / spawn_period_);
        for (size_t i = 0u; i < num_enemies; ++i) {
            utils::Vec2Df entity_top_left(distribution_x_(generator_), distribution_y_(generator_));
            utils::Rectangle<float> entity_bbox({{distribution_x_(generator_), distribution_y_(generator_)},
                                                 entity_params_.enemy_width,
                                                 entity_params_.enemy_height});
            ECS::Entity& entity = entity_manager.AddEntity();
            entity_manager.AddToGroup(entity.GetID(), GroupID::ENEMY);
            TransformComponent& transform = entity.AddComponent<TransformComponent>(entity_bbox);
            transform.speed_ = entity_params_.enemy_speed;
            entity.AddComponent<RenderComponent>(entity_params_.texture, entity_params_.src_rect,
                                                 entity_params_.animated, entity_params_.frames,
                                                 entity_params_.frame_duration);
            entity.AddComponent<ColliderComponent>(transform.bbox_);
            Game::GetInstance()->GetAudio().PlayChunk(zombie_born_sound_);
        }
        time_since_spawn_ = 0.f;
    }
}
