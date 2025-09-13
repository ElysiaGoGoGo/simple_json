# Use system-installed Google Test
find_package(GTest REQUIRED)

# Test executable
file(GLOB_RECURSE TEST_SOURCES ./*.cpp)
add_executable(
  json_tests
  ${TEST_SOURCES}
  ../src/json.cpp
  ../src/utf8handler.cpp
)

target_link_libraries(
  json_tests
  PRIVATE GTest::gtest_main

)
target_include_directories(
  json_tests
  PRIVATE ../inc
)

# Add test to CTest
include(GoogleTest)
gtest_discover_tests(json_tests)
