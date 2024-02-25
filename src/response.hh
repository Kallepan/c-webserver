#pragma once

#ifndef RESPONSE_HH
#define RESPONSE_HH
#include <iostream>
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
  Response() { std::cout << "Response object created\n"; }
  ~Response() { std::cout << "Response object destroyed\n"; }

  void printResponse() {
#ifdef DEBUG
    std::cout << this->getResponse() << "\n";
#else
    std::cout << "Response set\n";
#endif
  };

  std::string getResponse() const {
    std::string response = "";
    response += "HTTP/1.1 " + std::to_string(this->status_code) + " " +
                this->status_message + "\n";
    response += "Content-Type: " + this->content_type + "\n";
    response += "Content-Length: " + std::to_string(this->content_length) +
                "\n\n"; // note the double line feed (RFC 2616 requires this)
    response += this->content;
    return response;
  }

  void setData(int status_code, std::string status_message,
               std::string content_type, std::string content) {
    this->status_code = status_code;
    this->status_message = status_message;
    this->content_type = content_type;
    this->content = content;
    this->content_length = content.length();
  }
};

/**
 * The RespnseBuilder class is responsible for constructing the response.
 */
class ResponseBuilder {
private:
  Response *response;

public:
  ResponseBuilder() { this->response = new Response(); }
  ~ResponseBuilder() { delete response; }

  void setData(int status_code, std::string status_message,
               std::string content_type, std::string content) {
    this->response->setData(status_code, status_message, content_type, content);
  }

  std::string getResponse() const { return this->response->getResponse(); }

  void printResponse() { this->response->printResponse(); }
};

#endif // RESPONSE_HH