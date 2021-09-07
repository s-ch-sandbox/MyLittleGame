#include "EnemyMovementSystem.h"

#include <algorithm>
#include <vector>

#include "../Components/NoiseComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../EntityGroups.h"
#include "../Game.h"

static const float zombie_sound_threshold_angle = 0.7f;

EnemyMovementSystem::EnemyMovementSystem(float scan_radius, SDLProxy::SDLMixChunkSharedPtr zombie_attention_sound)
    : scan_radius_(scan_radius), zombie_attention_sound_(zombie_attention_sound) {}

void EnemyMovementSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    auto& enemy_group = entity_manager.GetGroup(GroupID::ENEMY);
    if (enemy_group.empty()) {
        return;
    }
    struct NoiseSource {
        utils::Vec2Df position;
        float intensity;
    };
    std::vector<NoiseSource> noise_sources;
    entity_manager.ForEachWith<TransformComponent, NoiseComponent>(
        [&noise_sources](ECS::Entity& e, TransformComponent& transform, NoiseComponent& noise) {
            noise_sources.push_back({transform.bbox_.GetCenterPoint(), noise.intensity_});
        });
    if (noise_sources.empty()) {
        return;
    }
    float scan_radius = scan_radius_;
    for (auto& entity : enemy_group) {
        TransformComponent& transform = entity->GetComponent<TransformComponent>();
        utils::Vec2Df entity_center = transform.bbox_.GetCenterPoint();
        std::sort(noise_sources.begin(), noise_sources.end(),
                  [&entity_center, scan_radius](const NoiseSource& x, const NoiseSource& y) {
                      return x.intensity * static_cast<float>((x.position - entity_center).GetNorm() < scan_radius) >
                             y.intensity * static_cast<float>((y.position - entity_center).GetNorm() < scan_radius);
                  });
        NoiseSource most_effective_source = *noise_sources.begin();
        if (most_effective_source.intensity > 0.f &&
            (most_effective_source.position - entity_center).GetNorm() < scan_radius) {
            utils::Vec2Df new_velocity = most_effective_source.position - entity_center;
            if ((fabsf(transform.velocity_.Dot(new_velocity)) < zombie_sound_threshold_angle) ||
                (transform.velocity_ == utils::Vec2Df())) {
                Game::GetInstance()->GetAudio().PlayChunk(zombie_attention_sound_);
            }
            transform.velocity_ = new_velocity;
        }
    }
}
