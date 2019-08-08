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
  explicit JsonValue(nullptr_t val) : val_(val) {}
  explicit JsonValue(bool val) : val_(val) {}
  explicit JsonValue(double val) : val_(val) {}

  ~JsonValue() = default;

  JsonType getType() const;
  nullptr_t toNull() const;
  bool toBool() const;
  double toDouble() const;

 private:
  std::variant<nullptr_t, bool, double> val_;
};

} // namespace

#endif //LIGHTJSON_JSONVALUE_H
