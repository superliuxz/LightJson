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
  explicit JsonValue(const Json::array &val) : val_(val) {}
  explicit JsonValue(const Json::object &val) : val_(
      val) {}
  // Delete copy ctor and assignment because |Json| has a unique_ptr of
  // |JsonValue|. unique_ptr cannot be copied.
  JsonValue(const JsonValue &) = delete;
  JsonValue &operator=(const JsonValue &) = delete;
  // Move ctor
  explicit JsonValue(std::string &&val) : val_(std::move(val)) {}
  explicit JsonValue(Json::array &&val) : val_(std::move(val)) {}
  explicit JsonValue(Json::object &&val)
      : val_(std::move(val)) {}
  // Dtor
  ~JsonValue() = default;

  JsonType getType() const;
  bool toBool() const;
  double toDouble() const;
  std::string toString() const;
  Json::array toArray() const;
  Json::object toObject() const;

  size_t size() const;
  // Random access
  const Json &operator[](size_t) const;
  Json &operator[](size_t);
  // Key-val access
  const Json &operator[](const std::string &) const;
  Json &operator[](const std::string &);

 private:
  std::variant<nullptr_t,
               bool,
               double,
               std::string,
               Json::array,
               Json::object> val_;
};

} // namespace

#endif //LIGHTJSON_JSONVALUE_H
