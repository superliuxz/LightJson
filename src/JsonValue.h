//
// Created by William Liu on 2019-08-08.
//

#ifndef LIGHTJSON_JSONVALUE_H
#define LIGHTJSON_JSONVALUE_H

#include <stddef.h>
#include <variant>
#include "../include/Json.h"
#include "JsonType.h"
#include "JsonException.h"

namespace lightjson {

class JsonValue {
 public:
  virtual ~JsonValue() = default;

  virtual bool toBool() const { throw JsonException("Not implemented"); }
  virtual double toDouble() const { throw JsonException("Not implemented"); }
  virtual std::string toString() const { throw JsonException("Not implemented"); }
  virtual Json::array toArray() const { throw JsonException("Not implemented"); }
  virtual Json::object toObject() const { throw JsonException("Not implemented"); }

  virtual Json &operator[](size_t) { throw JsonException("Not implemented"); }
  virtual const Json &operator[](size_t) const {
    throw JsonException("Not implemented");
  }
  virtual Json &operator[](const std::string &) {
    throw JsonException("Not implemented");
  }
  virtual const Json &operator[](const std::string &) const {
    throw JsonException("Not implemented");
  }
  virtual size_t size() const noexcept { return -1; }
  virtual JsonType type() const = 0;
};

template<typename T, JsonType U>
class Value : public JsonValue {
 public:
  explicit Value(const T &val) : val_(val) {}
  explicit Value(T &&val) : val_(val) {}
  JsonType type() const final { return U; }
 protected:
  T val_;
};

class JsonNull : public Value<std::nullptr_t, JsonType::kNull> {
 public:
  explicit JsonNull(std::nullptr_t) : Value(nullptr) {}
};

class JsonBool : public Value<bool, JsonType::kBool> {
 public:
  explicit JsonBool(bool val) : Value(val) {}
  bool toBool() const override { return val_; }
};

class JsonDouble : public Value<double, JsonType::kNumber> {
 public:
  explicit JsonDouble(double val) : Value(val) {}
  double toDouble() const override { return val_; }
};

class JsonString : public Value<std::string, JsonType::kString> {
 public:
  explicit JsonString(const std::string &val) : Value(val) {}
  explicit JsonString(std::string &&val) : Value(val) {}
  std::string toString() const override { return val_; }
};

class JsonArray : public Value<Json::array, JsonType::kArray> {
 public:
  explicit JsonArray(const Json::array &val) : Value(val) {}
  explicit JsonArray(Json::array &&val) : Value(val) {}
  Json::array toArray() const override { return val_; }
  const Json &operator[](size_t i) const override { return val_[i]; }
  Json &operator[](size_t i) override { return val_[i]; }
  size_t size() const noexcept override { return val_.size(); }
};

class JsonObject : public Value<Json::object, JsonType::kObject> {
 public:
  explicit JsonObject(const Json::object &val) : Value(val) {}
  explicit JsonObject(Json::object &&val) : Value(val) {}
  Json::object toObject() const override { return val_; }
  const Json &operator[](const std::string &i) const override { return val_.at(i); }
  Json &operator[](const std::string &i) override { return val_.at(i); }
  size_t size() const noexcept override { return val_.size(); }
};

} // namespace

#endif //LIGHTJSON_JSONVALUE_H
