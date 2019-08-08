#ifndef LIGHTJSON_LIGHTJSON_H
#define LIGHTJSON_LIGHTJSON_H

typedef enum {
  kNull,
  kTrue,
  kFalse,
  kNumber,
  kString,
  kArray,
  kObject
} JsonType;

/*
 * Representing the parsed value.
 */
struct JsonValue {
  double number;
  JsonType type;
};

/*
 * JSON-text = ws value ws (https://tools.ietf.org/html/rfc7159)
 * so:
 * if value is empty -> PARSE_EXPECT_VALUE;
 * if value is not one of the JsonType -> PARSE_INVALID_VALUE;
 * if value is not singular -> PARSE_ROOT_NOT_SINGULAR.
 */
enum {
  PARSE_OK = 0,
  PARSE_EXPECT_VALUE,
  PARSE_INVALID_VALUE,
  PARSE_ROOT_NOT_SINGULAR,
  PARSE_NUMBER_TOO_BIG
};

JsonType get_type(const JsonValue* value);

double get_number(const JsonValue* value);

int parse(JsonValue* value, const char* json);

#endif //LIGHTJSON_LIGHTJSON_H