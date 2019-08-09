//
// Created by William Liu on 2019-08-08.
//

#ifndef LIGHTJSON_JSON_H
#define LIGHTJSON_JSON_H

#include <cstddef>
#include <string>
#include <memory>

namespace lightjson {

enum class JsonType {
  kNull,
  kBool,
  kNumber,
  kString,
  kArray,
  kObject
};

// Forward declaration for unique_ptr.
class JsonValue;

class Json {
 public:
  // ctors
  explicit Json(std::nullptr_t);
  explicit Json(bool);
  explicit Json(int val) : Json(val * 1.0f) {}
  explicit Json(double val);
  explicit Json(const char *cStr) : Json(std::string(cStr)) {}
  explicit Json(const std::string &);
  // Copy ctor and copy assignment
  Json(const Json &);
  Json &operator=(const Json &);
  // Move ctor and assignment
  Json(Json &&) noexcept;
  Json &operator=(Json &&) noexcept;
  // Dtor
  ~Json();

  static Json parse(const std::string &data, std::string &error);
  JsonType getType() const;
  bool isNull() const noexcept;
  bool isBool() const noexcept;
  bool isNumber() const noexcept;
  bool isString() const noexcept;
  bool toBool() const;
  double toDouble() const;
  std::string toString() const;

 private:
  void swap(Json &) noexcept;
  std::unique_ptr<JsonValue> value_;
};

} // namespace

#endif //LIGHTJSON_JSON_H