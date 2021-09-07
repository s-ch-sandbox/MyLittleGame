#pragma once
#include "../ECS/System.h"
#include "../utils/SDLProxy.h"

class EnemyMovementSystem : public ECS::System<EnemyMovementSystem> {
   public:
    EnemyMovementSystem(float scan_radius, SDLProxy::SDLMixChunkSharedPtr zombie_attention_sound);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
    float scan_radius_;
    SDLProxy::SDLMixChunkSharedPtr zombie_attention_sound_;
};
