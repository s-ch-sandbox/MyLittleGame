#pragma once
#include <stdint.h>

#include "../ECS/System.h"
#include "../utils/Rectangle.h"
#include "../utils/SDLProxy.h"

class KeyboardControllerSystem : public ECS::System<KeyboardControllerSystem> {
   public:
    struct StoneEntityParameters {
        float width;
        float height;
        float speed;
        SDLProxy::SDLTextureSharedPtr texture;
        utils::Rectangle<unsigned int> src_rect;
        bool animated;
        unsigned int frames;
        float frame_duration;
    };

    KeyboardControllerSystem(const StoneEntityParameters& stone_params, float max_attack_distance, float attack_divisor,
                             SDLProxy::SDLMixChunkSharedPtr stone_throw_sound);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
    void UpdateMovement(ECS::EntityManager& entity_manager, float time_delta, const uint8_t* keyboard_state) const;
    void UpdateAttack(ECS::EntityManager& entity_manager, float time_delta, const uint8_t* keyboard_state) const;
    void UpdateShutdown(const uint8_t* keyboard_state) const;

    StoneEntityParameters stone_params_;
    float max_attack_distance_;
    float attack_divisor_;
    SDLProxy::SDLMixChunkSharedPtr stone_throw_sound_;
};
