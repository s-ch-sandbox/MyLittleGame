#include "Display.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include "easylogging++.h"

Display::Display(const WindowParameters& window_params)
    : window_(nullptr),
      renderer_(nullptr),
      camera_window_{utils::Vector2D<int>(0, 0), window_params.width, window_params.height} {
    Uint32 flags = 0;
    if (window_params.fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    LOG(INFO) << "Subsytem is initialized.";

    window_.reset(SDL_CreateWindow(window_params.title.c_str(), window_params.xpos, window_params.ypos,
                                   window_params.width, window_params.height, flags));
    if (!window_) {
        throw std::exception("Error when creating window.");
    }
    LOG(INFO) << "Window is created.";

    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, 0));
    if (!renderer_) {
        throw std::exception("Error when creating renderer.");
    }
    LOG(INFO) << "Renderer is created.";

    if (SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255) != 0) {
        throw std::exception("Error when rendering background color.");
    }
}

void Display::Clear() { SDL_RenderClear(renderer_.get()); }

void Display::Present() { SDL_RenderPresent(renderer_.get()); }

void Display::Draw(SDLProxy::SDLTextureSharedPtr texture, const utils::Rectangle<int>& src_rect,
                   const utils::Rectangle<int>& dst_rect, bool flip) {
    auto vertices = dst_rect.GetVertices();
    if (camera_window_.IsCovering(vertices[0]) || camera_window_.IsCovering(vertices[1]) ||
        camera_window_.IsCovering(vertices[2]) || camera_window_.IsCovering(vertices[3])) {
        utils::Rectangle<int> pane_rect = dst_rect;
        pane_rect.top_left_ -= camera_window_.top_left_;

        SDL_Rect src_rect_tmp{src_rect.top_left_.x, src_rect.top_left_.y, src_rect.width_, src_rect.height_};
        SDL_Rect* src_rect_tmp_ptr = src_rect_tmp.w || src_rect_tmp.h ? &src_rect_tmp : NULL;
        SDL_Rect dst_rect_tmp{pane_rect.top_left_.x, pane_rect.top_left_.y, pane_rect.width_, pane_rect.height_};
        SDL_RendererFlip sdl_flip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer_.get(), texture.get(), src_rect_tmp_ptr, &dst_rect_tmp, 0, NULL, sdl_flip);
    }
}

void Display::DrawMessage(const std::string& message, SDLProxy::SDLFontSharedPtr font, int r, int g, int b,
                          const utils::Vector2D<int> dst_position) {
    SDL_Color color{static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), 0};
    int w, h;
    TTF_SizeText(font.get(), message.c_str(), &w, &h);
    SDL_Surface* surface_message = TTF_RenderText_Solid(font.get(), message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_.get(), surface_message);
    SDL_Rect dst_rect_tmp{static_cast<Uint8>(dst_position.x), static_cast<Uint8>(dst_position.y), w, h};
    SDL_RenderCopy(renderer_.get(), texture, NULL, &dst_rect_tmp);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface_message);
}

utils::Rectangle<int>& Display::GetCamera() { return camera_window_; }
