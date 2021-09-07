#include "Game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    el::Configurations conf("./src/logger.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        LOG(ERROR) << "Couldn't initialize SDL subsystem.";
        return 1;
    }
    if (TTF_Init() != 0) {
        LOG(ERROR) << "Couldn't initialize SDL font.";
        return 1;
    }
    Game::InitInstance(WindowParameters{"Zombie", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false});
    Game* game = Game::GetInstance();
    game->RunLoop();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
