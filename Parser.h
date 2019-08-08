//
// Created by William Liu on 2019-08-08.
//

#ifndef LIGHTJSON_PARSER_H
#define LIGHTJSON_PARSER_H

#include <string>
#include "Json.h"
#include "JsonException.h"

namespace lightjson {

class Parser {
 public:
  Parser(const std::string &data) : curr_(data.c_str()) {}
  Json parse();

 private:
  const char *curr_;

  void parseWhiteSpace();
  Json parseValue();
  Json parseLiteral(const std::string &literal);
  Json parseNumber();
  void error(const std::string &msg) const {
    throw JsonException(msg + ": " + curr_);
  }
};

} // namespace

#endif //LIGHTJSON_PARSER_H
