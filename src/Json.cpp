//
// Created by William Liu on 2019-08-08.
//

#include <memory>
#include <sstream>
#include <iomanip>
#include "../include/Json.h"
#include "JsonValue.h"
#include "Parser.h"
#include "JsonType.h"

using namespace ::lightjson;

// Ctors
Json::Json() : Json(nullptr) {}
Json::Json(nullptr_t) : value_(std::make_unique<JsonNull>(nullptr)) {}
Json::Json(bool val) : value_(std::make_unique<JsonBool>(val)) {}
Json::Json(double val) : value_(std::make_unique<JsonDouble>(val)) {}
Json::Json(const std::string &val) : value_(std::make_unique<JsonString>(val)) {}
Json::Json(const Json::array &val)
    : value_(std::make_unique<JsonArray>(val)) {}
Json::Json(const lightjson::Json::object &val)
    : value_(std::make_unique<JsonObject>(val)) {}
// Copy ctor
Json::Json(const Json &o) {
  switch (o.getType()) {
    case JsonType::kNull: {
      value_ = std::make_unique<JsonNull>(nullptr);
      break;
    }
    case JsonType::kNumber: {
      value_ = std::make_unique<JsonDouble>(o.toNumber());
      break;
    }
    case JsonType::kBool: {
      value_ = std::make_unique<JsonBool>(o.toBool());
      break;
    }
    case JsonType::kString: {
      value_ = std::make_unique<JsonString>(o.toString());
      break;
    }
    case JsonType::kArray: {
      value_ = std::make_unique<JsonArray>(o.toArray());
      break;
    }
    case JsonType::kObject: {
      value_ = std::make_unique<JsonObject>(o.toObject());
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

std::string Json::serialize() const {
  switch (value_->type()) {
    case JsonType::kNull: return "null";
    case JsonType::kBool: return value_->toBool() ? "true" : "false";
    case JsonType::kNumber: return serializeNumber();
    case JsonType::kString: return serializeString();
    case JsonType::kArray: return serializeArray();
    default:return serializeObject();
  }
}

JsonType Json::getType() const {
  return value_->type();
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

std::string Json::serializeNumber() const {
  char buf[32];
  snprintf(buf, sizeof(buf), "%.17g", value_->toDouble());
  return buf;
}

std::string Json::serializeString() const {
  std::string retVal{"\""};
  for (auto ch: value_->toString()) {
    switch (ch) {
      case '\"': {
        retVal += "\\\"";
        break;
      }
      case '\\': {
        retVal += "\\\\";
        break;
      }
      case '\b': {
        retVal += "\\b";
        break;
      }
      case '\f': {
        retVal += "\\f";
        break;
      }
      case '\n': {
        retVal += "\\n";
        break;
      }
      case '\r': {
        retVal += "\\r";
        break;
      }
      case '\t': {
        retVal += "\\t";
        break;
      }
      default: {
        auto v = static_cast<unsigned>(ch);
        if (v < 0x20) {
          std::stringstream ss;
          ss << "\\u" << std::setfill('0') << std::setw(4) << v;
          retVal += ss.str();
        } else {
          retVal += ch;
        }
      }
    }
  }
  return retVal + '"';
}

std::string Json::serializeArray() const {
  std::string retVal{"["};
  for (auto i = 0; i != value_->size(); ++i) {
    if (i > 0) retVal += ", ";
    retVal += (*this)[i].serialize();
  }
  return retVal + "]";
}

std::string Json::serializeObject() const {
  std::string retVal{"{"};
  bool seen1st = false;
  for (const auto &p: value_->toObject()) {
    if (!seen1st) seen1st = true;
    else retVal += ", ";
    retVal += "\"" + p.first + "\"";
    retVal += ": ";
    retVal += p.second.serialize();
  }
  return retVal + "}";
}