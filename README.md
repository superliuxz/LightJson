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
  std::cout << json["o"] << std::endl; // {"3": null, "1": 1.3, "2": "2"}
  std::cout << json["o"]["1"] << std::endl; // 1.3
  auto str = json["a"].serialize();
  std::cout << str << std::endl; // [1, 2, 3]

  Json json2 = Json::object{
      {"key1", 2},
      {"key2", Json::array{{-0.5, true}} }
  };
  std::cout << json2 << std::endl; // {"key2": [-0.5, true], "key1": 2}
}
```