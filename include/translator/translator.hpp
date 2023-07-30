#ifndef TRANSLATOR_HPP_
#define TRANSLATOR_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "audio/morse_code_audio_player.hpp"

namespace morse {

/**
 * @brief Translate ASCII text data to Morse code audio or textual code.
 */
class Translator {
   public:
    /**
     * @brief Construct a ASCII to Morse code text/audio translator.
     *
     * @param [in] frequency_hz Frequency or pitch of dot/dash sounds in Hertz.
     * @param [in] duration_hz Duration of a dot sound in milliseconds. By
     *                         default, the duration of a dash is 3x the
     *                         duration of a dot.
     *
     * @throws std::runtime_error When the audio system cannot be initialized.
     */
    explicit Translator(double frequency_hz = kDefaultFrequencyHz,
                        int duration_ms = kDefaultDurationMs);

    ~Translator() = default;
    Translator(const Translator&) = default;
    Translator& operator=(const Translator&) = default;
    Translator(Translator&&) = default;
    Translator& operator=(Translator&&) = default;

    /**
     * @brief Play \p words as Morse audio over the host's speakers.
     *
     * @details ToAudio() will translate each word in \p words to a series of
     *          dots/dashes played over the host's speakers. If a character is
     *          not recognized, ToAudio() will skip that character. Valid
     *          characters include [a-z], [A-Z], and [0-9].
     *
     * @param [in] words A list of words.
     */
    void ToAudio(const std::vector<std::string>& words);

    /**
     * @brief Return the Morse encoding of \p words.
     *
     * @details ToCode() will translate each word in \p words to a series
     *          of dots/dashes. Characters in the output string are seperated
     *          by a space, words are seperated by " / ", and unknown
     *          characters are displayed as '#'. Valid characters include
     *          [a-z], [A-Z], and [0-9].
     *
     * @param [in] words A list of words.
     *
     * @returns The Morse encoding of \p words.
     */
    std::string ToCode(const std::vector<std::string>& words) const;

   private:
    static const std::unordered_map<char, std::string> kMorseToAscii;
    static const double kDefaultFrequencyHz;
    static const int kDefaultDurationMs;

    enum DelayMultiplier : int {
        kSymbol = 1,
        kChar = 3,
        kWord = 7,
    };

    void Delay(int delay_ms) const;

    MorseCodeAudioPlayer player_;
};

}  // namespace morse

#endif
