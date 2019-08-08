#include <iostream>
#include <math.h>
#include "LightJson.h"

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

struct JsonData {
  const char *json;
};

JsonType get_type(const JsonValue *value) {
  assert(value != nullptr);
  return value->type;
}

double get_number(const JsonValue *value) {
  assert(value != nullptr && value->type == kNumber);
  return value->number;
}

static void parse_whitespace(JsonData *data) {
  const char *p = data->json;
  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
    p++;
  data->json = p;
}

static int parse_literal(JsonData *data,
                         JsonValue *value,
                         const char *literal,
                         JsonType type) {
  EXPECT(data, literal[0]);
  size_t i;
  for (i = 0; literal[i + 1]; ++i)
    if (data->json[i] != literal[i + 1])
      return PARSE_INVALID_VALUE;
  data->json += i;
  value->type = type;
  return PARSE_OK;
}

static int parse_number(JsonData *data, JsonValue *value) {
  // Manually check if the number is valid, and move |curr| to the end.
  const char *curr = data->json;
  if (*curr == '-') ++curr;
  // if 0 leads, the number must have a decimal component.
  if (*curr == '0') ++curr;
  else {
    // else, leading digit cannot be 0.
    if (!ISDIGIT1TO9(*curr)) return PARSE_INVALID_VALUE;
    while (ISDIGIT(*curr)) ++curr;
  }
  if (*curr == '.') {
    ++curr;
    if (!ISDIGIT(*curr)) return PARSE_INVALID_VALUE;
    while (ISDIGIT(*curr)) ++curr;
  }
  if (*curr == 'e' || *curr == 'E') {
    ++curr;
    // +- sign after exponent is optional.
    if (*curr == '+' || *curr == '-') ++curr;
    if (!ISDIGIT(*curr)) return PARSE_INVALID_VALUE;
    while (ISDIGIT(*curr)) ++curr;
  }
  errno = 0;
  value->number = strtod(data->json, nullptr);
  if (errno == ERANGE
      && (value->number == HUGE_VAL || value->number == -HUGE_VAL))
    return PARSE_NUMBER_TOO_BIG;
  data->json = curr;
  value->type = kNumber;
  return PARSE_OK;
}

static int parse_value(JsonData *data, JsonValue *value) {
  switch (*data->json) {
    case 'n': return parse_literal(data, value, "null", kNull);
    case 'f': return parse_literal(data, value, "false", kFalse);
    case 't': return parse_literal(data, value, "true", kTrue);
    default: return parse_number(data, value);
    case '\0': return PARSE_EXPECT_VALUE;
  }
}

int parse(JsonValue *value, const char *json) {
  JsonData data;
  assert(value != nullptr);
  data.json = json;
  value->type = kNull;
  parse_whitespace(&data);
  int ret;
  if ((ret = parse_value(&data, value)) == PARSE_OK) {
    parse_whitespace(&data);
    if (*data.json != '\0') {
      value->type = kNull;
      return PARSE_ROOT_NOT_SINGULAR;
    }
  }
  return ret;
}