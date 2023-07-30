#ifndef IO_HPP_
#define IO_HPP_

#include <iostream>
#include <string>
#include <vector>

namespace morse {
namespace io {

/** @brief Read a list of words from STDIN. */
std::vector<std::string> ReadWords(std::istream& is = std::cin);

/**
 * @brief Read a list of words from the file at \p filepath
 *
 * @throws std::runtime_error When \p filepath is inaccessible.
 *
 * @returns A vector of words found in the parameter file in the order in which
 *          they appear.
 */
std::vector<std::string> ReadWords(const std::string& filepath);

/**
 * @brief Write \p data to the file at \p filepath
 *
 * @details If \p filepath exists, its contents will be overwritten.
 *
 * @throws std::runtime_error When \p filepath is inaccessible.
 */
void WriteToFile(const std::string& data, const std::string& filepath);

}  // namespace io
}  // namespace morse

#endif
