//
// Created by William Liu on 2019-08-08.
//

#include <memory>
#include "Json.h"
#include "JsonValue.h"
#include "Parser.h"

namespace lightjson {

Json::Json(nullptr_t) : value_(std::make_unique<JsonValue>(nullptr)) {}
Json::Json(bool val) : value_(std::make_unique<JsonValue>(val)) {}
Json::Json(double val) : value_(std::make_unique<JsonValue>(val)) {}

Json Json::parse(const std::string &data, std::string &error) {
  try {
    Parser p(data);
    return p.parse();
  } catch (JsonException &e) {
    return Json(nullptr);
  }
}

JsonType Json::getType() const {
  return value_->getType();
}

}

