#ifndef FILE_HH
#define FILE_HH

#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

class File {
public:
  File(const std::string &file_path) : file_path(file_path) {}

  std::optional<std::string> readAll() {
    if (!std::filesystem::exists(file_path)) {
      return std::nullopt;
    }

    std::ifstream file(file_path);
    if (!file.is_open()) {
      return std::nullopt;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

private:
  std::string file_path;
};
#endif // FILE_HH