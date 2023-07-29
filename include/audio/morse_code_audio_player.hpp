#ifndef MORSE_CODE_AUDIO_PLAYER_HPP_
#define MORSE_CODE_AUDIO_PLAYER_HPP_

#include <SDL2/SDL_mixer.h>

#include <string>

namespace morse {

/**
 * @brief Play dot and dash sounds through the host system's speakers on demand.
 */
class MorseCodeAudioPlayer {
   public:
    /**
     * @brief Construct an audio player that uses the parameter dot/dash audio.
     *
     * @param [in] dot_wav Path to a dot audio WAV file.
     * @param [in] dash_wav Path to a dash audio WAV file.
     *
     * @throws std::runtime_error Signals an irrecoverable error in the audio
     *                            player's construction. The what() message will
     *                            report the error identified by the underlying
     *                            SDL2 lib.
     */
    MorseCodeAudioPlayer(const std::string& dot_wav,
                         const std::string& dash_wav);
    ~MorseCodeAudioPlayer();

    MorseCodeAudioPlayer() = delete;
    MorseCodeAudioPlayer(const MorseCodeAudioPlayer&) = delete;
    MorseCodeAudioPlayer& operator=(const MorseCodeAudioPlayer&) = delete;
    MorseCodeAudioPlayer(MorseCodeAudioPlayer&&) = delete;
    MorseCodeAudioPlayer& operator=(MorseCodeAudioPlayer&&) = delete;

    /** @brief Play the dot soundbite. */
    void PlayDot() const { PlayWav(MorseSymbol::kDot); }

    /** @brief Play the dash soundbite. */
    void PlayDash() const { PlayWav(MorseSymbol::kDash); }

    /** @brief Returns the duration of the dot soundbite in seconds. */
    double DotDuration() const { return dot_duration_sec_; }

    /** @brief Returns the duration of the dash soundbite in seconds. */
    double DashDuration() const { return dash_duration_sec_; }

   private:
    enum class MorseSymbol {
        kDot,
        kDash,
    };

    void PlayWav(MorseSymbol symbol) const;
    double GetWavDurationSec(const std::string& wav_filename) const;

    Mix_Chunk* dot_;
    Mix_Chunk* dash_;
    double dot_duration_sec_;
    double dash_duration_sec_;
};

}  // namespace morse

#endif
