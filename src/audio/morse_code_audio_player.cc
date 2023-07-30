#include "audio/morse_code_audio_player.hpp"

#include <stdexcept>

namespace morse {

const double MorseCodeAudioPlayer::kDefaultFrequencyHz = 500;
const int MorseCodeAudioPlayer::kDefaultDurationMs = 70;

void MorseCodeAudioPlayer::PlayMorseCodeSound(MorseSymbol symbol) {
    if (symbol == MorseSymbol::kDot) {
        beeper_.Beep(frequency_hz_, dot_duration_ms_);
    } else if (symbol == MorseSymbol::kDash) {
        beeper_.Beep(frequency_hz_, dash_duration_ms_);
    }
    beeper_.Wait();
}

MorseCodeAudioPlayer::MorseCodeAudioPlayer(double frequency_hz, int duration_ms)
    : frequency_hz_(frequency_hz),
      dot_duration_ms_(duration_ms),
      dash_duration_ms_(duration_ms * 3) {
    if (frequency_hz <= 0.0) {
        throw std::runtime_error("frequency must be a positive value");
    }
    if (duration_ms <= 0) {
        throw std::runtime_error("duration must be a positive value");
    }
}

}  // namespace morse
