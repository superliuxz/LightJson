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

#define testError(expect, strJson)            \
  do {                                        \
    string errMsg;                            \
    lightjson::Json json = lightjson::Json::parse(strJson, errMsg); \
    auto pos = errMsg.find_first_of(":");     \
    auto actual = errMsg.substr(0, pos);      \
    EXPECT_EQ(actual, expect);                \
  } while (0)

#define testNull(strJson)         \
  do {                            \
    auto json = parseOk(strJson); \
    EXPECT_TRUE(json.isNull());   \
  } while (0)

#define testBool(expect, content)      \
  do {                                 \
    auto json = parseOk(content);      \
    EXPECT_TRUE(json.isBool());        \
    EXPECT_EQ(json.toBool(), expect);  \
    json = lightjson::Json(!expect);   \
    EXPECT_EQ(json.toBool(), !expect); \
  } while (0)

#define testNumber(expect, strJson)     \
  do {                                  \
    auto json = parseOk(strJson);       \
    EXPECT_TRUE(json.isNumber());       \
    EXPECT_EQ(json.toDouble(), expect); \
  } while (0)

TEST(Str2Json, JsonNull) {
  testNull("null");
  testNull("   null\n\r\t");
}

TEST(Str2Json, JsonBool) {
  testBool(true, "true");
  testBool(false, "false");
}

TEST(Str2Json, JsonNumber) {
  testNumber(0.0, "0");
  testNumber(0.0, "-0");
  testNumber(0.0, "-0.0");
  testNumber(1.0, "1");
  testNumber(-1.0, "-1");
  testNumber(1.5, "1.5");
  testNumber(-1.5, "-1.5");
  testNumber(3.1416, "3.1416");
  testNumber(1E10, "1E10");
  testNumber(1e10, "1e10");
  testNumber(1E+10, "1E+10");
  testNumber(1E-10, "1E-10");
  testNumber(-1E10, "-1E10");
  testNumber(-1e10, "-1e10");
  testNumber(-1E+10, "-1E+10");
  testNumber(-1E-10, "-1E-10");
  testNumber(1.234E+10, "1.234E+10");
  testNumber(1.234E-10, "1.234E-10");
  testNumber(5.0E-324, "5e-324");
  testNumber(0, "1e-10000");
  testNumber(1.0000000000000002, "1.0000000000000002");
  testNumber(4.9406564584124654e-324, "4.9406564584124654e-324");
  testNumber(-4.9406564584124654e-324, "-4.9406564584124654e-324");
  testNumber(2.2250738585072009e-308, "2.2250738585072009e-308");
  testNumber(-2.2250738585072009e-308, "-2.2250738585072009e-308");
  testNumber(2.2250738585072014e-308, "2.2250738585072014e-308");
  testNumber(-2.2250738585072014e-308, "-2.2250738585072014e-308");
  testNumber(1.7976931348623157e+308, "1.7976931348623157e+308");
  testNumber(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

TEST(Error, InvalidValue) {
  testError("Invalid value", "nul");
  testError("Invalid value", "?");
  testError("Invalid value", "+0");
  testError("Invalid value", "+1");
  testError("Invalid value", ".123");
  testError("Invalid value", "1.");
  testError("Invalid value", "inf");
  testError("Invalid value", "INF");
  testError("Invalid value", "NAN");
  testError("Invalid value", "nan");
  testError("Invalid value", "[1,]");
  testError("Invalid value", "[\"a\", nul]");
}

TEST(Error, RootNotSingular) {
  testError("Root not singular", "null x");
  testError("Root not singular", "0123");
  testError("Root not singular", "0x0");
  testError("Root not singular", "0x123");
}

TEST(Error, NumberTooBig) {
  testError("Number out of bound", "1e309");
  testError("Number out of bound", "-1e309");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}