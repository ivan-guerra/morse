#ifndef BEEP_HPP_
#define BEEP_HPP_

#include <SDL2/SDL.h>

#include <queue>

namespace morse {

/**
 * @brief A utility class for generating a beep sound on the host's speakers.
 *
 * @details This code is a slightly modified version of the original Beeper
 * implementation: <a
 * href="https://web.archive.org/web/20120313055436/http://www.dgames.org/beep-sound-with-sdl/">"Beep
 * sound with SDL"</a>
 */
class Beeper {
   public:
    /**
     * @brief Construct a beeper object.
     *
     * @throws std::runtime_error When the audio system cannot be initialized.
     */
    Beeper();
    ~Beeper() { SDL_CloseAudio(); }

    Beeper(const Beeper&) = default;
    Beeper& operator=(const Beeper&) = default;
    Beeper(Beeper&&) = default;
    Beeper& operator=(Beeper&&) = default;

    /**
     * @brief Register and play a beep.
     *
     * @details Beep() can be called multiple times in sequence. Each call will
     *          queue up a new beep. The beeps in the queue will immediately
     *          begin to play one after the other.
     */
    void Beep(double frequency_hz, int duration_ms);

    /** @brief Block until all registered beeps have been played. */
    void Wait() const;

   private:
    static const int kDefaultFrequencyHz = 44100;
    static const int kDefaultAmplitude = 28000;

    struct BeepObject {
        double freq;
        int samples_left;
    };

    void GenerateSamples(Sint16* stream, int length);

    friend void AudioCallback(void* _beeper, Uint8* _stream, int _length);

    double oscillator_;
    std::queue<BeepObject> beeps_;
};

}  // namespace morse

#endif
