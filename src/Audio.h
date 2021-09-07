#pragma once
#include "utils/SDLProxy.h"

class Audio {
   public:
    Audio();
    ~Audio();
    void PlayMusic(SDLProxy::SDLMixMusicSharedPtr music);
    void PlayChunk(SDLProxy::SDLMixChunkSharedPtr sound, int n_times = 0);

   private:
};
