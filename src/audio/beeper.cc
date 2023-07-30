#include "audio/beeper.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace morse {

void AudioCallback(void *_beeper, Uint8 *_stream, int _length) {
    Beeper *beeper = reinterpret_cast<Beeper *>(_beeper);
    Sint16 *stream = reinterpret_cast<Sint16 *>(_stream);
    int length = _length / 2;

    beeper->GenerateSamples(stream, length);
}

Beeper::Beeper() : oscillator_(0.0) {
    if (SDL_Init(SDL_INIT_AUDIO)) {
        throw std::runtime_error(SDL_GetError());
    }

    const int kDefaultNumChannels = 1;
    const int kDefaultSamples = 2048;
    SDL_AudioSpec desired_spec;
    desired_spec.freq = kDefaultFrequencyHz;
    desired_spec.format = AUDIO_S16SYS;
    desired_spec.channels = kDefaultNumChannels;
    desired_spec.samples = kDefaultSamples;
    desired_spec.callback = AudioCallback;
    desired_spec.userdata = this;

    SDL_AudioSpec obtained_spec;
    if (SDL_OpenAudio(&desired_spec, &obtained_spec)) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_PauseAudio(0); /* unpause audio */
}

void Beeper::Beep(double frequency_hz, int duration_ms) {
    static const int kMsToSec = 1000;
    BeepObject beep = {
        .freq = frequency_hz,
        .samples_left = duration_ms * kDefaultFrequencyHz / kMsToSec,
    };

    SDL_LockAudio();
    beeps_.push(beep);
    SDL_UnlockAudio();
}

void Beeper::GenerateSamples(Sint16 *stream, int length) {
    int i = 0;
    while (i < length) {
        if (beeps_.empty()) {
            while (i < length) {
                stream[i] = 0;
                i++;
            }
            return;
        }
        BeepObject &beep = beeps_.front();

        int samples_to_do = std::min(i + beep.samples_left, length);
        beep.samples_left -= samples_to_do - i;

        while (i < samples_to_do) {
            stream[i] = static_cast<Sint16>(
                kDefaultAmplitude *
                std::sin(oscillator_ * 2 * std::numbers::pi /
                         kDefaultFrequencyHz));
            i++;
            oscillator_ += beep.freq;
        }

        if (0 == beep.samples_left) { /* have we completed this beep */
            beeps_.pop();
        }
    }
}

void Beeper::Wait() const {
    const int kDelayMs = 20;
    std::size_t size = 0;
    do {
        SDL_LockAudio();
        size = beeps_.size();
        SDL_UnlockAudio();
    } while (size > 0);

    /* this delay removes speaker crackle following the last beep */
    const int kSmoothSoundDelayMs = kDelayMs * 10;
    SDL_Delay(kSmoothSoundDelayMs);
}

}  // namespace morse
