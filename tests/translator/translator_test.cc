#include "translator/translator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <vector>

const double kDefaultFrequencyHz = 500.0;
const int kDefaultDurationMs = 70;

TEST(TranslatorTest, TranslatorConstructorThrowsRuntimeErrorOnZeroFrequency) {
    ASSERT_THROW(morse::Translator(0.0, kDefaultDurationMs),
                 std::runtime_error);
}

TEST(TranslatorTest,
     TranslatorConstructorThrowsRuntimeErrorOnNegativeFrequency) {
    ASSERT_THROW(morse::Translator(-1.0, kDefaultDurationMs),
                 std::runtime_error);
}

TEST(TranslatorTest, TranslatorConstructorThrowsRuntimeErrorOnZeroDuration) {
    ASSERT_THROW(morse::Translator(kDefaultFrequencyHz, 0), std::runtime_error);
}

TEST(TranslatorTest,
     TranslatorConstructorThrowsRuntimeErrorOnNegativeDuration) {
    ASSERT_THROW(morse::Translator(kDefaultFrequencyHz, -1),
                 std::runtime_error);
}

TEST(TranslatorTest, ToCodeOutputsCorrectCodeForLowerCaseChars) {
    const std::vector<std::string> kAllLowerCaseInput = {
        "abcdefghijklmnopqrstuvwxyz0123456789"};
    const std::string kAllLowerCaseOutput(
        ".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- "
        ".-. ... - ..- ...- .-- -..- -.-- --.. ----- .---- ..--- ...-- ....- "
        "..... -.... --... ---.. ----.");

    /* test whether we can translate every char in the Morse alphabet */
    morse::Translator translator;
    ASSERT_EQ(kAllLowerCaseOutput, translator.ToCode(kAllLowerCaseInput));
}

TEST(TranslatorTest, ToCodeOutputsCorrectCodeForUpperCaseChars) {
    const std::vector<std::string> kAllUpperCaseInput = {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    const std::string kAllUpperCaseOutput(
        ".- -... -.-. -.. . ..-. --. .... .. .--- -.- .-.. -- -. --- .--. --.- "
        ".-. ... - ..- ...- .-- -..- -.-- --.. ----- .---- ..--- ...-- ....- "
        "..... -.... --... ---.. ----.");

    /* ToCode() should convert uppercase characters to lowercase before
     * attempting to translate the char to Morse code */
    morse::Translator translator;
    ASSERT_EQ(kAllUpperCaseOutput, translator.ToCode(kAllUpperCaseInput));
}

TEST(TranslatorTest, ToCodeOutputsCorrectCodeForInvalidChars) {
    const std::vector<std::string> kInvalidCharsInput = {
        ":';\",./<>?!@#$%^&*()_+-=~`\\ \t\n"};
    const std::string kInvalidCharsOutput(
        "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # #");

    /* verify we can handle the most common invalid characters */
    morse::Translator translator;
    ASSERT_EQ(kInvalidCharsOutput, translator.ToCode(kInvalidCharsInput));
}

TEST(TranslatorTest, ToCodeOutputsCorrectCodeForMultiWordInput) {
    const std::vector<std::string> kMultiWordInput = {"this", "is", "a",
                                                      "test"};
    const std::string kMultiWordOutput(
        "- .... .. ... / .. ... / .- / - . ... -");

    morse::Translator translator;
    ASSERT_EQ(kMultiWordOutput, translator.ToCode(kMultiWordInput));
}

TEST(TranslatorTest, ToCodeOutputsCorrectCodeForEmptyInput) {
    morse::Translator translator;
    ASSERT_EQ("", translator.ToCode({}));
}
