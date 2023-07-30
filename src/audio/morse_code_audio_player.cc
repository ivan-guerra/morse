#include "audio/morse_code_audio_player.hpp"

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
      dash_duration_ms_(duration_ms * 3) {}

}  // namespace morse
