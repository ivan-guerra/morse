#include <getopt.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "io/io.hpp"
#include "translator/translator.hpp"

static const std::pair<int, int> kZeroToOneHundred = {0, 100};
static const std::pair<int, int> kPitchLimitsHz = {200, 800};
static const std::pair<int, int> kDurationLimitsMs = {50, 250};

struct ProgramConfig {
    std::string input_filepath;
    std::string output_filepath;
    bool disable_audio = false;
    bool print_code = false;
    int pitch_percent = 50;
    int duration_percent = 25;
};

static double MapValueToRange(const std::pair<int, int>& src_range,
                              const std::pair<int, int>& tgt_range, int value) {
    double slope = 1.0 * (tgt_range.second - tgt_range.first) /
                   (src_range.second - src_range.first);
    return (tgt_range.first + slope * (value - src_range.first));
}

static void PrintErrAndExit(const std::string& err) {
    std::cerr << "error: " << err << std::endl;
    std::cerr << "try 'morse --help' for more information" << std::endl;
    std::exit(EXIT_FAILURE);
}

void PrintUsage() {
    std::cout << "usage: morse [OPTION]..." << std::endl;
    std::cout << "convert ascii text to Morse code text and audio" << std::endl
              << std::endl;

    std::cout << "OPTIONS" << std::endl;
    std::cout << "\t-i,--input-ascii\n\t\tpath to an input ASCII text file"
              << std::endl;
    std::cout << "\t-o,--output-ascii\n\t\tpath to output Morse coded input"
              << std::endl;
    std::cout << "\t-d,--disable-audio\n\t\tdisable Morse code audio player"
              << std::endl;
    std::cout << "\t-p,--print-code\n\t\tprint Morse encoding to STDOUT"
              << std::endl;
    std::cout
        << "\t-u,--pitch\n\t\ta integer percentage value in the range "
           "[0, 100], the higher the \n\t\tpercentage the higher the pitch of "
           "each dot/dash (default 50)"
        << std::endl;
    std::cout
        << "\t-l,--duration\n\t\ta integer percentage value in the range "
           "[0, 100], the higher the \n\t\tpercentage the longer each dot/dash "
           "tone lasts (default 25)"
        << std::endl;
    std::cout << "\t-h,--help\n\t\tprint this help message" << std::endl;
}

int main(int argc, char** argv) {
    struct option long_options[] = {
        {"input-ascii", required_argument, 0, 'i'},
        {"output-ascii", required_argument, 0, 'o'},
        {"disable-audio", required_argument, 0, 'd'},
        {"print-code", required_argument, 0, 'p'},
        {"pitch", required_argument, 0, 'u'},
        {"duration", required_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };

    /* parse program options */
    int opt = '\0';
    int long_index = 0;
    ProgramConfig config;
    while (-1 != (opt = getopt_long(argc, argv, "i:o:dpu:l:h",
                                    static_cast<struct option*>(long_options),
                                    &long_index))) {
        switch (opt) {
            case 'i':
                config.input_filepath = optarg;
                break;
            case 'o':
                config.output_filepath = optarg;
                break;
            case 'd':
                config.disable_audio = true;
                break;
            case 'p':
                config.print_code = true;
                break;
            case 'h':
                PrintUsage();
                return 0;
            case 'u':
                config.pitch_percent = std::stoi(optarg);
                break;
            case 'l':
                config.duration_percent = std::stoi(optarg);
                break;
            case '?':
                return 1;
        }
    }

    try {
        std::vector<std::string> words;
        if (!config.input_filepath.empty()) { /* read text from input file */
            words = morse::io::ReadWords(config.input_filepath);
        } else { /* read text from STDIN */
            words = morse::io::ReadWords();
        }

        /* verify the pitch/duration percentages are in the range [0, 100] */
        auto OutOfRange = [](const std::pair<int, int>& range, int value) {
            return ((value > range.second) || (value < range.first));
        };
        if (OutOfRange(kZeroToOneHundred, config.pitch_percent)) {
            PrintErrAndExit("pitch percent must be in range [0, 100]");
        }
        if (OutOfRange(kZeroToOneHundred, config.duration_percent)) {
            PrintErrAndExit("duration percent must be in range [0, 100]");
        }

        const double kPitchHz = MapValueToRange(
            kZeroToOneHundred, kPitchLimitsHz, config.pitch_percent);
        const int kDurationMs = static_cast<int>(MapValueToRange(
            kZeroToOneHundred, kDurationLimitsMs, config.duration_percent));
        morse::Translator translator(kPitchHz, kDurationMs);

        if (config.print_code || !config.output_filepath.empty()) {
            std::string code = translator.ToCode(words);
            if (config.print_code) { /* write Morse code to STDOUT */
                std::cout << code << std::endl;
            }

            if (!config.output_filepath.empty()) { /* write code to file */
                morse::io::WriteToFile(translator.ToCode(words),
                                       config.output_filepath);
            }
        }

        if (!config.disable_audio) { /* play the Morse coded input over the
        speakers */
            translator.ToAudio(words);
        }
    } catch (const std::exception& e) {
        PrintErrAndExit(e.what());
    }

    return 0;
}
