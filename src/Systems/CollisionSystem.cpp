#include "CollisionSystem.h"

#include "../Components/ColliderComponent.h"
#include "../Components/DistanceLimitComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/EntityManager.h"
#include "../EntityGroups.h"
#include "../Game.h"

CollisionSystem::CollisionSystem(SDLProxy::SDLMixChunkSharedPtr zombie_death_sound, const std::string& rock_tag,
                                 const std::string& water_tag)
    : zombie_death_sound_(zombie_death_sound), rock_tag_(rock_tag), water_tag_(water_tag) {}

void CollisionSystem::Update(ECS::EntityManager& entity_manager, float time_delta) {
    entity_manager.ForEachWith<ColliderComponent, TransformComponent>(
        [](ECS::Entity& entity, ColliderComponent& collider, TransformComponent& transform) {
            collider.collider_bbox_ = transform.bbox_;
        });
    UpdatePlayerEnemyCollisions(entity_manager, time_delta);
    UpdateEnemyStoneCollisions(entity_manager, time_delta);
    UpdateTileCollisions(entity_manager, time_delta);
}

void CollisionSystem::UpdatePlayerEnemyCollisions(ECS::EntityManager& entity_manager, float time_delta) {
    auto& player_group = entity_manager.GetGroup(GroupID::PLAYER);
    if (player_group.empty()) {
        return;
    }
    ECS::Entity* player = *player_group.begin();
    for (ECS::Entity* e : entity_manager.GetGroup(GroupID::ENEMY)) {
        if (e->HasComponent<ColliderComponent>()) {
            auto e_collider_bbox = e->GetComponent<ColliderComponent>().collider_bbox_;
            if (player->GetComponent<ColliderComponent>().collider_bbox_.IsIntersecting(e_collider_bbox)) {
                player->Destroy();
            }
        }
    }
}

void CollisionSystem::UpdateEnemyStoneCollisions(ECS::EntityManager& entity_manager, float time_delta) {
    for (auto& enemy : entity_manager.GetGroup(GroupID::ENEMY)) {
        ColliderComponent& enemy_collider = enemy->GetComponent<ColliderComponent>();
        for (auto& stone : entity_manager.GetGroup(GroupID::STONE)) {
            if (enemy_collider.collider_bbox_.IsIntersecting(stone->GetComponent<ColliderComponent>().collider_bbox_)) {
                enemy->Destroy();
                stone->Destroy();
                Game::GetInstance()->GetAudio().PlayChunk(zombie_death_sound_);
            }
        }
    }
}

void CollisionSystem::UpdateTileCollisions(ECS::EntityManager& entity_manager, float time_delta) {
    for (auto& enemy : entity_manager.GetGroup(GroupID::ENEMY)) {
        ColliderComponent& enemy_collider = enemy->GetComponent<ColliderComponent>();
        for (auto& tile : entity_manager.GetGroup(GroupID::MAP)) {
            if (tile->HasComponent<ColliderComponent>()) {
                ColliderComponent& tile_collider = tile->GetComponent<ColliderComponent>();
                if (tile_collider.collider_bbox_.IsIntersecting(enemy_collider.collider_bbox_)) {
                    if (tile_collider.tag_ == rock_tag_) {
                        TransformComponent& enemy_transform = enemy->GetComponent<TransformComponent>();
                        enemy_transform.velocity_ = -enemy_transform.velocity_;
                    } else if (tile_collider.tag_ == water_tag_) {
                        enemy->Destroy();
                    }
                }
            }
        }
    }

    for (auto& player : entity_manager.GetGroup(GroupID::PLAYER)) {
        ColliderComponent& player_collider = player->GetComponent<ColliderComponent>();
        for (auto& tile : entity_manager.GetGroup(GroupID::MAP)) {
            if (tile->HasComponent<ColliderComponent>()) {
                ColliderComponent& tile_collider = tile->GetComponent<ColliderComponent>();
                if (tile_collider.collider_bbox_.IsIntersecting(player_collider.collider_bbox_)) {
                    if (tile_collider.tag_ == rock_tag_) {
                        TransformComponent& player_transform = player->GetComponent<TransformComponent>();
                        player_transform.bbox_.top_left_ -=
                            player_transform.velocity_.GetNormalized() * player_transform.speed_ * time_delta / 1000.f;
                    } else if (tile_collider.tag_ == water_tag_) {
                        player->Destroy();
                    }
                }
            }
        }
    }

    for (auto& stone : entity_manager.GetGroup(GroupID::STONE)) {
        ColliderComponent& stone_collider = stone->GetComponent<ColliderComponent>();
        for (auto& tile : entity_manager.GetGroup(GroupID::MAP)) {
            if (tile->HasComponent<ColliderComponent>()) {
                ColliderComponent& tile_collider = tile->GetComponent<ColliderComponent>();
                if (tile_collider.collider_bbox_.IsIntersecting(stone_collider.collider_bbox_) &&
                    tile_collider.tag_ == rock_tag_) {
                    stone->GetComponent<DistanceLimitComponent>().distance_remaining_ = 0.f;
                }
            }
        }
    }
}
