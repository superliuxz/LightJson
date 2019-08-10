//
// Created by William Liu on 2019-08-08.
//

#include <gtest/gtest.h>
#include <string>
#include "Json.h"

using namespace std;

lightjson::Json parseOk(const string &jsonStr) {
  string errMsg;
  auto json = lightjson::Json::parse(jsonStr, errMsg);
  EXPECT_EQ(errMsg, "");
  return json;
}

#define TEST_ERROR(expect, strJson)           \
  do {                                        \
    string errMsg;                            \
    lightjson::Json json =                    \
        lightjson::Json::                     \
        parse(strJson, errMsg);               \
    auto pos = errMsg.find_first_of(":");     \
    auto actual = errMsg.substr(0, pos);      \
    EXPECT_EQ(expect, actual);                \
  } while (0)

#define TEST_NULL(strJson)        \
  do {                            \
    auto json = parseOk(strJson); \
    EXPECT_TRUE(json.isNull());   \
  } while (0)

#define TEST_BOOL(expect, content)     \
  do {                                 \
    auto json = parseOk(content);      \
    EXPECT_TRUE(json.isBool());        \
    EXPECT_EQ(expect, json.toBool());  \
    json = lightjson::Json(!expect);   \
    EXPECT_EQ(!expect, json.toBool()); \
  } while (0)

#define TEST_NUMBER(expect, strJson)    \
  do {                                  \
    auto json = parseOk(strJson);       \
    EXPECT_TRUE(json.isNumber());       \
    EXPECT_EQ(expect, json.toDouble()); \
  } while (0)

#define TEST_STRING(expect, strJson)   \
  do {                                 \
    auto json = parseOk(strJson);      \
    EXPECT_TRUE(json.isString());      \
    EXPECT_EQ(expect, json.toString());\
  } while (0)

TEST(Success, Null) {
  TEST_NULL("null");
  TEST_NULL("   null\n\r\t");
}

TEST(Success, Bool) {
  TEST_BOOL(true, "true");
  TEST_BOOL(false, "false");
}

TEST(Success, Number) {
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

TEST(Success, String) {
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

TEST(Error, InvalidValue) {
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

TEST(Error, RootNotSingular) {
  TEST_ERROR("Root not singular", "null x");
  TEST_ERROR("Root not singular", "0123");
  TEST_ERROR("Root not singular", "0x0");
  TEST_ERROR("Root not singular", "0x123");
}

TEST(Error, NumberTooBig) {
  TEST_ERROR("Number out of bound", "1e309");
  TEST_ERROR("Number out of bound", "-1e309");
}

TEST(Error, MissingQuote) {
  TEST_ERROR("Missing quotation mark", "\"");
  TEST_ERROR("Missing quotation mark", "\"abc");
}

TEST(Error, InvalidEscape) {
  TEST_ERROR("Invalid escape character", "\"\\v\"");
  TEST_ERROR("Invalid escape character", "\"\\'\"");
  TEST_ERROR("Invalid escape character", "\"\\0\"");
  TEST_ERROR("Invalid escape character", "\"\\x12\"");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}