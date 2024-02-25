#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

#ifndef HTTP_ERRORS_HH
#define HTTP_ERRORS_HH
namespace http_error {
class HTTPError : public std::runtime_error {
private:
  std::string error;

public:
  explicit HTTPError(const std::string &error) : std::runtime_error(error) {
    this->error = error;
  }

  std::string getError() { return this->error; }

  // Pure virtual functions
  virtual std::string
  defaultFileToServe() const = 0; // returns the default file to serve
  virtual std::string defaultContent()
      const = 0; // returns the default content if the file does not exist

  virtual int getStatusCode() const = 0; // returns the status code
  virtual std::string
  getStatusMessage() const = 0; // returns the status message
};

// 400 Bad Request
class BadRequestError : public HTTPError {
public:
  explicit BadRequestError(const std::string &message = "Bad Request")
      : HTTPError(message) {}

  int getStatusCode() const override { return 400; }

  std::string getStatusMessage() const override { return "Bad Request"; }

  std::string defaultFileToServe() const override { return "res/400.html"; }

  std::string defaultContent() const override {
    return "<html><body><h1>400 Bad Request</h1></body></html>";
  }
};

// 404 Not Found
class NotFoundError : public HTTPError {
public:
  explicit NotFoundError(const std::string &message = "Not Found")
      : HTTPError(message) {}

  int getStatusCode() const override { return 404; }
  std::string getStatusMessage() const override { return "Not Found"; }

  std::string defaultFileToServe() const override { return "res/404.html"; }

  std::string defaultContent() const override {
    return "<html><body><h1>404 Not Found</h1></body></html>";
  }
};
} // namespace http_error

#endif // HTTP_ERRORS_HH