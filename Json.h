//
// Created by William Liu on 2019-08-08.
//

#ifndef LIGHTJSON_JSON_H
#define LIGHTJSON_JSON_H

#include <cstddef>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

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
  using array = std::vector<Json>;
  using object = std::unordered_map<std::string, Json>;
  // ctors
  explicit Json();
  explicit Json(std::nullptr_t);
  explicit Json(bool);
  explicit Json(int val) : Json(val * 1.0f) {}
  explicit Json(double val);
  explicit Json(const char *cStr) : Json(std::string(cStr)) {}
  explicit Json(const std::string &);
  explicit Json(const Json::array &);
  explicit Json(const Json::object &);
  // Copy ctor and copy assignment
  Json(const Json &);
  Json &operator=(const Json &);
  // Move ctor and assignment
  Json(Json &&) noexcept;
  Json &operator=(Json &&) noexcept;
  // Dtor
  ~Json();
  // Parse and serialize
  static Json parse(const std::string &data, std::string &error);
  std::string serialize() const;

  JsonType getType() const;
  bool isNull() const noexcept;
  bool isBool() const noexcept;
  bool isNumber() const noexcept;
  bool isString() const noexcept;
  bool isArray() const noexcept;
  bool isObject() const noexcept;

  bool toBool() const;
  double toNumber() const;
  std::string toString() const;
  Json::array toArray() const;
  Json::object toObject() const;

  size_t size() const;

  // operators
  // random access
  Json &operator[](size_t);
  const Json &operator[](size_t) const;
  // key-val access
  Json &operator[](const std::string &);
  const Json &operator[](const std::string &) const;
  // Comparison
  bool operator==(const Json &) const;
  inline bool operator!=(const Json &o) {
    return !(this->operator==(o));
  }
  // Stream
  inline std::ostream &operator<<(std::ostream &os) {
    return os << this->serialize();
  }

 private:
  void swap(Json &) noexcept;
  std::string serializeNumber() const;
  std::string serializeString() const;
  std::string serializeArray() const;
  std::string serializeObject() const;
  // PIMPL
  std::unique_ptr<JsonValue> value_;
};

} // namespace

#endif //LIGHTJSON_JSON_H
