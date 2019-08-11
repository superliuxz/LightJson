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
  // Ctor
  explicit Parser(const char *data) : curr_(data) {}
  explicit Parser(const std::string &data) : curr_(data.c_str()) {}
  // Make the Parser uncopiable.
  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;

  Json parse();

 private:
  const char *curr_;

  Json parseValue();
  Json parseLiteral(const std::string &);
  Json parseNumber();
  Json parseString();
  Json parseArray();
  Json parseObject();

  std::string parseRawString();
  void parseWhiteSpace();
  int parse4hex(const char **);
  void error(const std::string &msg) const {
    throw JsonException(msg + ": " + curr_);
  }
};

} // namespace

#endif //LIGHTJSON_PARSER_H
