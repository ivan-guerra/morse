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

const double Translator::kDefaultFrequencyHz = 500;
const int Translator::kDefaultDurationMs = 100;

void Translator::Delay(int delay_ms) const {
    static const int kMsToUsec = 1000;
    usleep(delay_ms * kMsToUsec);
}

Translator::Translator(double frequency_hz, int duration_ms)
    : player_(frequency_hz, duration_ms) {}

void Translator::ToAudio(const std::vector<std::string>& words) {
    for (const std::string& word : words) {
        for (const char& c : word) {
            if (kMorseToAscii.count(c)) {
                for (const char& morse_char : kMorseToAscii.at(c)) {
                    if ('.' == morse_char) {
                        player_.PlayDot();
                    } else {
                        player_.PlayDash();
                    }
                    Delay(player_.DotDuration() * DelayMultiplier::kSymbol);
                }
            }
            Delay(player_.DotDuration() * DelayMultiplier::kChar);
        }
        Delay(player_.DotDuration() * DelayMultiplier::kWord);
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
