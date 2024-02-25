// Server side Cpp program to demonstrate Socket programming

#include "file.hh"
#include "response.hh"
#include "router.hh"
#include "state_manager.hh"
#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <optional>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// #define DEBUG

const int PORT = 8080;
const int BUFFER_SIZE = 30000;
const int MESSAGE_QUEUE_SIZE = 10;

std::optional<std::string> loadFile(const std::string &file_path) {
  /**
   * Load file from file_path into response_content
   * if file is not found, return false else return true
   */

  File file(file_path);
  return file.readAll();
}

void sendResponse(int new_socket, const std::string &response) {
  write(new_socket, response.c_str(), response.length());
  std::cout << "+++++++++++++++++++ Message sent +++++++++++++++++++\n";
  std::cout << "+++++++++++++++++++ Closing connection ++++++++++++++++\n\n";
}

void setupServer(int &server_fd, struct sockaddr_in &address, long &valread) {
  // Creating socket file descriptor
  // AF_INET: IPv4, SOCK_STREAM: TCP, 0: IP
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    throw std::runtime_error("socket failed");
  }

  address.sin_family = AF_INET;         // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // Any address
  address.sin_port = htons(PORT);       // Port

  // Fill with 0
  std::fill(std::begin(address.sin_zero), std::end(address.sin_zero), '\0');

  // Forcefully attaching socket to the defined port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    throw std::runtime_error("port bind failed");
  }
  // Listen for connections
  if (listen(server_fd, MESSAGE_QUEUE_SIZE) < 0) {
    throw std::runtime_error("listen");
  }
}

void cleanUp(int server_fd) {
  if (server_fd != -1) {
    close(server_fd);
  }
}

void handleConnection(int new_socket, Router &router, long valread) {
  std::cout
      << "+++++++++++++++++++ Connection established +++++++++++++++++++\n";
  // Buffer for message
  std::string buffer(BUFFER_SIZE, '\0');

  // Read message into buffer
  valread = read(new_socket, &buffer[0], buffer.size());
  ResponseBuilder responseBuilder = ResponseBuilder();

  // Resize buffer to message size
  buffer.resize(valread);

  // Parse buffer and get GET request path
  std::string content;
  std::string path;

  std::optional<std::string> path_opt =
      router.translateFromBufferToPath(buffer);
  if (path_opt.has_value()) {
    path = path_opt.value();
  } else {
    responseBuilder.setData(400, "Bad Request", "text/html", "400 Bad Request");
  }

  // Load file from path
  std::optional<std::string> optional_content = loadFile(path);
  if (optional_content.has_value()) {
    content = optional_content.value();
    responseBuilder.setData(200, "OK", "text/html", content);
  } else {
    optional_content = loadFile("res/404.html");
    if (optional_content.has_value()) {
      content = optional_content.value();
      responseBuilder.setData(404, "Not Found", "text/html", content);
    } else {
      responseBuilder.setData(404, "Not Found", "text/html", "404 Not Found");
    }
  }

  // send header and content
  sendResponse(new_socket, responseBuilder.getResponse());
  close(new_socket);
}

void runServer() {
  int server_fd = -1;
  long valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  try {
    setupServer(server_fd, address, valread);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  Router router = Router();
  StateManager stateManager = StateManager();

  while (true) {
    int new_socket;
    std::cout << "\n+++++++++++++++++++ Waiting for new connection "
                 "+++++++++++++++++++\n\n";
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      std::cerr << "accept \n";
      continue;
    }

    // Create a new thread for each connection
    stateManager.addFuture(std::async(handleConnection, new_socket,
                                      std::ref(router), std::ref(valread)));
  }

  cleanUp(server_fd);
}

int main() {
  runServer();
  return 0;
}