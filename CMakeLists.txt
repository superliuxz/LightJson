cmake_minimum_required(VERSION 3.14)
project(LightJson)

set(CMAKE_CXX_STANDARD 17)

add_library(LightJson SHARED LightJson.cpp LightJson.h test.cpp Parser.h Json.h Parser.cpp JsonException.h Json.cpp JsonValue.cpp JsonValue.h)
add_executable(test test.cpp)
target_link_libraries(test LightJson)
