#include "io/io.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace morse {
namespace io {

static std::vector<std::string> ParseWords(const std::string& data) {
    std::istringstream iss(data);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(words));
    return words;
}

std::vector<std::string> ReadWords() {
    std::string data;
    std::string line;
    while (std::getline(std::cin, line)) {
        data += line;
    }
    return ParseWords(data);
}

std::vector<std::string> ReadWords(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        std::runtime_error("'" + filepath + "' does not exist");
    }

    std::ifstream input_file(filepath);
    if (!input_file) {
        throw std::runtime_error("'" + filepath + "' cannot be opened");
    }

    std::stringstream buffer;
    buffer << input_file.rdbuf();

    return ParseWords(buffer.str());
}

void WriteToFile(const std::string& data, const std::string& filepath) {
    std::ofstream output_file(filepath);
    if (!output_file) {
        throw std::runtime_error("'" + filepath + "' cannot be opened");
    }

    output_file << data;
}

}  // namespace io
}  // namespace morse
