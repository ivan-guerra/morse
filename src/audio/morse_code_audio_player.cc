#include "audio/morse_code_audio_player.hpp"

#include <SDL.h>
#include <SDL2/SDL_mixer.h>

#include <cstdint>
#include <stdexcept>
#include <string>

namespace morse {

double MorseCodeAudioPlayer::GetWavDurationSec(
    const std::string& wav_filename) const {
    SDL_AudioSpec spec;
    uint32_t audioLen = 0;
    uint8_t* audioBuf = nullptr;
    double seconds = 0.0;
    const int kBitsInAByte = 8;

    if (SDL_LoadWAV(wav_filename.c_str(), &spec, &audioBuf, &audioLen)) {
        /* we aren't using the actual audio in this example */
        SDL_FreeWAV(audioBuf);
        uint32_t sampleSize = SDL_AUDIO_BITSIZE(spec.format) / kBitsInAByte;
        uint32_t sampleCount = audioLen / sampleSize;
        /* could do a sanity check and make sure (audioLen % sampleSize) is 0 */
        uint32_t sampleLen = 0;
        if (spec.channels) {
            sampleLen = sampleCount / spec.channels;
        } else {
            /* spec.channels should be 1 or higher, but just in case */
            sampleLen = sampleCount;
        }
        seconds =
            static_cast<double>(sampleLen) / static_cast<double>(spec.freq);
    } else {
        throw std::runtime_error(SDL_GetError());
    }
    return seconds;
}

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
    : dot_(nullptr),
      dash_(nullptr),
      dot_duration_sec_(0.0),
      dash_duration_sec_(0.0) {
    /* SDL_Init() is optional since we later call Mix_OpenAudio(), the SDL2 docs
     * recommend init'ing anyways so we do it */
    if (SDL_Init(SDL_INIT_AUDIO)) {
        throw std::runtime_error(SDL_GetError());
    }

    /* using the default values recommended in the SDL2 docs:
     * https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudio */
    const int kDefaultFrequencyHz = 48000;
    const Uint16 kDefaultFormat = AUDIO_S16SYS;
    const int kDefaultNumChannels = 1; /* 1 = mono */
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

    /* compute the duration of each sound */
    dot_duration_sec_ = GetWavDurationSec(dot_wav);
    dash_duration_sec_ = GetWavDurationSec(dash_wav);
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
