//
// Created by William Liu on 2019-08-08.
//

#include "JsonValue.h"
#include <assert.h>

using namespace ::lightjson;

JsonType JsonValue::getType() const {
  if (std::holds_alternative<nullptr_t>(val_))
    return JsonType::kNull;
  else if (std::holds_alternative<bool>(val_))
    return JsonType::kBool;
  else if (std::holds_alternative<double>(val_))
    return JsonType::kNumber;
  else if (std::holds_alternative<std::string>(val_))
    return JsonType::kString;
  else if (std::holds_alternative<std::vector<Json> >(val_))
    return JsonType::kArray;
  else
    return JsonType::kObject;
}

bool JsonValue::toBool() const {
  assert(std::holds_alternative<bool>(val_));
  return std::get<bool>(val_);
}

double JsonValue::toDouble() const {
  assert(std::holds_alternative<double>(val_));
  return std::get<double>(val_);
}

std::string JsonValue::toString() const {
  assert(std::holds_alternative<std::string>(val_));
  return std::get<std::string>(val_);
}

std::vector<Json> JsonValue::toArray() const {
  assert(std::holds_alternative<std::vector<Json> >(val_));
  return std::get<std::vector<Json> >(val_);
}

size_t JsonValue::size() const {
  assert(std::holds_alternative<std::vector<Json> >(val_));
  return std::get<std::vector<Json> >(val_).size();
}

const Json &JsonValue::operator[](size_t pos) const {
  assert(std::holds_alternative<std::vector<Json> >(val_));
  return std::get<std::vector<Json> >(val_)[pos];
}

Json &JsonValue::operator[](size_t pos) {
  assert(std::holds_alternative<std::vector<Json> >(val_));
  return std::get<std::vector<Json> >(val_)[pos];
}
