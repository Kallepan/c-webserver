#ifndef ROUTER_HH
#define ROUTER_HH
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

const std::vector<std::string> forbiddenSubstrings = {"..", "//", "~"};

class Router {
public:
  /**
   * @brief Translate the buffer from the request to a cleanedPath
   *
   * This function provides a way to access the requested path from a buffer
   * and clean it from forbidden substrings
   */
  std::optional<std::string>
  translateFromBufferToPath(std::string &buffer) const;

private:
  /**
   * @brief Check if a file exists
   *
   * This function checks if a file exists in the file system
   */
  bool fileExists(const std::string &file_path) const {
    return std::filesystem::exists(file_path);
  };
  /**
   * @brief Clean the path from forbidden substrings
   */
  bool isValidPath(const std::string &path) const {
    for (const auto &forbiddenSubstring : forbiddenSubstrings) {
      if (path.find(forbiddenSubstring) != std::string::npos) {
        std::cout << "Directory traversal detected\n";
        return false;
      }
    }

    return true;
  };
};

#endif // ROUTER_HH