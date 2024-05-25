#include <gtest/gtest.h>
#include <sstream>
#include "json.h"
#include "parser.h"
#include <iostream>

// Helper function to parse JSON from a string
std::optional<JsonValue> parse_json_string(const std::string& json) {
    std::istringstream input(json);
    return parse(input);
}

// Test case for parsing a string within an array
TEST(JsonParserTest, ParseString) {
    std::string json = "[\"hello\"]";
    std::optional<JsonValue> result = parse_json_string(json);
    
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->type(), JsonValue::Type::Array);
    ASSERT_EQ(result->as_array().size(), 1);
    EXPECT_EQ(result->at(0).as_string(), "hello");
}

// Test case for parsing a number within an array
TEST(JsonParserTest, ParseNumber) {
    std::string json = "[123.456]";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->type(), JsonValue::Type::Array);
    ASSERT_EQ(result->as_array().size(), 1);
    EXPECT_DOUBLE_EQ(result->at(0).as_double(), 123.456);
}

// Test case for parsing a boolean true within an array
TEST(JsonParserTest, ParseTrue) {
    std::string json = "[true]";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->type(), JsonValue::Type::Array);
    ASSERT_EQ(result->as_array().size(), 1);
    EXPECT_EQ(result->at(0).as_boolean(), true);
}

// Test case for parsing a boolean false within an array
TEST(JsonParserTest, ParseFalse) {
    std::string json = "[false]";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->type(), JsonValue::Type::Array);
    ASSERT_EQ(result->as_array().size(), 1);
    EXPECT_EQ(result->at(0).as_boolean(), false);
}

// Test case for parsing a null value within an array
TEST(JsonParserTest, ParseNull) {
    std::string json = "[null]";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->type(), JsonValue::Type::Array);
    ASSERT_EQ(result->as_array().size(), 1);
    EXPECT_EQ(result->at(0).type(), JsonValue::Type::Null);
}

// Test case for parsing an empty object
TEST(JsonParserTest, ParseEmptyObject) {
    std::string json = "{}";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), JsonValue::Type::Object);
}

// Test case for parsing a non-empty object
TEST(JsonParserTest, ParseObject) {
    std::string json = "{\"key\": \"value\", \"number\": 42}";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), JsonValue::Type::Object);
    EXPECT_EQ(result->at("key").as_string(), "value");
    EXPECT_DOUBLE_EQ(result->at("number").as_double(), 42);
}

// Test case for parsing an empty array
TEST(JsonParserTest, ParseEmptyArray) {
    std::string json = "[]";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), JsonValue::Type::Array);
}

// Test case for parsing a non-empty array
TEST(JsonParserTest, ParseArray) {
    std::string json = "[1, \"two\", true, null]";
    std::optional<JsonValue> result = parse_json_string(json);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->type(), JsonValue::Type::Array);
    EXPECT_DOUBLE_EQ(result->at(0).as_double(), 1);
    EXPECT_EQ(result->at(1).as_string(), "two");
    EXPECT_EQ(result->at(2).as_boolean(), true);
    EXPECT_EQ(result->at(3).type(), JsonValue::Type::Null);
}

// Test case for handling invalid JSON
TEST(JsonParserTest, ParseInvalidJson) {
    std::string json = "{invalid json}";
    std::optional<JsonValue> result = parse_json_string(json);
    EXPECT_FALSE(result.has_value());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
