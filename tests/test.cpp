//
// Created by William Liu on 2019-08-08.
//

#include <gtest/gtest.h>
#include <string>
#include "../include/Json.h"

using namespace ::lightjson;

Json assertParseSuccess(const std::string &jsonStr) {
  std::string errMsg;
  auto json = Json::parse(jsonStr, errMsg);
  EXPECT_EQ(errMsg, "");
  return json;
}

#define TEST_ERROR(expect, strJson)       \
  do {                                    \
    std::string errMsg;                   \
    Json json =                           \
        Json:: parse(strJson, errMsg);    \
    auto pos = errMsg.find_first_of(":"); \
    auto actual = errMsg.substr(0, pos);  \
    EXPECT_EQ(expect, actual);            \
  } while (0)

#define TEST_NULL(strJson)           \
  do {                               \
    auto json =                      \
        assertParseSuccess(strJson); \
    EXPECT_TRUE(json.isNull());      \
  } while (0)

#define TEST_BOOL(expect, content)     \
  do {                                 \
    auto json =                        \
        assertParseSuccess(content);   \
    EXPECT_TRUE(json.isBool());        \
    EXPECT_EQ(expect, json.toBool());  \
    json = Json(!expect);   \
    EXPECT_EQ(!expect, json.toBool()); \
  } while (0)

#define TEST_NUMBER(expect, strJson)    \
  do {                                  \
    auto json =                         \
        assertParseSuccess(strJson);    \
    EXPECT_TRUE(json.isNumber());       \
    EXPECT_EQ(expect, json.toNumber()); \
  } while (0)

#define TEST_STRING(expect, strJson)    \
  do {                                  \
    auto json =                         \
        assertParseSuccess(strJson);    \
    EXPECT_TRUE(json.isString());       \
    EXPECT_EQ(expect, json.toString()); \
  } while (0)

#define TEST_ROUNDTRIP(expect)                        \
  do {                                                \
    Json json =                            \
        assertParseSuccess(expect);                   \
    auto actual = json.serialize();                   \
    if (json.isNumber())                              \
      EXPECT_EQ(std::strtod(actual.c_str(), nullptr), \
                std::strtod(expect, nullptr));        \
    else                                              \
      EXPECT_EQ(actual, expect);                      \
  } while (0)

TEST(ParseSuccess, Null) {
  TEST_NULL("null");
  TEST_NULL("   null\n\r\t");
}

TEST(ParseSuccess, Bool) {
  TEST_BOOL(true, "true");
  TEST_BOOL(false, "false");
}

TEST(ParseSuccess, Number) {
  TEST_NUMBER(0.0, "0");
  TEST_NUMBER(0.0, "-0");
  TEST_NUMBER(0.0, "-0.0");
  TEST_NUMBER(1.0, "1");
  TEST_NUMBER(-1.0, "-1");
  TEST_NUMBER(1.5, "1.5");
  TEST_NUMBER(-1.5, "-1.5");
  TEST_NUMBER(3.1416, "3.1416");
  TEST_NUMBER(1E10, "1E10");
  TEST_NUMBER(1e10, "1e10");
  TEST_NUMBER(1E+10, "1E+10");
  TEST_NUMBER(1E-10, "1E-10");
  TEST_NUMBER(-1E10, "-1E10");
  TEST_NUMBER(-1e10, "-1e10");
  TEST_NUMBER(-1E+10, "-1E+10");
  TEST_NUMBER(-1E-10, "-1E-10");
  TEST_NUMBER(1.234E+10, "1.234E+10");
  TEST_NUMBER(1.234E-10, "1.234E-10");
  TEST_NUMBER(5.0E-324, "5e-324");
  TEST_NUMBER(0, "1e-10000");
  TEST_NUMBER(1.0000000000000002, "1.0000000000000002");
  TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324");
  TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
  TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308");
  TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
  TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308");
  TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
  TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308");
  TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

TEST(ParseSuccess, String) {
  TEST_STRING("", "\"\"");
  TEST_STRING("Hello", "\"Hello\"");
  TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
  TEST_STRING("\" \\ / \b \f \n \r \t",
              "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
  TEST_STRING(std::string("Hello\0World", 11), "\"Hello\\u0000World\"");
  TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
  TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
  TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
  TEST_STRING("\xF0\x9D\x84\x9E",
              "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
  TEST_STRING("\xF0\x9D\x84\x9E",
              "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
}

TEST(ParseSuccess, Array) {
  Json json;
  json = assertParseSuccess("[ ]");
  EXPECT_TRUE(json.isArray());
  EXPECT_EQ(json.size(), 0);

  json = assertParseSuccess("  [  ]  ");
  EXPECT_TRUE(json.isArray());
  EXPECT_EQ(json.size(), 0);

  json = assertParseSuccess("[ null , false , true , 123 , \"abc\" ]");
  EXPECT_TRUE(json.isArray());
  EXPECT_EQ(json.size(), 5);
  EXPECT_EQ(json[0], Json(nullptr));
  EXPECT_EQ(json[1], Json(false));
  EXPECT_EQ(json[2], Json(true));
  EXPECT_EQ(json[3], Json(123.0));
  EXPECT_EQ(json[4], Json("abc"));

  json = assertParseSuccess("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]");
  EXPECT_TRUE(json.isArray());
  EXPECT_EQ(json.size(), 4);

  EXPECT_TRUE(json[0].isArray());
  EXPECT_EQ(json[0].size(), 0);

  EXPECT_TRUE(json[1].isArray());
  EXPECT_EQ(json[1].size(), 1);
  EXPECT_EQ(json[1][0].toNumber(), 0);

  EXPECT_TRUE(json[2].isArray());
  EXPECT_EQ(json[2].size(), 2);
  EXPECT_EQ(json[2][0].toNumber(), 0);
  EXPECT_EQ(json[2][1].toNumber(), 1);

  EXPECT_TRUE(json[3].isArray());
  EXPECT_EQ(json[3].size(), 3);
  EXPECT_EQ(json[3][0].toNumber(), 0);
  EXPECT_EQ(json[3][1].toNumber(), 1);
  EXPECT_EQ(json[3][2].toNumber(), 2);
}

TEST(ParseSuccess, Object) {
  Json json;
  json = assertParseSuccess("{}");
  EXPECT_TRUE(json.isObject());
  EXPECT_EQ(json.size(), 0);

  json = assertParseSuccess(" {  }   ");
  EXPECT_TRUE(json.isObject());
  EXPECT_EQ(json.size(), 0);

  json = assertParseSuccess(
      " { "
      "\"n\" : null , "
      "\"f\" : false , "
      "\"t\" : true , "
      "\"i\" : 123 , "
      "\"s\" : \"abc\", "
      "\"a\" : [ 1, 2, 3 ],"
      "\"o\" : { \"1\" : 1.3, \"2\" : \"2\", \"3\" : null }"
      " } ");
  EXPECT_TRUE(json.isObject());
  EXPECT_EQ(json.size(), 7);

  EXPECT_TRUE(json["n"].isNull());

  EXPECT_TRUE(json["f"].isBool());
  EXPECT_EQ(json["f"].toBool(), false);

  EXPECT_TRUE(json["t"].isBool());
  EXPECT_EQ(json["t"].toBool(), true);

  EXPECT_TRUE(json["i"].isNumber());
  EXPECT_EQ(json["i"].toNumber(), 123.0);

  EXPECT_TRUE(json["s"].isString());
  EXPECT_EQ(json["s"].toString(), "abc");

  EXPECT_TRUE(json["a"].isArray());
  EXPECT_EQ(json["a"].size(), 3);
  EXPECT_TRUE(json["a"][0].isNumber());
  EXPECT_EQ(json["a"][0].toNumber(), 1);
  EXPECT_TRUE(json["a"][1].isNumber());
  EXPECT_EQ(json["a"][1].toNumber(), 2);
  EXPECT_TRUE(json["a"][2].isNumber());
  EXPECT_EQ(json["a"][2].toNumber(), 3);

  EXPECT_TRUE(json["o"].isObject());
  EXPECT_EQ(json["o"].size(), 3);
  EXPECT_TRUE(json["o"]["1"].isNumber());
  EXPECT_EQ(json["o"]["1"].toNumber(), 1.3);
  EXPECT_TRUE(json["o"]["2"].isString());
  EXPECT_EQ(json["o"]["2"].toString(), "2");
  EXPECT_TRUE(json["o"]["3"].isNull());
}

TEST(RoundTrip, Literal) {
  TEST_ROUNDTRIP("null");
  TEST_ROUNDTRIP("true");
  TEST_ROUNDTRIP("false");
}

TEST(RoundTrip, Number) {
  TEST_ROUNDTRIP("0");
  TEST_ROUNDTRIP("-0");
  TEST_ROUNDTRIP("1");
  TEST_ROUNDTRIP("-0");
  TEST_ROUNDTRIP("1.5");
  TEST_ROUNDTRIP("-1.5");
  TEST_ROUNDTRIP("3.25");
  TEST_ROUNDTRIP("1e+20");
  TEST_ROUNDTRIP("1.234e+20");
  TEST_ROUNDTRIP("1.234e-20");
  TEST_ROUNDTRIP("1.0000000000000002");
  TEST_ROUNDTRIP("4.9406564584124654e-324");
  TEST_ROUNDTRIP("-4.9406564584124654e-324");
  TEST_ROUNDTRIP("2.2250738585072009e-308");
  TEST_ROUNDTRIP("-2.2250738585072009e-308");
  TEST_ROUNDTRIP("2.2250738585072014e-308");
  TEST_ROUNDTRIP("-2.2250738585072014e-308");
  TEST_ROUNDTRIP("1.7976931348623157e+308");
  TEST_ROUNDTRIP("-1.7976931348623157e+308");
}

TEST(RoundTrip, String) {
  TEST_ROUNDTRIP("\"\"");
  TEST_ROUNDTRIP("\"Hello\"");
  TEST_ROUNDTRIP("\"Hello\\nWorld\"");
  TEST_ROUNDTRIP("\"\\\" \\\\ / \\b \\f \\n \\r \\t\"");
  TEST_ROUNDTRIP("\"Hello\\u0000World\"");
}

TEST(RoundTrip, Array) {
  TEST_ROUNDTRIP("[]");
  TEST_ROUNDTRIP("[null, false, true, 123, \"abc\", [1, 2, 3]]");
}

TEST(RoundTrip, Object) {
  TEST_ROUNDTRIP("{}");
  // TODO: deterministic ordering of map.
  std::string jsonString = "{"
                           "\"n\": null, "
                           "\"f\": false, "
                           "\"t\": true, "
                           "\"i\": 123, "
                           "\"a\": [1, 2, 3], "
                           "\"s\": \"abc\", "
                           "\"o\": {\"1\": 1, \"2\": 2, \"3\": 3}"
                           "}";
  auto json = assertParseSuccess(jsonString);
  auto json2 = assertParseSuccess(json.serialize());
  EXPECT_EQ(json, json2);
}

TEST(ParseError, InvalidValue) {
  TEST_ERROR("Invalid value", "nul");
  TEST_ERROR("Invalid value", "?");
  TEST_ERROR("Invalid value", "+0");
  TEST_ERROR("Invalid value", "+1");
  TEST_ERROR("Invalid value", ".123");
  TEST_ERROR("Invalid value", "1.");
  TEST_ERROR("Invalid value", "inf");
  TEST_ERROR("Invalid value", "INF");
  TEST_ERROR("Invalid value", "NAN");
  TEST_ERROR("Invalid value", "nan");
  TEST_ERROR("Invalid value", "[1,]");
  TEST_ERROR("Invalid value", "[\"a\", nul]");
}

TEST(ParseError, RootNotSingular) {
  TEST_ERROR("Root not singular", "null x");
  TEST_ERROR("Root not singular", "0123");
  TEST_ERROR("Root not singular", "0x0");
  TEST_ERROR("Root not singular", "0x123");
}

TEST(ParseError, NumberTooBig) {
  TEST_ERROR("Number out of bound", "1e309");
  TEST_ERROR("Number out of bound", "-1e309");
}

TEST(ParseError, MissingQuote) {
  TEST_ERROR("Missing quotation mark", "\"");
  TEST_ERROR("Missing quotation mark", "\"abc");
}

TEST(ParseError, InvalidEscape) {
  TEST_ERROR("Invalid escape character", "\"\\v\"");
  TEST_ERROR("Invalid escape character", "\"\\'\"");
  TEST_ERROR("Invalid escape character", "\"\\0\"");
  TEST_ERROR("Invalid escape character", "\"\\x12\"");
}

TEST(ParseError, MissQuotationMark) {
  TEST_ERROR("Missing quotation mark", "\"");
  TEST_ERROR("Missing quotation mark", "\"abc");
}

TEST(ParseError, InvalidStringEscape) {
  TEST_ERROR("Invalid escape character", "\"\\v\"");
  TEST_ERROR("Invalid escape character", "\"\\'\"");
  TEST_ERROR("Invalid escape character", "\"\\0\"");
  TEST_ERROR("Invalid escape character", "\"\\x12\"");
}

TEST(ParseError, InvalidCharacter) {
  TEST_ERROR("Invalid character", "\"\x01\"");
  TEST_ERROR("Invalid character", "\"\x1F\"");
}

TEST(ParseError, InvalidHexValue) {
  TEST_ERROR("Invalid hex value", "\"\\u\"");
  TEST_ERROR("Invalid hex value", "\"\\u0\"");
  TEST_ERROR("Invalid hex value", "\"\\u01\"");
  TEST_ERROR("Invalid hex value", "\"\\u012\"");
  TEST_ERROR("Invalid hex value", "\"\\u/000\"");
  TEST_ERROR("Invalid hex value", "\"\\uG000\"");
  TEST_ERROR("Invalid hex value", "\"\\u0/00\"");
  TEST_ERROR("Invalid hex value", "\"\\u0G00\"");
  TEST_ERROR("Invalid hex value", "\"\\u000/\"");
  TEST_ERROR("Invalid hex value", "\"\\u00G/\"");
  TEST_ERROR("Invalid hex value", "\"\\u 123/\"");
}

TEST(ParseError, InvalidUnicodeSurrogate) {
  TEST_ERROR("Invalid unicode surrogate", "\"\\uD800\"");
  TEST_ERROR("Invalid unicode surrogate", "\"\\uDBFF\"");
  TEST_ERROR("Invalid unicode surrogate", "\"\\uD800\\\\\\");
  TEST_ERROR("Invalid unicode surrogate", "\"\\uD800\\uDBFF\"");
  TEST_ERROR("Invalid unicode surrogate", "\"\\uD800\\uE000\"");
}

TEST(ParseError, MissingClosingSquareBracketOrComma) {
  TEST_ERROR("Missing closing bracket or comma", "[1");
  TEST_ERROR("Missing closing bracket or comma", "[1}");
  TEST_ERROR("Missing closing bracket or comma", "[1 2");
  TEST_ERROR("Missing closing bracket or comma", "[[]");
}

TEST(ParseError, MissingKey) {
  TEST_ERROR("Missing key", "{:1,");
  TEST_ERROR("Missing key", "{1:1,");
  TEST_ERROR("Missing key", "{true:1,");
  TEST_ERROR("Missing key", "{false:1,");
  TEST_ERROR("Missing key", "{null:1,");
  TEST_ERROR("Missing key", "{[]:1,");
  TEST_ERROR("Missing key", "{{}:1,");
  TEST_ERROR("Missing key", "{\"a\":1,");
}

TEST(ParseError, MissingColon) {
  TEST_ERROR("Missing colon", "{\"a\"}");
  TEST_ERROR("Missing colon", "{\"a\",\"b\"}");
}

TEST(ParseError, MissingClosingCurlyBracketOrComma) {
  TEST_ERROR("Missing closing bracket or comma", "{\"a\":1");
  TEST_ERROR("Missing closing bracket or comma", "{\"a\":1]");
  TEST_ERROR("Missing closing bracket or comma", "{\"a\":1 \"b\"");
  TEST_ERROR("Missing closing bracket or comma", "{\"a\":{}");
}

TEST(Json, Constructor) {
  Json json;
  json = Json(nullptr);
  EXPECT_TRUE(json.isNull());

  json = Json(true);
  EXPECT_TRUE(json.isBool());
  EXPECT_EQ(json.toBool(), true);

  json = Json(false);
  EXPECT_TRUE(json.isBool());
  EXPECT_EQ(json.toBool(), false);

  json = Json(0);
  EXPECT_TRUE(json.isNumber());
  EXPECT_EQ(json.toNumber(), 0);

  json = Json(100.1);
  EXPECT_TRUE(json.isNumber());
  EXPECT_EQ(json.toNumber(), 100.1);

  json = Json("hello");
  EXPECT_TRUE(json.isString());
  EXPECT_EQ(json.toString(), "hello");

  Json::array arr
      {Json(nullptr), Json(true), Json(1.2)};
  json = Json(arr);
  EXPECT_TRUE(json.isArray());
  EXPECT_TRUE(json[0].isNull());
  EXPECT_TRUE(json[1].isBool());
  EXPECT_TRUE(json[2].isNumber());

  Json::object obj;
  obj.insert({"hello", Json(nullptr)});
  obj.insert({"world", Json("!!")});
  json = Json(obj);
  EXPECT_TRUE(json.isObject());
  EXPECT_TRUE(json["world"].isString());
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}