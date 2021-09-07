#pragma once
#include "../ECS/System.h"
#include "../utils/SDLProxy.h"

class StoneDestroyingSystem : public ECS::System<StoneDestroyingSystem> {
   public:
    StoneDestroyingSystem(float noise_intensity, float noise_duration,
                          SDLProxy::SDLMixChunkSharedPtr fallen_stone_sound);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
    float noise_intensity_;
    float noise_duration_;
    SDLProxy::SDLMixChunkSharedPtr fallen_stone_sound_;
};
