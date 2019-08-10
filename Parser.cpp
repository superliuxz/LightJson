//
// Created by William Liu on 2019-08-08.
//

#include <cstring>
#include <math.h>
#include <locale>
#include <codecvt>
#include <sstream>
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
  std::wstring_convert<std::codecvt_utf8<wchar_t> > convert;
  std::ostringstream oss;
  const char *p = curr_;
  for (;;) {
    switch (*++p) {
      // closing quote.
      case '\"': {
        curr_ = ++p;
        const char *cstr = oss.str().c_str();
        const auto size = oss.tellp();
        return Json(std::string(cstr, size));
      }
        // Escape.
      case '\\':
        switch (*++p) {
          case '\"': {
            oss << '\"';
            break;
          }
          case '\\': {
            oss << '\\';
            break;
          }
          case '/': {
            oss << '/';
            break;
          }
          case 'b': {
            oss << '\b';
            break;
          }
          case 'f': {
            oss << '\f';
            break;
          }
          case 'n': {
            oss << '\n';
            break;
          }
          case 't': {
            oss << '\t';
            break;
          }
          case 'r': {
            oss << '\r';
            break;
          }
          case 'u': {
            int highSurrogate = parse4hex(&p);
            // Try parse \uXXXX\uYYYY
            // https://en.wikipedia.org/wiki/UTF-16#U+D800_to_U+DFFF
            if (0xd800 <= highSurrogate && highSurrogate <= 0xdbff) {
              if (*++p != '\\') error("Invalid surrogate pair");
              if (*++p != 'u') error("Invalid surrogate pair");
              int lowSurrogate = parse4hex(&p);
              if (lowSurrogate < 0xdc00 || lowSurrogate > 0xdfff)
                error("Invalid unicode surrogate");
              highSurrogate =
                  (((highSurrogate - 0xd800) << 10) | (lowSurrogate - 0xdc00))
                      + 0x10000;
            }
            std::string utf8 = convert.to_bytes(highSurrogate);
            // cannot convert to char* here because if utf8='\0', stf8.c_str()
            // will terminate right away. Therefore manually push char into the
            // stream.
            for (auto &ch: utf8) oss << ch;
            break;
          }
          default: error("Invalid escape character");
        }
        break;
      case '\0':error("Missing quotation mark");
      default:
        if (static_cast<unsigned char>(*p) < 0x20)
          error("Invalid char");
        oss << *p;
    }
  }
}

void Parser::parseWhiteSpace() {
  while (*curr_ == ' ' || *curr_ == '\t' || *curr_ == '\n' || *curr_ == '\r')
    curr_++;
}

int Parser::parse4hex(const char **p) {
  int u = 0;
  for (int i = 0; i < 4; ++i) {
    auto curr = static_cast<unsigned>(toupper(*++*p));
    u <<= 4;
    if ('0' <= curr && curr <= '9') u |= (curr - '0');
    else if ('A' <= curr && curr <= 'F') u |= (curr - 'A' + 10);
    else {
      error("Invalid hex value");
    }
  }
  return u;
}
