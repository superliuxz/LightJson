//
// Created by William Liu on 2019-08-08.
//

#include <memory>
#include "Json.h"
#include "JsonValue.h"
#include "Parser.h"

using namespace ::lightjson;

// Ctors
Json::Json() : Json(nullptr) {}
Json::Json(nullptr_t) : value_(std::make_unique<JsonValue>(nullptr)) {}
Json::Json(bool val) : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(double val) : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(const std::string &val) : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(const Json::array &val)
    : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(const lightjson::Json::object &val)
    : value_(std::make_unique<JsonValue>(val)) {}
// Copy ctor
Json::Json(const Json &o) {
  switch (o.getType()) {
    case JsonType::kNull: {
      value_ = std::make_unique<JsonValue>(nullptr);
      break;
    }
    case JsonType::kNumber: {
      value_ = std::make_unique<JsonValue>(o.toNumber());
      break;
    }
    case JsonType::kBool: {
      value_ = std::make_unique<JsonValue>(o.toBool());
      break;
    }
    case JsonType::kString: {
      value_ = std::make_unique<JsonValue>(o.toString());
      break;
    }
    case JsonType::kArray: {
      value_ = std::make_unique<JsonValue>(o.toArray());
      break;
    }
    case JsonType::kObject: {
      value_ = std::make_unique<JsonValue>(o.toObject());
      break;
    }
  }
}
// Copy assignment
Json &Json::operator=(const Json &o) {
  Json temp(o);
  swap(temp);
  return *this;
}
// Move ctor
Json::Json(Json &&o) noexcept : value_(std::move(o.value_)) {
  o.value_ = nullptr;
}
// Move assignment
Json &Json::operator=(Json &&o) noexcept = default;
// Dtor
// This has to be explicitly declared, otherwise would get an error:
// In instantiation of 'void std::default_delete<_Tp>::operator()(_Tp*) const [with _Tp = lightjson::JsonValue]':
// required from 'std::unique_ptr<_Tp, _Dp>::~unique_ptr() [with _Tp = lightjson::JsonValue; _Dp = std::default_delete<lightjson::JsonValue>]'
// /Users/will/Desktop/LightJson/Json.h:26:7:   required from here
// invalid application of 'sizeof' to incomplete type 'lightjson::JsonValue'
// Not sure why this is...
Json::~Json() = default;
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
bool Json::isArray() const noexcept { return getType() == JsonType::kArray; }
bool Json::isObject() const noexcept { return getType() == JsonType::kObject; }

bool Json::toBool() const { return value_->toBool(); }
double Json::toNumber() const { return value_->toDouble(); }
std::string Json::toString() const { return value_->toString(); }
Json::array Json::toArray() const { return value_->toArray(); }
Json::object Json::toObject() const { return value_->toObject(); }

size_t Json::size() const { return value_->size(); }

Json &Json::operator[](size_t pos) {
  return value_->operator[](pos);
}
const Json &Json::operator[](size_t pos) const {
  return value_->operator[](pos);
}

Json &Json::operator[](const std::string &key) {
  return value_->operator[](key);
}

const Json &Json::operator[](const std::string &key) const {
  return value_->operator[](key);
}

bool Json::operator==(const lightjson::Json &o) const {
  if (this->getType() != o.getType()) return false;
  switch (this->getType()) {
    case JsonType::kNull: return true;
    case JsonType::kBool: return this->toBool() == o.toBool();
    case JsonType::kNumber: return this->toNumber() == o.toNumber();
    case JsonType::kString: return this->toString() == o.toString();
    case JsonType::kArray: return this->toArray() == o.toArray();
    case JsonType::kObject: return this->toObject() == o.toObject();
    default: return false;
  }
}
// Private
void Json::swap(Json &o) noexcept {
  std::swap(value_, o.value_);
}