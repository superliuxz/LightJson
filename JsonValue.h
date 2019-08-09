//
// Created by William Liu on 2019-08-08.
//

#ifndef LIGHTJSON_JSONVALUE_H
#define LIGHTJSON_JSONVALUE_H

#include <stddef.h>
#include <variant>
#include "Json.h"

namespace lightjson {

class JsonValue {
 public:
  // Ctor
  explicit JsonValue(nullptr_t val) : val_(val) {}
  explicit JsonValue(bool val) : val_(val) {}
  explicit JsonValue(double val) : val_(val) {}
  explicit JsonValue(const std::string &val) : val_(val) {}
  // Move ctor.
  explicit JsonValue(std::string &&val) : val_(std::move(val)) {}

  // Dtor
  ~JsonValue() = default;

  JsonType getType() const;
  bool toBool() const;
  double toDouble() const;
  std::string toString() const;

 private:
  std::variant<nullptr_t, bool, double, std::string> val_;
};

} // namespace

#endif //LIGHTJSON_JSONVALUE_H
