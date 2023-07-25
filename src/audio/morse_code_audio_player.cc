#include "audio/morse_code_audio_player.hpp"

#include <SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdexcept>
#include <string>

namespace morse {

void MorseCodeAudioPlayer::PlayWav(MorseSymbol symbol) const {
    const int kDefaultChannel = -1;
    const int kPlayOnce = 0;

    if (symbol == MorseSymbol::kDot) {
        Mix_PlayChannel(kDefaultChannel, dot_, kPlayOnce);
    } else if (symbol == MorseSymbol::kDash) {
        Mix_PlayChannel(kDefaultChannel, dash_, kPlayOnce);
    }
}

MorseCodeAudioPlayer::MorseCodeAudioPlayer(const std::string& dot_wav,
                                           const std::string& dash_wav)
    : dot_(nullptr), dash_(nullptr) {
    /* SDL_Init() is optional since we later call Mix_OpenAudio(), the SDL2 docs
     * recommend init'ing anyways so we do it */
    if (SDL_Init(SDL_INIT_AUDIO)) {
        throw std::runtime_error(SDL_GetError());
    }

    /* using the default values recommended in the SDL2 docs:
     * https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudio */
    const int kDefaultFrequencyHz = 48000;
    const Uint16 kDefaultFormat = AUDIO_S16SYS;
    const int kDefaultNumChannels = 2; /* 2 = stereo */
    const int kDefaultChunkSize = 2048;
    if (Mix_OpenAudio(kDefaultFrequencyHz, kDefaultFormat, kDefaultNumChannels,
                      kDefaultChunkSize)) {
        throw std::runtime_error(Mix_GetError());
    }

    /* load the WAV sounds for dot and dash into memory */
    dot_ = Mix_LoadWAV(dot_wav.c_str());
    if (!dot_) {
        throw std::runtime_error(Mix_GetError());
    }
    dash_ = Mix_LoadWAV(dash_wav.c_str());
    if (!dash_) {
        throw std::runtime_error(Mix_GetError());
    }
}

MorseCodeAudioPlayer::~MorseCodeAudioPlayer() {
    if (dot_) {
        Mix_FreeChunk(dot_);
    }

    if (dash_) {
        Mix_FreeChunk(dash_);
    }

    Mix_CloseAudio();
    SDL_Quit();
}

}  // namespace morse
