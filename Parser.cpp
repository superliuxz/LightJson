//
// Created by William Liu on 2019-08-08.
//

#include <cstring>
#include <math.h>
#include "Parser.h"

using namespace ::lightjson;

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

Json Parser::parseValue() {
  switch (*curr_) {
    case 'n': return parseLiteral("null");
    case 't': return parseLiteral("true");
    case 'f': return parseLiteral("false");
    case '\"': return parseString();
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
  if (std::abs(val) == HUGE_VAL)
    error("Number out of bound");
  return Json(val);
}

/*
string = quotation-mark *char quotation-mark
char = unescaped /
   escape (
       %x22 /          ; "    quotation mark  U+0022
       %x5C /          ; \    reverse solidus U+005C
       %x2F /          ; /    solidus         U+002F
       %x62 /          ; b    backspace       U+0008
       %x66 /          ; f    form feed       U+000C
       %x6E /          ; n    line feed       U+000A
       %x72 /          ; r    carriage return U+000D
       %x74 /          ; t    tab             U+0009
       %x75 4HEXDIG )  ; uXXXX                U+XXXX
escape = %x5C          ; \
quotation-mark = %x22  ; "
unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
 */
Json Parser::parseString() {
  std::string stack;
  const char *p = curr_;
  for (;;) {
    switch (*++p) {
      // closing quote.
      case '\"':curr_ = ++p;
        return Json(stack);
        // Escape.
      case '\\':
        switch (*++p) {
          case '\"': stack.push_back('\"');
            break;
          case '\\': stack.push_back('\\');
            break;
          case '/': stack.push_back('/');
            break;
          case 'b': stack.push_back('\b');
            break;
          case 'f': stack.push_back('\f');
            break;
          case 'n': stack.push_back('\n');
            break;
          case 't': stack.push_back('\t');
            break;
          case 'r': stack.push_back('\r');
            break;
          default: error("Invalid escape character");
        }
        break;
      case '\0':error("Missing quotation mark");
      default:
        if (static_cast<unsigned char>(*p) < 0x20)
          error("Invalid char");
        stack.push_back(*p);
    }
  }
}

void Parser::parseWhiteSpace() {
  while (*curr_ == ' ' || *curr_ == '\t' || *curr_ == '\n' || *curr_ == '\r')
    curr_++;
}
