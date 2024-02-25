#include "router.hh"

const std::string RES_DIR = "res/";
const std::string GET_REQUEST = "GET";
const std::string HTTP_VERSION = "HTTP/1.1";
const int GET_REQUEST_OFFSET = 4;
const int HTTP_VERSION_OFFSET = 5;

std::optional<std::string>
Router::translateFromBufferToPath(std::string &buffer) const {
  std::size_t found = buffer.find(GET_REQUEST);

  if (found == std::string::npos) {
    return std::nullopt;
  }

  found = buffer.find(HTTP_VERSION);
  if (found == std::string::npos) {
    return std::nullopt;
  }

  std::string path =
      buffer.substr(GET_REQUEST_OFFSET, found - HTTP_VERSION_OFFSET);

  if (!isValidPath(path)) {
    return std::nullopt;
  }

  if (path == "/" || path.empty()) {
    path = RES_DIR + "index.html";
  } else {
    path = RES_DIR + path;
  }

  if (!fileExists(path)) {
    return std::nullopt;
  }

  return path;
};
