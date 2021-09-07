#pragma once
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "../utils/Rectangle.h"
#include "../utils/SDLProxy.h"

class CollisionSystem : public ECS::System<CollisionSystem> {
   public:
    CollisionSystem(SDLProxy::SDLMixChunkSharedPtr zombie_death_sound, const std::string& rock_tag,
                    const std::string& water_tag);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
    void UpdatePlayerEnemyCollisions(ECS::EntityManager& entity_manager, float time_delta);
    void UpdateEnemyStoneCollisions(ECS::EntityManager& entity_manager, float time_delta);
    void UpdateTileCollisions(ECS::EntityManager& entity_manager, float time_delta);
    SDLProxy::SDLMixChunkSharedPtr zombie_death_sound_;
    std::string rock_tag_;
    std::string water_tag_;
};
