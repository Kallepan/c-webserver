// Router header file
#pragma once

#ifndef ROUTER_HH
#define ROUTER_HH
#include "http_errors.hh"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Router {
public:
  /**
   * @brief Constructor
   *
   * This constructor initializes the router with the given port.
   *
   * @param port The port to listen on
   */
  Router(){};

  /**
   * @brief Destructor
   *
   * This destructor destroys the router.
   */
  ~Router(){};

  /**
   * @brief Translate the given path
   *
   * This function translates the given path to the correct file.
   */
  void translateFromBufferToPath(std::string &buffer, std::string &path);
  void decodeRequestedPathToFilePath(std::string &path);
  bool fileExists(const std::string &file_path) {
    std::ifstream file(file_path);
    return file.good();
  }

private:
  void cleanPath(std::string &path);
};

#endif // ROUTER_HH