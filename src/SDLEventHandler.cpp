#include "SDLEventHandler.h"

#include "SDL.h"

SDLEventHandler::EventResult SDLEventHandler::HandleEvents() {
    SDL_Event sdl_event;
    SDL_PollEvent(&sdl_event);
    switch (sdl_event.type) {
        case SDL_QUIT:
            return SDLEventHandler::EventResult::QUIT;
    }
    return SDLEventHandler::EventResult::NONE;
}
