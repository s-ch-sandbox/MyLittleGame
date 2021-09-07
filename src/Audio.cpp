#include "Audio.h"

#include "SDL_mixer.h"
#include "easylogging++.h"

Audio::Audio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    LOG(INFO) << "Audio opened.";
}

Audio::~Audio() { Mix_CloseAudio(); }

void Audio::PlayMusic(SDLProxy::SDLMixMusicSharedPtr music) {
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_PlayMusic(music.get(), -1);
}

void Audio::PlayChunk(SDLProxy::SDLMixChunkSharedPtr sound, int n_times) { Mix_PlayChannel(-1, sound.get(), n_times); }
