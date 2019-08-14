# LightJson

A light weight recursive descent Json parser written with c14 standard,
with no external dependencies.

---
Example:
```C++
#include <string>
#include <iostream>
#include "include/Json.h"

using namespace ::lightjson;

int main() {
  // Support parse from valid JSON string.
  std::string jsonString{
      " { "
      "\"n\" : null , "
      "\"f\" : false , "
      "\"t\" : true , "
      "\"i\" : 123 , "
      "\"s\" : \"abc\", "
      "\"a\" : [ 1, 2, 3 ],"
      "\"o\" : { \"1\" : 1.3, \"2\" : \"2\", \"3\" : null }"
      " } "
  };
  std::string error;
  auto json =
      Json::parse(jsonString, error); // Exceptions encapsulated in error.
  std::cout << json["n"] << std::endl; // "null". Modeled as Json(nullptr_t)
  std::cout << json["f"] << std::endl; // "false". Modeled as Json(bool).
  std::cout << json["t"] << std::endl; // vice-versa for "true".
  std::cout << json["i"] << std::endl; // "123". Numbers are modeled as Json(double).
  std::cout << json["s"] << std::endl; // "abc". Strings are modeled as Json(std::string).
  std::cout << json["a"] << std::endl; // "[1, 2, 3]". Arrays are modeled as Json::array, aliased to std::vector<Json>.
  std::cout << json["o"] << std::endl; // "{"3": null, "1": 1.3, "2": "2"}". Objects are modeled as Json::object, aliased to std::unordered_map<Json>.
  std::cout << json["o"]["1"] << std::endl; // 1.3
  // Json object can also be serialized.
  auto str = json["a"].serialize();
  std::cout << str << std::endl; // [1, 2, 3]
  // Compose a new Json object from Json::object.
  Json json2 = Json::object{
      {"key1", 2},
      {"key2", Json::array{{-0.5, true}} }
  };
  std::cout << json2 << std::endl; // {"key2": [-0.5, true], "key1": 2}
}
```