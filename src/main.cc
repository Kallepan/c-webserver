// Server side Cpp program to demonstrate Socket programming

#include "http_errors.hh"
#include "response.hh"
#include "router.hh"
#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// #define DEBUG

const int PORT = 8080;
const int BUFFER_SIZE = 30000;
const int MESSAGE_QUEUE_SIZE = 10;

void loadFile(const std::string &file_path, std::string &response_content) {
  /**
   * Load file from file_path into response_content
   * if file is not found, return false else return true
   */

  // we expect the file to exist
  std::ifstream file(file_path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  response_content = buffer.str();
}

void sendResponse(int new_socket, const std::string &response) {
  write(new_socket, response.c_str(), response.length());
  close(new_socket);

  std::cout << "------------------ Message sent -------------------\n";
}

void setupServer(int &server_fd, struct sockaddr_in &address, long &valread,
                 int &addrlen, int &new_socket) {
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) ==
      0) // AF_INET: IPv4, SOCK_STREAM: TCP, 0: IP
  {
    throw std::runtime_error("socket failed");
  }

  address.sin_family = AF_INET;         // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // Any address
  address.sin_port = htons(PORT);       // Port

  std::fill(std::begin(address.sin_zero), std::end(address.sin_zero),
            '\0'); // Fill with 0

  // Forcefully attaching socket to the defined port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) <
      0) // Bind socket to address
  {
    throw std::runtime_error("port bind failed");
  }
  if (listen(server_fd, MESSAGE_QUEUE_SIZE) < 0) // Listen for connections
  {
    throw std::runtime_error("listen");
  }
}

void cleanUp(int server_fd) {
  if (server_fd != -1) {
    close(server_fd);
  }
}

void handleConnection(int new_socket, Router &router, long valread) {
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
  try {
    router.translateFromBufferToPath(buffer, path);
    router.decodeRequestedPathToFilePath(path);
    loadFile(path, content);
    responseBuilder.setData(200, "OK", "text/html", content);
  } catch (const http_error::HTTPError &e) {
    if (router.fileExists(e.defaultFileToServe())) {
      loadFile(e.defaultFileToServe(), content);
    } else {
      content = e.defaultContent();
    }

    responseBuilder.setData(e.getStatusCode(), e.getStatusMessage(),
                            "text/html", content);
  }

  // print response
  responseBuilder.printResponse();

  // send header and content
  sendResponse(new_socket, responseBuilder.getResponse());
}

void runServer() {
  int server_fd = -1;
  long valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address), new_socket;
  try {
    setupServer(server_fd, address, valread, addrlen, new_socket);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  Router router = Router();

  std::vector<std::future<void>> futures;
  while (true) {
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      std::cerr << "accept \n";
      continue;
    }

    // Create a new thread for each connection
    futures.push_back(std::async(
        // call handleConnection with new_socket
        handleConnection, new_socket, std::ref(router), std::ref(valread)));
  }

  cleanUp(server_fd);
}

int main() {
  runServer();
  return 0;
}