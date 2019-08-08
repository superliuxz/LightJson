//
// Created by William Liu on 2019-08-08.
//

#include <cstring>
#include <limits>
#include "Parser.h"

namespace lightjson {

constexpr bool isDigit(const char *ch) { return '0' <= *ch && *ch <= '9'; }
constexpr bool isDigit1to9(const char *ch) { return '1' <= *ch && *ch <= '9'; }

Json Parser::parse() {
  parseWhiteSpace();
  auto json = parseValue();
  parseWhiteSpace();
  if (*curr_)
    error("Root not singular");
  return json;
}

void Parser::parseWhiteSpace() {
  while (*curr_ == ' ' || *curr_ == '\t' || *curr_ == '\n' || *curr_ == '\r')
    curr_++;
}

Json Parser::parseValue() {
  switch (*curr_) {
    case 'n': return parseLiteral("null");
    case 't': return parseLiteral("true");
    case 'f': return parseLiteral("false");
    case '\0': error("Expect value");
    default: return parseNumber();
  }
}

Json Parser::parseLiteral(const std::string &literal) {
  if (strncmp(curr_, literal.c_str(), literal.size()) != 0) {
    error("Invalid value");
  }
  curr_ += literal.size();
  if (literal[0] == 't') return Json(true);
  if (literal[0] == 'f') return Json(false);
  return Json(nullptr);
}

Json Parser::parseNumber() {
  const char *start = curr_;
  // Manually check if the number is valid, and move |curr| to the end.
  if (*curr_ == '-') ++curr_;
  // if 0 leads, the number must have a decimal component.
  if (*curr_ == '0') ++curr_;
  else {
    // else, leading digit cannot be 0 (or anything else).
    if (!isDigit1to9(curr_)) error("Invalid value");
    while (isDigit(++curr_));
  }
  if (*curr_ == '.') {
    if (!isDigit(++curr_)) error("Invalid value");
    while (isDigit(++curr_));
  }
  if (*curr_ == 'e' || *curr_ == 'E') {
    ++curr_;
    // +- sign after exponent is optional.
    if (*curr_ == '+' || *curr_ == '-') ++curr_;
    if (!isDigit(curr_)) error("Invalid value");
    while (isDigit(++curr_));
  }
  auto val = strtod(start, nullptr);
  if (val == std::numeric_limits<double>::max()
      || val == std::numeric_limits<double>::min())
    error("Number too big");
  return Json(val);
}

} // namespace

