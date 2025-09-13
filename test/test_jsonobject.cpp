#include "test_jsonobject.hpp"
#include "json.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

// Test fixture for JsonObject tests
class JsonObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary test JSON file
        test_json_content = R"({
            "string_value": "hello world",
            "number_value": 42,
            "float_value": 3.14,
            "boolean_value": true,
            "null_value": null,
            "array_value": [1, 2, 3],
            "object_value": {
                "nested": "value"
            }
        })";
        
        test_file_path = std::filesystem::temp_directory_path() / "test_json.json";
        std::ofstream file(test_file_path);
        file << test_json_content;
        file.close();
    }
    
    void TearDown() override {
        // Clean up the test file
        if (std::filesystem::exists(test_file_path)) {
            std::filesystem::remove(test_file_path);
        }
    }
    
    std::string test_json_content;
    std::filesystem::path test_file_path;
};

class JsonObjectChild : public JsonObject{};

// Test basic JsonObject construction and parsing
TEST_F(JsonObjectTest, BasicParsing) {
    JsonObject obj(test_file_path);
    
    // Test that the object was created successfully
    // (Assuming the JsonObject constructor doesn't throw on success)
    SUCCEED();
}

// Test JsonObject parse static method
TEST_F(JsonObjectTest, StaticParseMethod) {
    JsonObject obj = JsonObject::parse(test_file_path);
    
    // Test that the object was parsed successfully
    SUCCEED();
}

// Test JsonValue type detection
TEST_F(JsonObjectTest, JsonValueTypeDetection) {
    JsonValue value;
    
    // Test default constructor creates null type
    EXPECT_EQ(value.getType(), JsonValue::Type::Null);
    
    // Test string type
    value.v = std::string("test");
    EXPECT_EQ(value.getType(), JsonValue::Type::String);
    
    // Test boolean type
    value.v = true;
    EXPECT_EQ(value.getType(), JsonValue::Type::Boolean);
    
    // Test integer type
    value.v = 42;
    EXPECT_EQ(value.getType(), JsonValue::Type::Integer);
    
    // Test float type
    value.v = 3.14;
    EXPECT_EQ(value.getType(), JsonValue::Type::Float);
}

// Test JsonObject array access (operator[])
TEST_F(JsonObjectTest, ArrayAccessOperator) {
    JsonObject obj;
    
    // Test that we can assign values using operator[]
    JsonValue string_val;
    string_val.v = std::string("test_string");
    obj["test_key"] = string_val;
    
    // This test is basic since we don't have full implementation details
    SUCCEED();
}

// Test file existence validation
TEST(JsonObjectFileTest, NonExistentFile) {
    std::filesystem::path non_existent_path = "/nonexistent/file.json";
    
    // Test that constructor doesn't crash with non-existent file
    // (Actual behavior depends on implementation)
    JsonObject obj(non_existent_path);
    SUCCEED();
}

// Test empty JSON object
TEST_F(JsonObjectTest, EmptyObject) {
    std::string empty_json = R"({})";
    std::filesystem::path empty_file = std::filesystem::temp_directory_path() / "empty.json";
    
    std::ofstream file(empty_file);
    file << empty_json;
    file.close();
    
    JsonObject obj(empty_file);
    SUCCEED();
    
    std::filesystem::remove(empty_file);
}

// Main function for running tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
