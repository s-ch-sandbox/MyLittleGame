#pragma once
#include <string>

#include "../ECS/Component.h"
#include "../utils/Rectangle.h"
#include "../utils/SDLProxy.h"

struct RenderComponent : public ECS::Component<RenderComponent> {
    SDLProxy::SDLTextureSharedPtr texture_;
    utils::Rectangle<unsigned int> src_rect_;
    bool animated_;
    unsigned int frames_;
    unsigned int current_frame_;
    float frame_duration_;     // in milliseconds
    float time_since_change_;  // time elapsed since the last frame changing

    RenderComponent(const SDLProxy::SDLTextureSharedPtr& texture = nullptr,
                    const utils::Rectangle<unsigned int>& src_rect = {}, bool animated = false, unsigned int frames = 0,
                    float frame_duration = 0.f)
        : texture_(texture),
          src_rect_(src_rect),
          animated_(animated),
          frames_(frames),
          frame_duration_(frame_duration),
          time_since_change_(0.f),
          current_frame_(0u) {}
};
