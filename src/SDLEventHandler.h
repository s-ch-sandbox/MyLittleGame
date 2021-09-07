#pragma once
class SDLEventHandler {
    enum class EventResult { NONE, QUIT };

   public:
    EventResult HandleEvents();
};
