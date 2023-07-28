#include "translator/translator.hpp"

#include <unistd.h>

#include <string>
#include <unordered_map>

#include "audio/morse_code_audio_player.hpp"

namespace morse {

const std::unordered_map<char, std::string> Translator::kMorseToAscii = {
    {'a', ".-"},    {'b', "-..."},  {'c', "-.-."},  {'d', "-.."},
    {'e', "."},     {'f', "..-."},  {'g', "--."},   {'h', "...."},
    {'i', ".."},    {'j', ".---"},  {'k', "-.-"},   {'l', ".-.."},
    {'m', "--"},    {'n', "-."},    {'o', "---"},   {'p', ".--."},
    {'q', "--.-"},  {'r', ".-."},   {'s', "..."},   {'t', "-"},
    {'u', "..-"},   {'v', "...-"},  {'w', ".--"},   {'x', "-..-"},
    {'y', "-.--"},  {'z', "--.."},  {'0', "-----"}, {'1', ".----"},
    {'2', "..---"}, {'3', "...--"}, {'4', "....-"}, {'5', "....."},
    {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."},
};

void Translator::Delay(double delay_sec) const {
    static const double kSecToUsec = 1000000.0;
    usleep(static_cast<unsigned int>(delay_sec * kSecToUsec));
}

void Translator::PlaySound(char morse_char) const {
    static const char kDot = '.';
    static const char kDash = '-';
    if (morse_char == kDot) {
        player_.PlayDot();
    } else if (morse_char == kDash) {
        player_.PlayDash();
    }
    Delay(player_.DotDuration() * static_cast<double>(DelayMultiplier::kDot));
}

Translator::Translator(const std::string& dot_wav, const std::string& dash_wav)
    : player_(dot_wav, dash_wav) {}

void Translator::ToAudio(const std::vector<std::string>& words) const {
    for (const std::string& word : words) {
        for (const char& c : word) {
            if (kMorseToAscii.count(c)) {
                for (const char& morse_char : kMorseToAscii.at(c)) {
                    PlaySound(morse_char);
                }
            }
            Delay(player_.DotDuration() *
                  static_cast<double>(DelayMultiplier::kChar));
        }
        Delay(player_.DotDuration() *
              static_cast<double>(DelayMultiplier::kWord));
    }
}

std::string Translator::ToCode(const std::vector<std::string>& words) const {
    std::string translation;
    for (const std::string& word : words) {
        for (const char& c : word) {
            if (kMorseToAscii.count(c)) {
                for (const char& morse_char : kMorseToAscii.at(c)) {
                    translation += morse_char;
                }
            } else {
                translation += '#';
            }
            translation += ' ';
        }
        translation += "/ ";
    }

    /* trim off the trailing " / " string */
    return translation.substr(0, translation.size() - 3);
}

}  // namespace morse
