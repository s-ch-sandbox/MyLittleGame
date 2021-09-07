#pragma once
#include "../ECS/EntityManager.h"
#include "../ECS/System.h"
#include "../utils/SDLProxy.h"
#include "../utils/Vector2D.h"
#include "vector"

class RenderSystem : public ECS::System<RenderSystem> {
   public:
    struct Color {
        int r;
        int g;
        int b;
    };

    RenderSystem(const std::vector<ECS::EntityManager::GroupID>& drawing_order, SDLProxy::SDLFontSharedPtr fps_font,
                 SDLProxy::SDLFontSharedPtr gameover_font, const Color& fps_color, const Color& gameover_color,
                 const utils::Vector2D<int>& fps_position, const utils::Vector2D<int>& gameover_position,
                 float fps_draw_period);

   private:
    void Update(ECS::EntityManager& entity_manager, float time_delta) override;
    void RenderGroups(ECS::EntityManager& entity_manager, float time_delta);
    void RenderFPS(float time_delta);
    void RenderGameover(ECS::EntityManager& entity_manager);

    std::vector<ECS::EntityManager::GroupID> drawing_order_;
    SDLProxy::SDLFontSharedPtr fps_font_;
    utils::Vector2D<int> fps_position_;
    Color fps_color_;
    SDLProxy::SDLFontSharedPtr gameover_font_;
    utils::Vector2D<int> gameover_position_;
    Color gameover_color_;
    float fps_draw_period_;  // ms
    float time_since_fps_draw_;
    unsigned int fps_updates_;
    float fps_;
};
