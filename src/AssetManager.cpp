#include "AssetManager.h"

#include "Display.h"
#include "Game.h"

SDLProxy::SDLTextureSharedPtr AssetManager::GetTexture(const std::string& path) {
    auto it = textures_.find(path);
    if (it != textures_.end()) {
        return it->second;
    }
    SDLProxy::SDLTextureSharedPtr texture =
        SDLProxy::LoadTexture(path, Game::GetInstance()->GetDisplay().renderer_.get());
    auto result = textures_.emplace(path, texture);
    if (!result.second) {
        throw std::exception("Couldn't store texture!");
    }
    return result.first->second;
}

SDLProxy::SDLMixMusicSharedPtr AssetManager::GetMusic(const std::string& path) {
    auto it = musics_.find(path);
    if (it != musics_.end()) {
        return it->second;
    }
    SDLProxy::SDLMixMusicSharedPtr music = SDLProxy::LoadMusic(path);
    auto result = musics_.emplace(path, music);
    if (!result.second) {
        throw std::exception("Couldn't store music!");
    }
    return result.first->second;
}

SDLProxy::SDLMixChunkSharedPtr AssetManager::GetChunk(const std::string& path) {
    auto it = chunks_.find(path);
    if (it != chunks_.end()) {
        return it->second;
    }
    SDLProxy::SDLMixChunkSharedPtr chunk = SDLProxy::LoadChunk(path);
    auto result = chunks_.emplace(path, chunk);
    if (!result.second) {
        throw std::exception("Couldn't store chunk!");
    }
    return result.first->second;
}

SDLProxy::SDLFontSharedPtr AssetManager::GetFont(const std::string& path, int pt_size) {
    auto it = fonts_.find(path);
    if (it != fonts_.end()) {
        return it->second;
    }
    SDLProxy::SDLFontSharedPtr font = SDLProxy::LoadFont(path, pt_size);
    auto result = fonts_.emplace(path, font);
    if (!result.second) {
        throw std::exception("Couldn't store font!");
    }
    return result.first->second;
}
