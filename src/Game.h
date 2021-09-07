#pragma once
#include "AssetManager.h"
#include "Audio.h"
#include "Display.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "SDLEventHandler.h"
#include "utils/Singleton.h"

class Game : public utils::Singleton<Game> {
    friend class utils::Singleton<Game>;

   public:
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    ~Game() = default;
    void RunLoop();
    void Shutdown();
    AssetManager& GetAssetManager();
    Audio& GetAudio();
    Display& GetDisplay();

   private:
    Game(const WindowParameters& window_params);
    void ConfigureGame();
    ECS::Entity& CreatePlayer();

    SDLEventHandler sdl_event_handler_;
    Display display_;
    Audio audio_;
    AssetManager asset_manager_;
    ECS::EntityManager entity_manager_;
    ECS::SystemManager system_manager_;
    bool is_running_;
};
