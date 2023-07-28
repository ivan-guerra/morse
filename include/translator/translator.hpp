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
     * @brief Construct a translator with the paramater dot/dash soundbites.
     *
     * @param [in] dot_wav Path to dot soundbite.
     * @param [in] dash_wav Path to dash soundbite.
     *
     * @throws std::runtime_error When either audio file cannot be loaded.
     */
    Translator(const std::string& dot_wav, const std::string& dash_wav);
    ~Translator() = default;

    Translator() = delete;
    Translator(const Translator&) = delete;
    Translator& operator=(const Translator&) = delete;
    Translator(Translator&&) = delete;
    Translator& operator=(Translator&&) = delete;

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
    void ToAudio(const std::vector<std::string>& words) const;

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

    enum DelayMultiplier : int {
        kDot = 5,
        kChar = 6,
        kWord = 7,
    };

    void Delay(double delay_sec) const;
    void PlaySound(char morse_char) const;

    MorseCodeAudioPlayer player_;
};

}  // namespace morse

#endif
