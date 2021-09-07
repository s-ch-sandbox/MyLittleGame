#pragma once
#include <string>
#include <unordered_map>

#include "ECS/EntityManager.h"
#include "utils/SDLProxy.h"

class Tilemap {
   public:
    Tilemap(ECS::EntityManager& entity_manager, const std::string& path,
            const std::unordered_map<unsigned int, SDLProxy::SDLTextureSharedPtr>& tiletype2texture,
            const std::unordered_map<unsigned int, std::string>& tiletype2tag);
    template <typename T>
    void GetWorldSize(T* width, T* height) const;

   private:
    unsigned int tile_width_;
    unsigned int tile_height_;
    unsigned int num_tiles_x_;
    unsigned int num_tiles_y_;
};

template <typename T>
void Tilemap::GetWorldSize(T* width, T* height) const {
    *width = static_cast<T>(tile_width_ * num_tiles_x_);
    *height = static_cast<T>(tile_height_ * num_tiles_y_);
}
