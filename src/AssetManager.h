#pragma once
#include <string>
#include <unordered_map>

#include "utils/SDLProxy.h"

class AssetManager {
   public:
    SDLProxy::SDLTextureSharedPtr GetTexture(const std::string& path);
    SDLProxy::SDLMixMusicSharedPtr GetMusic(const std::string& path);
    SDLProxy::SDLMixChunkSharedPtr GetChunk(const std::string& path);
    SDLProxy::SDLFontSharedPtr GetFont(const std::string& path, int pt_size);

   private:
    std::unordered_map<std::string, SDLProxy::SDLTextureSharedPtr> textures_;
    std::unordered_map<std::string, SDLProxy::SDLMixMusicSharedPtr> musics_;
    std::unordered_map<std::string, SDLProxy::SDLMixChunkSharedPtr> chunks_;
    std::unordered_map<std::string, SDLProxy::SDLFontSharedPtr> fonts_;
};
