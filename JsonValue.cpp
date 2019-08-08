//
// Created by William Liu on 2019-08-08.
//

#include "JsonValue.h"
#include <assert.h>

namespace lightjson {

JsonType JsonValue::getType() const {
  if (std::holds_alternative<nullptr_t>(val_))
    return JsonType::kNull;
  else if (std::holds_alternative<bool>(val_))
    return JsonType::kBool;
  else
    return JsonType::kNumber;
}

nullptr_t JsonValue::toNull() const {
  assert(std::holds_alternative<nullptr_t>(val_));
  return std::get<nullptr_t>(val_);
}

bool JsonValue::toBool() const {
  assert(std::holds_alternative<bool>(val_));
  return std::get<bool>(val_);
}

double JsonValue::toDouble() const {
  assert(std::holds_alternative<double>(val_));
  return std::get<double>(val_);
}

} // namespace