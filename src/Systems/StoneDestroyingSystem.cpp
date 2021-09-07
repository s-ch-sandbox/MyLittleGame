#include "StoneDestroyingSystem.h"

#include "../Components/DistanceLimitComponent.h"
#include "../Components/LifetimeComponent.h"
#include "../Components/NoiseComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../EntityGroups.h"
#include "../Game.h"

StoneDestroyingSystem::StoneDestroyingSystem(float noise_intensity, float noise_duration,
                                             SDLProxy::SDLMixChunkSharedPtr fallen_stone_sound)
    : noise_intensity_(noise_intensity), noise_duration_(noise_duration), fallen_stone_sound_(fallen_stone_sound) {}

void StoneDestroyingSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    auto& stone_group = entity_manager.GetGroup(GroupID::STONE);
    if (stone_group.empty()) {
        return;
    }
    for (auto& entity : stone_group) {
        if (entity->GetComponent<DistanceLimitComponent>().distance_remaining_ <= 0.f) {
            utils::Vec2Df position = entity->GetComponent<TransformComponent>().bbox_.GetCenterPoint();
            entity->Destroy();
            ECS::Entity& noise_entity = entity_manager.AddEntity();
            noise_entity.AddComponent<TransformComponent>(utils::Rectangle<float>(position, 0.f, 0.f));
            noise_entity.AddComponent<NoiseComponent>(noise_intensity_);
            noise_entity.AddComponent<LifetimeComponent>(noise_duration_);
            Game::GetInstance()->GetAudio().PlayChunk(fallen_stone_sound_);
        }
    }
}
