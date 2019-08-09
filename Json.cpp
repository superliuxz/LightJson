//
// Created by William Liu on 2019-08-08.
//

#include <memory>
#include "Json.h"
#include "JsonValue.h"
#include "Parser.h"

using namespace ::lightjson;

// Ctors
Json::Json(nullptr_t) : value_(std::make_unique<JsonValue>(nullptr)) {}
Json::Json(bool val) : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(double val) : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(const std::string &val) : value_(std::make_unique<JsonValue>(val)) {}
// Copy ctor
Json::Json(const Json &rhs) {
  switch (rhs.getType()) {
    case JsonType::kNull:value_ = std::make_unique<JsonValue>(nullptr);
    case JsonType::kNumber:value_ = std::make_unique<JsonValue>(rhs.toDouble());
    case JsonType::kBool:value_ = std::make_unique<JsonValue>(rhs.toBool());
    case JsonType::kString:value_ = std::make_unique<JsonValue>(rhs.toString());
  }
}
// Copy assignment
Json &Json::operator=(const Json &rhs) {
  Json temp(rhs);
  swap(temp);
  return *this;
}
// Move ctor
Json::Json(Json &&rhs) noexcept : value_(std::move(rhs.value_)) {
  rhs.value_ = nullptr;
}
// Move assignment
Json &Json::operator=(lightjson::Json &&rhs) noexcept = default;
// Dtor
Json::~Json() {}
// Public
Json Json::parse(const std::string &data, std::string &error) {
  try {
    Parser p(data);
    return p.parse();
  } catch (JsonException &e) {
    error = e.what();
    return Json(nullptr);
  }
}

JsonType Json::getType() const {
  return value_->getType();
}

bool Json::isNull() const noexcept { return getType() == JsonType::kNull; }
bool Json::isBool() const noexcept { return getType() == JsonType::kBool; }
bool Json::isNumber() const noexcept { return getType() == JsonType::kNumber; }
bool Json::isString() const noexcept { return getType() == JsonType::kString; }

bool Json::toBool() const { return value_->toBool(); }
double Json::toDouble() const { return value_->toDouble(); }
std::string Json::toString() const { return value_->toString(); }
// Private
void Json::swap(Json &rhs) noexcept {
  std::swap(value_, rhs.value_);
}