#pragma once
#include <memory>
#include <string>

#include "utils/Rectangle.h"
#include "utils/SDLProxy.h"

struct WindowParameters {
    std::string title;
    int xpos;
    int ypos;
    int width;
    int height;
    bool fullscreen;
};

class Display {
   public:
    friend class AssetManager;
    explicit Display(const WindowParameters&);
    void Clear();
    void Present();
    void Draw(SDLProxy::SDLTextureSharedPtr texture, const utils::Rectangle<int>& src_rect,
              const utils::Rectangle<int>& dst_rect, bool flip = false);
    void DrawMessage(const std::string& message, SDLProxy::SDLFontSharedPtr font, int r, int g, int b,
                     const utils::Vector2D<int> dst_position);
    utils::Rectangle<int>& GetCamera();

   private:
    utils::Rectangle<int> camera_window_;
    SDLProxy::SDLWindowUniquePtr window_;
    SDLProxy::SDLRendererUniquePtr renderer_;
};
