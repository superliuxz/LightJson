//
// Created by William Liu on 2019-08-08.
//

#ifndef LIGHTJSON_JSONEXCEPTION_H
#define LIGHTJSON_JSONEXCEPTION_H

#include <stdexcept>

namespace lightjson {

class JsonException : public std::runtime_error {
 public:
  explicit JsonException(const std::string &error) : runtime_error(error) {}
  const char *what() const noexcept override { return runtime_error::what(); }
};

} // namespace


#endif //LIGHTJSON_JSONEXCEPTION_H
