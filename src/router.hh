#ifndef ROUTER_HH
#define ROUTER_HH
#include "http_errors.hh"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

class Router {
public:
  /**
   * @brief Translate the buffer from the request to a cleanedPath
   *
   * This function provides a way to access the requested path from a buffer
   * and clean it from forbidden substrings
   */
  void translateFromBufferToPath(std::string &buffer, std::string &path) const;

  /**
   * @brief Decode the requested path to a file path
   *
   * This function takes a cleand path requested from the server and
   * returns the underlying file path to be served
   */
  void decodeRequestedPathToFilePath(std::string &path) const;

  /**
   * @brief Check if a file exists
   *
   * This function checks if a file exists in the file system
   */
  bool fileExists(const std::string &file_path) const {
    return std::filesystem::exists(file_path);
  };

private:
  /**
   * @brief Clean the path from forbidden substrings
   */
  void cleanPath(const std::string &path) const;
};

#endif // ROUTER_HH