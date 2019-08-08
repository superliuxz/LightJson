#include <stdlib.h>
#include <iostream>
#include "LightJson.h"

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)

struct JsonData {
  const char *json;
};

JsonType get_type(const JsonValue *value) {
  assert(value != nullptr);
  return value->type;
}

static void parse_whitespace(JsonData *data) {
  const char *p = data->json;
  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
    p++;
  data->json = p;
}

static int parse_null(JsonData *data, JsonValue *value) {
  EXPECT(data, 'n');
  if (data->json[0] != 'u' || data->json[1] != 'l' || data->json[2] != 'l')
    return PARSE_INVALID_VALUE;
  data->json += 3; // EXPECT macro has already moved json by 1.
  value->type = kNull;
  return PARSE_OK;
}

static int parse_true(JsonData *data, JsonValue *value) {
  EXPECT(data, 't');
  if (data->json[0] != 'r' || data->json[1] != 'u' || data->json[2] != 'e')
    return PARSE_INVALID_VALUE;
  data->json += 3;
  value->type = kTrue;
  return PARSE_OK;
}

static int parse_false(JsonData *data, JsonValue *value) {
  EXPECT(data, 'f');
  if (data->json[0] != 'a' || data->json[1] != 'l' || data->json[2] != 's'
      || data->json[3] != 'e')
    return PARSE_INVALID_VALUE;
  data->json += 4;
  value->type = kFalse;
  return PARSE_OK;
}

static int parse_value(JsonData *data, JsonValue *value) {
  switch (*data->json) {
    case 'n': return parse_null(data, value);
    case 'f': return parse_false(data, value);
    case 't': return parse_true(data, value);
    case '\0': return PARSE_EXPECT_VALUE;
    default: return PARSE_INVALID_VALUE;
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
    if (*data.json != '\0')
      return PARSE_ROOT_NOT_SINGULAR;
  }
  return ret;
}