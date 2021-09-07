#include "Game.h"

#include <chrono>

#include "Components/AttackComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/NoiseComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "EntityGroups.h"
#include "SDL.h"
#include "Systems/CameraSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/DistanceLimitSystem.h"
#include "Systems/EnemyMovementSystem.h"
#include "Systems/EnemySpawnSystem.h"
#include "Systems/KeyboardControllerSystem.h"
#include "Systems/LifetimeSystem.h"
#include "Systems/MouseControllerSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/StoneDestroyingSystem.h"
#include "Systems/TransformSystem.h"
#include "Tilemap.h"

static const float player_starting_position_x = 40.f;
static const float player_starting_position_y = 40.f;
static const float player_width = 70.f;
static const float player_height = 80.f;
static const float player_speed = 100.f;
static const char player_texture_path[] = "assets/player.png";
static const unsigned int player_texture_src_x = 0u;
static const unsigned int player_texture_src_y = 0u;
static const unsigned int player_texture_src_width = 128u;
static const unsigned int player_texture_src_height = 144u;
static const bool player_texture_animated = true;
static const unsigned int player_texture_frames = 4u;
static const float player_texture_frame_duration = 150.f;
static const float enemy_width = 70.f;
static const float enemy_height = 80.f;
static const float enemy_speed = 10.f;
static const char enemy_texture_path[] = "assets/enemy.png";
static const unsigned int enemy_texture_src_x = 0u;
static const unsigned int enemy_texture_src_y = 0u;
static const unsigned int enemy_texture_src_width = 128u;
static const unsigned int enemy_texture_src_height = 144u;
static const bool enemy_texture_animated = true;
static const unsigned int enemy_texture_frames = 6u;
static const float enemy_texture_frame_duration = 200.f;
static const float enemy_spawn_period = 3000.f;
static const float enemy_scan_radius = 500.f;
static const char map_path[] = "assets/map.txt";
static const char background_music_path[] = "assets/soundtrack.wav";
static const char zombie_death_sound_path[] = "assets/zombie_dies.wav";
static const char zombie_attention_sound_path[] = "assets/zombie_attention.wav";
static const char zombie_born_sound_path[] = "assets/zombie_born.wav";
static const char stone_throw_sound_path[] = "assets/stone_throw.wav";
static const float max_attack_distance = 700.f;
static const float attack_divisor = 2.f;
static const float stone_width = 10.f;
static const float stone_height = 10.f;
static const float stone_speed = 100.0f;
static const char stone_texture_path[] = "assets/stone.png";
static const unsigned int stone_texture_src_x = 0u;
static const unsigned int stone_texture_src_y = 0u;
static const unsigned int stone_texture_src_width = 32u;
static const unsigned int stone_texture_src_height = 32u;
static const bool stone_texture_animated = false;
static const unsigned int stone_texture_frames = 0u;
static const float stone_texture_frame_duration = 0.f;
static const float fallen_stone_noise_intensity = 2.f;
static const float fallen_stone_noise_duration = 3000.f;
static const char fallen_stone_sound_path[] = "assets/stone_falls.wav";
static const unsigned int water_tile_id = 0u;
static const unsigned int dirt_tile_id = 1u;
static const unsigned int rock_tile_id = 2u;
static const char water_tile_texture_path[] = "assets/water.png";
static const char dirt_tile_texture_path[] = "assets/dirt.png";
static const char rock_tile_texture_path[] = "assets/rock.png";
static const char rock_tag[] = "rocktile";
static const char water_tag[] = "watertile";
static const char fps_font_path[] = "assets/fps.ttf";
static const char gameover_font_path[] = "assets/gameover.ttf";
static const int fps_font_size = 20;
static const int gameover_font_size = 250;
static const int fps_color_r = 255;
static const int fps_color_g = 255;
static const int fps_color_b = 255;
static const int gameover_color_r = 190;
static const int gameover_color_g = 0;
static const int gameover_color_b = 0;
static const int fps_position_x = 0;
static const int fps_position_y = 0;
static const int gameover_position_x = 120;
static const int gameover_position_y = 150;
static const float fps_update_period = 1000.f;

Game::Game(const WindowParameters& window_params)
    : display_(window_params), is_running_(false), system_manager_(&entity_manager_) {}

AssetManager& Game::GetAssetManager() { return asset_manager_; }
Display& Game::GetDisplay() { return display_; }

void Game::ConfigureGame() {
    std::unordered_map<unsigned int, SDLProxy::SDLTextureSharedPtr> texture_map{
        {water_tile_id, asset_manager_.GetTexture(water_tile_texture_path)},
        {dirt_tile_id, asset_manager_.GetTexture(dirt_tile_texture_path)},
        {rock_tile_id, asset_manager_.GetTexture(rock_tile_texture_path)}};
    std::unordered_map<unsigned int, std::string> tag_map{{water_tile_id, water_tag}, {rock_tile_id, rock_tag}};
    Tilemap map(entity_manager_, map_path, texture_map, tag_map);
    float world_width, world_height;
    map.GetWorldSize(&world_width, &world_height);

    ECS::Entity& player = CreatePlayer();

    EnemySpawnSystem::EnemyEntityParameters enemy_params;
    enemy_params.enemy_width = enemy_width;
    enemy_params.enemy_height = enemy_height;
    enemy_params.spawn_area.width_ = world_width - enemy_params.enemy_width;
    enemy_params.spawn_area.height_ = world_height - enemy_params.enemy_height;
    enemy_params.enemy_speed = enemy_speed;
    enemy_params.texture = asset_manager_.GetTexture(enemy_texture_path);
    enemy_params.src_rect = utils::Rectangle<unsigned int>({enemy_texture_src_x, enemy_texture_src_y},
                                                           enemy_texture_src_width, enemy_texture_src_height);
    enemy_params.animated = enemy_texture_animated;
    enemy_params.frames = enemy_texture_frames;
    enemy_params.frame_duration = enemy_texture_frame_duration;
    system_manager_.AddSystem<EnemySpawnSystem>(enemy_params, enemy_spawn_period,
                                                asset_manager_.GetChunk(zombie_born_sound_path));
    std::vector<ECS::EntityManager::GroupID> drawing_order = {GroupID::MAP, GroupID::PLAYER, GroupID::ENEMY,
                                                              GroupID::STONE};
    SDLProxy::SDLFontSharedPtr fps_font = asset_manager_.GetFont(fps_font_path, fps_font_size);
    SDLProxy::SDLFontSharedPtr gameover_font = asset_manager_.GetFont(gameover_font_path, gameover_font_size);
    RenderSystem::Color fps_color{fps_color_r, fps_color_g, fps_color_b};
    RenderSystem::Color gameover_color{gameover_color_r, gameover_color_g, gameover_color_b};
    utils::Vector2D<int> fps_position(fps_position_x, fps_position_y);
    utils::Vector2D<int> gameover_position(gameover_position_x, gameover_position_y);
    system_manager_.AddSystem<RenderSystem>(drawing_order, fps_font, gameover_font, fps_color, gameover_color,
                                            fps_position, gameover_position, fps_update_period);
    system_manager_.AddSystem<EnemyMovementSystem>(enemy_scan_radius,
                                                   asset_manager_.GetChunk(zombie_attention_sound_path));
    system_manager_.AssignPriority<RenderSystem>(1);
    system_manager_.AddSystem<TransformSystem>(world_width, world_height);
    system_manager_.AddSystem<CollisionSystem>(asset_manager_.GetChunk(zombie_death_sound_path), rock_tag, water_tag);
    KeyboardControllerSystem::StoneEntityParameters stone_params;
    stone_params.width = stone_width;
    stone_params.height = stone_height;
    stone_params.speed = stone_speed;
    stone_params.texture = asset_manager_.GetTexture(stone_texture_path);
    stone_params.src_rect.top_left_.x = stone_texture_src_x;
    stone_params.src_rect.top_left_.y = stone_texture_src_y;
    stone_params.src_rect.width_ = stone_texture_src_width;
    stone_params.src_rect.height_ = stone_texture_src_height;
    stone_params.animated = stone_texture_animated;
    stone_params.frames = stone_texture_frames;
    stone_params.frame_duration = stone_texture_frame_duration;
    system_manager_.AddSystem<KeyboardControllerSystem>(stone_params, max_attack_distance, attack_divisor,
                                                        asset_manager_.GetChunk(stone_throw_sound_path));
    system_manager_.AddSystem<CameraSystem>(static_cast<int>(world_width), static_cast<int>(world_height));
    system_manager_.AddSystem<MouseControllerSystem>();
    system_manager_.AddSystem<DistanceLimitSystem>();
    system_manager_.AddSystem<StoneDestroyingSystem>(fallen_stone_noise_intensity, fallen_stone_noise_duration,
                                                     asset_manager_.GetChunk(fallen_stone_sound_path));
    system_manager_.AddSystem<LifetimeSystem>();
}

ECS::Entity& Game::CreatePlayer() {
    ECS::Entity& player = entity_manager_.AddEntity();
    utils::Rectangle<float> player_bbox({player_starting_position_x, player_starting_position_y}, player_width,
                                        player_height);
    TransformComponent& player_transform = player.AddComponent<TransformComponent>(player_bbox);
    player_transform.speed_ = player_speed;
    auto player_texture = asset_manager_.GetTexture(player_texture_path);
    utils::Rectangle<unsigned int> player_src_rect({player_texture_src_x, player_texture_src_y},
                                                   player_texture_src_width, player_texture_src_height);
    player.AddComponent<RenderComponent>(player_texture, player_src_rect, player_texture_animated,
                                         player_texture_frames, player_texture_frame_duration);
    player.AddComponent<ColliderComponent>(player_transform.bbox_);
    player.AddComponent<NoiseComponent>();
    player.AddComponent<AttackComponent>();
    entity_manager_.AddToGroup(player.GetID(), GroupID::PLAYER);
    return player;
}

void Game::RunLoop() {
    auto background_music = asset_manager_.GetMusic(background_music_path);
    audio_.PlayMusic(background_music);
    ConfigureGame();
    is_running_ = true;
    std::chrono::time_point<std::chrono::steady_clock> t_start = std::chrono::high_resolution_clock::now();
    while (is_running_) {
        sdl_event_handler_.HandleEvents();
        entity_manager_.Refresh();
        std::chrono::duration<float, std::milli> duration(std::chrono::high_resolution_clock::now() - t_start);
        t_start = std::chrono::high_resolution_clock::now();
        float time_elapsed = duration.count();
        system_manager_.Update(time_elapsed);
    }
}

void Game::Shutdown() { is_running_ = false; }

Audio& Game::GetAudio() { return audio_; }
