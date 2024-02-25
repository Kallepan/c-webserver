#include "router.hh"

const std::string RES_DIR = "res/";
const std::string GET_REQUEST = "GET";
const std::string HTTP_VERSION = "HTTP/1.1";
const std::vector<std::string> forbiddenSubstrings = {"..", "//", "~"};
const int GET_REQUEST_OFFSET = 4;
const int HTTP_VERSION_OFFSET = 5;

void Router::translateFromBufferToPath(std::string &buffer,
                                       std::string &path) const {
  std::size_t found = buffer.find(GET_REQUEST);

  if (found == std::string::npos) {
    throw http_error::BadRequestError();
  }

  found = buffer.find(HTTP_VERSION);
  if (found == std::string::npos) {
    throw http_error::BadRequestError();
  }

  path = buffer.substr(GET_REQUEST_OFFSET, found - HTTP_VERSION_OFFSET);

  this->cleanPath(path);
};

void Router::cleanPath(const std::string &path) const {
  for (const auto &forbiddenSubstring : forbiddenSubstrings) {
    if (path.find(forbiddenSubstring) != std::string::npos) {
      std::cout << "Directory traversal detected\n";
      throw http_error::BadRequestError();
    }
  }
};

void Router::decodeRequestedPathToFilePath(std::string &path) const {
  // TODO: Implement a better way to decode the requested path to a file path
  if (path == "/" || path.empty()) {
    path = RES_DIR + "index.html";
    return;
  }

  throw http_error::NotFoundError();
};