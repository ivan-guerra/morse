#ifndef MORSE_CODE_AUDIO_PLAYER_HPP_
#define MORSE_CODE_AUDIO_PLAYER_HPP_

#include "audio/beeper.hpp"

namespace morse {

/**
 * @brief Play dot and dash sounds through the host system's speakers on demand.
 */
class MorseCodeAudioPlayer {
   public:
    /**
     * @brief Construct a Morse code dot/dash audio player.
     *
     * @param [in] frequency_hz Frequency or pitch of dot/dash sounds in Hertz.
     * @param [in] duration_hz Duration of a dot sound in milliseconds. By
     *                         default, the duration of a dash is 3x the
     *                         duration of a dot.
     *
     * @throws std::runtime_error When the audio system cannot be initialized or
     *                            frequency/duration is not positive.
     */
    explicit MorseCodeAudioPlayer(double frequency_hz = kDefaultFrequencyHz,
                                  int duration_ms = kDefaultDurationMs);

    ~MorseCodeAudioPlayer() = default;
    MorseCodeAudioPlayer(const MorseCodeAudioPlayer&) = default;
    MorseCodeAudioPlayer& operator=(const MorseCodeAudioPlayer&) = default;
    MorseCodeAudioPlayer(MorseCodeAudioPlayer&&) = default;
    MorseCodeAudioPlayer& operator=(MorseCodeAudioPlayer&&) = default;

    /** @brief Play the dot soundbite. */
    void PlayDot() { PlayMorseCodeSound(MorseSymbol::kDot); }

    /** @brief Play the dash soundbite. */
    void PlayDash() { PlayMorseCodeSound(MorseSymbol::kDash); }

    /** @brief Returns the duration of the dot soundbite in milliseconds. */
    int DotDuration() const { return dot_duration_ms_; }

    /**
     * @brief Returns the duration of the dash soundbite in milliseconds.
     *
     * @details According to the Morse code wiki, the duration of a dash is 3
     *          times the duration of a dot by default.
     */
    int DashDuration() const { return dash_duration_ms_; }

   private:
    static const double kDefaultFrequencyHz;
    static const int kDefaultDurationMs;

    enum class MorseSymbol {
        kDot,
        kDash,
    };

    void PlayMorseCodeSound(MorseSymbol symbol);

    double frequency_hz_;
    int dot_duration_ms_;
    int dash_duration_ms_;
    Beeper beeper_;
};

}  // namespace morse

#endif
