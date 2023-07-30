#include "io/io.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static const std::vector<std::string> kInputWords = {
    "Hello,", "World.", "This",         "is",
    "a",      "file",   "used",         "to",
    "test",   "the",    "translator's", "capabilities."};

static bool ResetOutputFile() {
    std::ofstream of;
    of.open("data/output.txt", std::ofstream::out | std::ofstream::trunc);
    return (of.is_open());
}

TEST(IoTest, ReadWordsFromStdinReturnsInputWords) {
    std::ifstream fake_stdin("data/input.txt");
    ASSERT_TRUE(fake_stdin.is_open());

    std::vector<std::string> words = morse::io::ReadWords(fake_stdin);
    ASSERT_EQ(words, kInputWords);
}

TEST(IoTest, ReadWordsFromStdinReturnsEmptyListWhenGivenNoInput) {
    std::ifstream fake_stdin("data/empty.txt");
    ASSERT_TRUE(fake_stdin.is_open());

    std::vector<std::string> empty_words;
    std::vector<std::string> words = morse::io::ReadWords(fake_stdin);
    ASSERT_EQ(empty_words, words);
}

TEST(IoTest, ReadWordsFromFileReturnsInputWords) {
    std::vector<std::string> words = morse::io::ReadWords("data/input.txt");
    ASSERT_EQ(words, kInputWords);
}

TEST(IoTest, ReadWordsFromFileReturnsEmptyListWhenGivenEmptyFile) {
    std::vector<std::string> empty_words;
    std::vector<std::string> words = morse::io::ReadWords("data/empty.txt");
    ASSERT_EQ(empty_words, words);
}

TEST(IoTest, ReadWordsFromFileThrowsRuntimeErrorWhenGivenInvalidFile) {
    ASSERT_THROW(morse::io::ReadWords("nonexistent.txt"), std::runtime_error);
}

TEST(IoTest, WriteToFileWritesDataToFileSuccessfully) {
    const std::string kData("this is a test");
    morse::io::WriteToFile(kData, "data/output.txt");

    /* read back the data that was written */
    std::ifstream written_file("data/output.txt");
    ASSERT_TRUE(written_file.is_open());
    std::stringstream buffer;
    buffer << written_file.rdbuf();
    ASSERT_EQ(buffer.str(), kData);

    ASSERT_TRUE(ResetOutputFile());
}

TEST(IoTest, WriteToFileOverwritesExistingFileData) {
    /* write and then verify the first string is written */
    const std::string kData1("first data string");
    morse::io::WriteToFile(kData1, "data/output.txt");
    std::ifstream written_file1("data/output.txt");
    ASSERT_TRUE(written_file1.is_open());
    std::stringstream buffer1;
    buffer1 << written_file1.rdbuf();
    ASSERT_EQ(buffer1.str(), kData1);

    /* overwrite the content of the first write with the second string */
    const std::string kData2("second data string");
    morse::io::WriteToFile(kData2, "data/output.txt");
    std::ifstream written_file2("data/output.txt");
    ASSERT_TRUE(written_file2.is_open());
    std::stringstream buffer2;
    buffer2 << written_file2.rdbuf();
    ASSERT_EQ(buffer2.str(), kData2);

    ASSERT_TRUE(ResetOutputFile());
}
