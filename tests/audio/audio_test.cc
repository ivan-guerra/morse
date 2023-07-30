#include <gtest/gtest.h>

#include <stdexcept>

#include "audio/morse_code_audio_player.hpp"

const double kDefaultFrequencyHz = 500.0;
const int kDefaultDurationMs = 70;

TEST(AudioTest,
     MorseCodeAudioPlayerConstructorThrowsRuntimeErrorOnZeroFrequency) {
    ASSERT_THROW(morse::MorseCodeAudioPlayer(0.0, kDefaultDurationMs),
                 std::runtime_error);
}

TEST(AudioTest,
     MorseCodeAudioPlayerConstructorThrowsRuntimeErrorOnNegativeFrequency) {
    ASSERT_THROW(morse::MorseCodeAudioPlayer(-1.0, kDefaultDurationMs),
                 std::runtime_error);
}

TEST(AudioTest,
     MorseCodeAudioPlayerConstructorThrowsRuntimeErrorOnZeroDuration) {
    ASSERT_THROW(morse::MorseCodeAudioPlayer(kDefaultFrequencyHz, 0),
                 std::runtime_error);
}

TEST(AudioTest,
     MorseCodeAudioPlayerConstructorThrowsRuntimeErrorOnNegativeDuration) {
    ASSERT_THROW(morse::MorseCodeAudioPlayer(kDefaultFrequencyHz, -1),
                 std::runtime_error);
}

TEST(AudioTest, DotDurationReturnsDurationGivenAtConstruction) {
    morse::MorseCodeAudioPlayer player(kDefaultFrequencyHz, kDefaultDurationMs);
    ASSERT_EQ(kDefaultDurationMs, player.DotDuration());
}

TEST(AudioTest, DashDurationReturnsThreeTimesDurationGivenAtConstruction) {
    morse::MorseCodeAudioPlayer player(kDefaultFrequencyHz, kDefaultDurationMs);
    ASSERT_EQ(kDefaultDurationMs * 3, player.DashDuration());
}
