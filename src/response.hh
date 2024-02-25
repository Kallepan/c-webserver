#ifndef RESPONSE_HH
#define RESPONSE_HH
#include <iostream>
#include <memory>
#include <string>

/**
 * The Response class specifies the response.
 *
 */
class Response {
private:
  int status_code;
  std::string status_message;
  int content_length;
  std::string content_type;
  std::string content;

public:
#ifdef DEBUG
  Response() { std::cout << "Response object created\n"; }
  ~Response() { std::cout << "Response object destroyed\n"; }
#endif

  std::string getResponse() const {
    std::string response = "";
    response +=
        "HTTP/1.1 " + std::to_string(status_code) + " " + status_message + "\n";
    response += "Content-Type: " + content_type + "\n";
    response += "Content-Length: " + std::to_string(content_length) +
                "\n\n"; // note the double line feed (RFC 2616 requires this)
    response += content;
    return response;
  }

  void setData(int status_code, std::string status_message,
               std::string content_type, std::string content) {
    this->status_code = status_code;
    this->status_message = std::move(status_message);
    this->content_type = std::move(content_type);
    this->content = std::move(content);
    this->content_length = this->content.length();
  }
};

/**
 * The RespnseBuilder class is responsible for constructing the response.
 */
class ResponseBuilder {
private:
  std::unique_ptr<Response> response;

public:
  ResponseBuilder() : response(std::make_unique<Response>()){};

  void setData(int status_code, std::string status_message,
               std::string content_type, std::string content) {
    this->response->setData(status_code, std::move(status_message),
                            std::move(content_type), std::move(content));
  }

  std::string getResponse() const { return response->getResponse(); }
};

#endif // RESPONSE_HH