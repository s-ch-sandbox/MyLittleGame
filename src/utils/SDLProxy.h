#pragma once
#include <memory>
#include <string>

/* Wrappers to use SDL resources in RAII fashion way */

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct _Mix_Music;
struct Mix_Chunk;
struct _TTF_Font;

namespace SDLProxy {
struct SDL_Window_deleter {
    void operator()(SDL_Window*);
};

struct SDL_Renderer_deleter {
    void operator()(SDL_Renderer*);
};

using SDLWindowUniquePtr = std::unique_ptr<SDL_Window, SDL_Window_deleter>;
using SDLRendererUniquePtr = std::unique_ptr<SDL_Renderer, SDL_Renderer_deleter>;
using SDLTextureSharedPtr = std::shared_ptr<SDL_Texture>;
using SDLMixMusicSharedPtr = std::shared_ptr<_Mix_Music>;
using SDLMixChunkSharedPtr = std::shared_ptr<Mix_Chunk>;
using SDLFontSharedPtr = std::shared_ptr<_TTF_Font>;

SDLTextureSharedPtr LoadTexture(const std::string& filename, SDL_Renderer* renderer);
SDLMixMusicSharedPtr LoadMusic(const std::string& filename);
SDLMixChunkSharedPtr LoadChunk(const std::string& filename);
SDLFontSharedPtr LoadFont(const std::string& filename, int pt_size);
}  // namespace SDLProxy
