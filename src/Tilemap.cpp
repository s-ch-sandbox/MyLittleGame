#include "Tilemap.h"

#include <fstream>

#include "Components/ColliderComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "EntityGroups.h"
#include "Game.h"
#include "easylogging++.h"

Tilemap::Tilemap(ECS::EntityManager& entity_manager, const std::string& path,
                 const std::unordered_map<unsigned int, SDLProxy::SDLTextureSharedPtr>& tiletype2texture,
                 const std::unordered_map<unsigned int, std::string>& tiletype2tag) {
    LOG(INFO) << "Reading map (" << path << ")";
    std::fstream file;
    file.open(path);
    file >> num_tiles_x_ >> num_tiles_y_ >> tile_width_ >> tile_height_;
    LOG(INFO) << "Map size (tiles): [" << num_tiles_x_ << "x" << num_tiles_y_ << "]. Tile size: [" << tile_width_ << "x"
              << tile_height_ << "]";
    for (unsigned int y = 0; y < num_tiles_y_; ++y) {
        for (unsigned int x = 0; x < num_tiles_x_; ++x) {
            unsigned int tile_type;
            file >> tile_type;
            ECS::Entity& tile_entity = entity_manager.AddEntity();
            utils::Rectangle<float> tile_bbox(
                {static_cast<float>(x * tile_width_), static_cast<float>(y * tile_height_)},
                static_cast<float>(tile_width_), static_cast<float>(tile_height_));
            tile_entity.AddComponent<TransformComponent>(tile_bbox);
            tile_entity.AddComponent<RenderComponent>(tiletype2texture.find(tile_type)->second);
            if (tiletype2tag.find(tile_type) != tiletype2tag.end()) {
                tile_entity.AddComponent<ColliderComponent>(tile_bbox, tiletype2tag.find(tile_type)->second);
            }
            entity_manager.AddToGroup(tile_entity.GetID(), GroupID::MAP);
        }
    }
    file.close();
}
