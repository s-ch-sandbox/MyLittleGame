#pragma once
#include <random>

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "../utils/Rectangle.h"
#include "../utils/SDLProxy.h"

class EnemySpawnSystem : public ECS::System<EnemySpawnSystem> {
   public:
    struct EnemyEntityParameters {
        utils::Rectangle<float> spawn_area;
        float enemy_width;
        float enemy_height;
        float enemy_speed;
        SDLProxy::SDLTextureSharedPtr texture;
        utils::Rectangle<unsigned int> src_rect;
        bool animated;
        unsigned int frames;
        float frame_duration;
    };

    EnemySpawnSystem(const EnemyEntityParameters& entity_params, float spawn_period,
                     SDLProxy::SDLMixChunkSharedPtr zombie_born_sound);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;

    const float spawn_period_;  // ms
    float time_since_spawn_;    // ms
    std::mt19937 generator_;
    std::uniform_real_distribution<float> distribution_x_;
    std::uniform_real_distribution<float> distribution_y_;
    const EnemyEntityParameters entity_params_;
    SDLProxy::SDLMixChunkSharedPtr zombie_born_sound_;
};
