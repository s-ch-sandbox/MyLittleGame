#include "KeyboardControllerSystem.h"

#include "../Components/AttackComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/DistanceLimitComponent.h"
#include "../Components/NoiseComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/EntityManager.h"
#include "../EntityGroups.h"
#include "../Game.h"
#include "SDL.h"

KeyboardControllerSystem::KeyboardControllerSystem(const StoneEntityParameters& stone_params, float max_attack_distance,
                                                   float attack_divisor,
                                                   SDLProxy::SDLMixChunkSharedPtr stone_throw_sound)
    : stone_params_(stone_params),
      max_attack_distance_(max_attack_distance),
      attack_divisor_(attack_divisor),
      stone_throw_sound_(stone_throw_sound) {}

void KeyboardControllerSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    const uint8_t* keyboard_state = SDL_GetKeyboardState(NULL);
    UpdateShutdown(keyboard_state);
    UpdateMovement(entity_manager, time_delta, keyboard_state);
    UpdateAttack(entity_manager, time_delta, keyboard_state);
}

void KeyboardControllerSystem::UpdateMovement(ECS::EntityManager& entity_manager, float time_delta,
                                              const uint8_t* keyboard_state) const {
    auto& player_group = entity_manager.GetGroup(GroupID::PLAYER);
    if (player_group.empty()) {
        return;
    }
    ECS::Entity& entity = **player_group.begin();
    TransformComponent& transform = entity.GetComponent<TransformComponent>();

    transform.velocity_ = {0.f, 0.f};
    if (keyboard_state[SDL_SCANCODE_W]) {
        transform.velocity_.y = -1.f;
    }
    if (keyboard_state[SDL_SCANCODE_S]) {
        transform.velocity_.y = 1.f;
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
        transform.velocity_.x = -1.f;
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        transform.velocity_.x = 1.f;
    }

    NoiseComponent& noise = entity.GetComponent<NoiseComponent>();
    noise.intensity_ = (transform.velocity_.x != 0.f || transform.velocity_.y != 0.f) ? 1.f : 0.f;
}

void KeyboardControllerSystem::UpdateAttack(ECS::EntityManager& entity_manager, float time_delta,
                                            const uint8_t* keyboard_state) const {
    auto& player_group = entity_manager.GetGroup(GroupID::PLAYER);
    if (player_group.empty()) {
        return;
    }
    ECS::Entity& entity = **player_group.begin();
    TransformComponent& transform = entity.GetComponent<TransformComponent>();
    AttackComponent& attack = entity.GetComponent<AttackComponent>();

    if (keyboard_state[SDL_SCANCODE_SPACE]) {
        attack.value_ += time_delta;
    } else if (attack.value_ > 0.f) {
        ECS::Entity& stone_entity = entity_manager.AddEntity();
        utils::Vec2Df stone_velocity = transform.orientation_.GetNormalized();
        utils::Rectangle<float> bbox(
            {utils::Vec2Df{transform.bbox_.GetCenterPoint()}, stone_params_.width, stone_params_.height});
        stone_entity.AddComponent<TransformComponent>(bbox, stone_velocity, utils::Vec2Df(), stone_params_.speed);
        stone_entity.AddComponent<ColliderComponent>(bbox);
        stone_entity.AddComponent<RenderComponent>(stone_params_.texture, stone_params_.src_rect,
                                                   stone_params_.animated, stone_params_.frames,
                                                   stone_params_.frame_duration);
        float distance_limit = fminf(attack.value_ / attack_divisor_, max_attack_distance_);
        stone_entity.AddComponent<DistanceLimitComponent>(distance_limit);
        entity_manager.AddToGroup(stone_entity.GetID(), GroupID::STONE);
        attack.value_ = 0.f;
        Game::GetInstance()->GetAudio().PlayChunk(stone_throw_sound_);
    }
}

void KeyboardControllerSystem::UpdateShutdown(const uint8_t* keyboard_state) const {
    if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
        Game::GetInstance()->Shutdown();
    }
}
