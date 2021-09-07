#include "SDLProxy.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "easylogging++.h"

namespace SDLProxy {
void SDL_Window_deleter::operator()(SDL_Window* ptr) { SDL_DestroyWindow(ptr); }

void SDL_Renderer_deleter::operator()(SDL_Renderer* ptr) { SDL_DestroyRenderer(ptr); }

SDLTextureSharedPtr LoadTexture(const std::string& filename, SDL_Renderer* renderer) {
    LOG(INFO) << "Loading texture (" << filename << ")";
    SDL_Surface* tmpSurface = IMG_Load(filename.c_str());
    if (!tmpSurface) {
        LOG(INFO) << "Texture not found! (" << filename << ")";
        throw std::exception("Couldn't find texture");
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    return SDLTextureSharedPtr(tex, SDL_DestroyTexture);
}

SDLMixMusicSharedPtr LoadMusic(const std::string& filename) {
    LOG(INFO) << "Loading music (" << filename << ")";
    Mix_Music* music = Mix_LoadMUS(filename.c_str());
    if (!music) {
        LOG(INFO) << "Music not found! (" << filename << ")";
        throw std::exception("Couldn't find music");
    }
    return SDLMixMusicSharedPtr(music, Mix_FreeMusic);
}

SDLMixChunkSharedPtr LoadChunk(const std::string& filename) {
    LOG(INFO) << "Loading music (" << filename << ")";
    Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
    if (!chunk) {
        LOG(INFO) << "Music not found! (" << filename << ")";
        throw std::exception("Couldn't find music");
    }
    return SDLMixChunkSharedPtr(chunk, Mix_FreeChunk);
}

SDLFontSharedPtr LoadFont(const std::string& filename, int pt_size) {
    LOG(INFO) << "Loading font (" << filename << ")";
    _TTF_Font* font = TTF_OpenFont(filename.c_str(), pt_size);
    if (!font) {
        LOG(INFO) << "Font not found! (" << filename << ")";
        throw std::exception("Couldn't find font");
    }
    return SDLFontSharedPtr(font, TTF_CloseFont);
}
}  // namespace SDLProxy
