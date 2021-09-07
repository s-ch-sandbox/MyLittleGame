#include "MapConstructor.h"

#include <fstream>

#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "EntityGroups.h"
#include "Game.h"
#include "easylogging++.h"

static const std::string tile_type2texture_path[] = {"assets/water.png", "assets/dirt.png"};
static const unsigned int texture_size_x = 12u;
static const unsigned int texture_size_y = 12u;

void LoadMap(ECS::EntityManager& entity_manager, const std::string& path, size_t* size_x, size_t* size_y) {
    LOG(INFO) << "Reading map (" << path << ")";
    std::fstream file;
    file.open(path);
    size_t tile_size_x = 0;
    size_t tile_size_y = 0;
    size_t tiles_x = 0;
    size_t tiles_y = 0;
    file >> tiles_x >> tiles_y >> tile_size_x >> tile_size_y;
    *size_x = tiles_x * tile_size_x;
    *size_y = tiles_y * tile_size_y;
    LOG(INFO) << "Map size (tiles): [" << tiles_x << "x" << tiles_y << "]. Tile size: [" << tile_size_x << "x"
              << tile_size_y << "]";
    for (size_t y = 0u; y < tiles_y; ++y) {
        for (size_t x = 0u; x < tiles_x; ++x) {
            size_t tile_type;
            file >> tile_type;
            ECS::Entity& tile_entity = entity_manager.AddEntity();
            utils::Rectangle<float> tile_bbox(
                {static_cast<float>(x * tile_size_x), static_cast<float>(y * tile_size_y)},
                static_cast<float>(tile_size_x), static_cast<float>(tile_size_y));
            tile_entity.AddComponent<TransformComponent>(tile_bbox);
            auto texture = Game::GetInstance()->GetAssetManager().GetTexture(tile_type2texture_path[tile_type]);
            tile_entity.AddComponent<RenderComponent>(
                texture, utils::Rectangle<unsigned int>({0u, 0u}, texture_size_x, texture_size_y));
            entity_manager.AddToGroup(tile_entity.GetID(), GroupID::MAP);
        }
    }
    file.close();
}
