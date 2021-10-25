#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

class ErrorHandler {
public:
  static ErrorHandler &GetInstance() {
    static ErrorHandler error_handler;
    return error_handler;
  }

  ErrorHandler(ErrorHandler const &) = delete;
  ErrorHandler(ErrorHandler &&) = delete;
  ErrorHandler &operator=(ErrorHandler const &) = delete;
  ErrorHandler &operator=(ErrorHandler &&) = delete;

  void PrintErrors() {
    if (error_messages.empty()) {
      return;
    }
    std::cout << "Errors are following:\n";
    for (const auto &error : error_messages) {
      std::cout << error << std::endl;
    }
  }
  void AddErrrors(std::string error) { error_messages.emplace_back(error); }

protected:
  std::deque<std::string> error_messages{};
  ErrorHandler() {}
  ~ErrorHandler() {}
};

#endif
