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
  else if (std::holds_alternative<Json::array>(val_))
    return JsonType::kArray;
  else {
    assert(std::holds_alternative<Json::object>(val_));
    return JsonType::kObject;
  }
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

Json::array JsonValue::toArray() const {
  assert(std::holds_alternative<Json::array>(val_));
  return std::get<Json::array>(val_);
}

Json::object JsonValue::toObject() const {
  assert(std::holds_alternative<Json::object>(val_));
  return std::get<Json::object>(val_);
}

size_t JsonValue::size() const {
  if (std::holds_alternative<Json::array>(val_))
    return std::get<Json::array>(val_).size();
  assert(std::holds_alternative<Json::object>(val_));
  return std::get<Json::object>(val_).size();
}

const Json &JsonValue::operator[](size_t pos) const {
  assert(std::holds_alternative<Json::array>(val_));
  return std::get<Json::array>(val_)[pos];
}

Json &JsonValue::operator[](size_t pos) {
  assert(std::holds_alternative<Json::array>(val_));
  return std::get<Json::array>(val_)[pos];
}

const Json &JsonValue::operator[](const std::string &key) const {
  assert(std::holds_alternative<Json::object>(val_));
  // https://en.cppreference.com/w/cpp/container/unordered_map/operator_at
  // operator[] is non-const because it inserts the key if it doesn't exist.
  // If this behavior is undesirable or if the container is const, at() may be
  // used.
  return std::get<Json::object>(val_).at(key);
}

Json &JsonValue::operator[](const std::string &key) {
  assert(std::holds_alternative<Json::object>(val_));
  return std::get<Json::object>(val_).at(key);
}
