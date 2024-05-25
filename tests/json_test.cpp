#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "json.h"

bool compare_json_strings(const std::string& json_str1, const std::string& json_str2) {
    nlohmann::json json1 = nlohmann::json::parse(json_str1);
    nlohmann::json json2 = nlohmann::json::parse(json_str2);
    return json1 == json2;
}

TEST(JsonValueTest, NullValue) {
    JsonValue json_value(JsonValue::Type::Null);
    EXPECT_EQ(json_value.type(), JsonValue::Type::Null);
    EXPECT_EQ(json_value.to_string(), "null");
}

TEST(JsonValueTest, BooleanValue) {
    JsonValue json_value(true);
    EXPECT_EQ(json_value.type(), JsonValue::Type::Boolean);
    EXPECT_TRUE(json_value.as_boolean());
    EXPECT_EQ(json_value.to_string(), "true");

    json_value.set_value(false);
    EXPECT_FALSE(json_value.as_boolean());
    EXPECT_EQ(json_value.to_string(), "false");
}

TEST(JsonValueTest, IntConversion) {
    JsonValue json_value(0);
    EXPECT_EQ(json_value.type(), JsonValue::Type::Number);
    EXPECT_EQ(json_value.as_double(), 0.0);
}

TEST(JsonValueTest, NumberValue) {
    JsonValue json_value(42.0);
    EXPECT_EQ(json_value.type(), JsonValue::Type::Number);
    EXPECT_EQ(json_value.as_double(), 42.0);
    EXPECT_EQ(json_value.to_string(), "42.000000");
}

TEST(JsonValueTest, StringValue) {
    JsonValue json_value("Hello, World!");
    EXPECT_EQ(json_value.type(), JsonValue::Type::String);
    EXPECT_EQ(json_value.as_string(), "Hello, World!");
    EXPECT_EQ(json_value.to_string(), "\"Hello, World!\"");
}

TEST(JsonValueTest, ObjectValue) {
    JsonValue json_value(JsonValue::Type::Object);

    json_value.set_index("name", "John Doe");
    json_value.set_index("age", 30.0);

    EXPECT_EQ(json_value.type(), JsonValue::Type::Object);
    EXPECT_EQ(json_value.at("name").as_string(), "John Doe");
    EXPECT_EQ(json_value.at("age").as_double(), 30.0);
    // this is more strict than necessary because it imposes ordering on unordered keys
    std::string expected_json = R"({"age": 30, "name": "John Doe"})";
    std::string unexpected_json = R"({"age": 30.1, "name": "John Doe"})";
    EXPECT_TRUE(compare_json_strings(json_value.to_string(), expected_json));
    EXPECT_FALSE(compare_json_strings(json_value.to_string(), unexpected_json));
    // EXPECT_EQ(json_value.to_string(), "{\"name\": \"John Doe\", \"age\": 30.000000}");
}

// Test case to verify JSON array representation
TEST(JsonValueTest, ArrayValue) {
    JsonValue json_value(JsonValue::Type::Array);

    json_value.push_back("C++");
    json_value.push_back("Python");

    EXPECT_EQ(json_value.type(), JsonValue::Type::Array);
    EXPECT_EQ(json_value.at(0).as_string(), "C++");
    EXPECT_EQ(json_value.at(1).as_string(), "Python");

    std::string expected_json = R"(["C++", "Python"])";
    std::string unexpected_json = R"(["C+++", "Python"])";
    EXPECT_TRUE(compare_json_strings(json_value.to_string(), expected_json));
    EXPECT_FALSE(compare_json_strings(json_value.to_string(), unexpected_json));
    // this is more strict than necessary because it imposes ordering on unordered keys
    // EXPECT_EQ(json_value.to_string(), "[\"C++\", \"Python\"]");
}

// Test case to verify nested JSON structure
TEST(JsonValueTest, NestedValue) {
    JsonValue json_value(JsonValue::Type::Object);
    json_value.set_index("name", "Jane Doe");
    json_value.set_index("languages", JsonValue::Array{"C++", "Python"});

    EXPECT_EQ(json_value.type(), JsonValue::Type::Object);
    EXPECT_EQ(json_value.at("name").as_string(), "Jane Doe");
    EXPECT_EQ(json_value.at("languages").type(), JsonValue::Type::Array);
    EXPECT_EQ(json_value.at("languages").at(0).as_string(), "C++");
    EXPECT_EQ(json_value.at("languages").at(1).as_string(), "Python");

    std::string expected_json = R"({"languages": ["C++", "Python"], "name": "Jane Doe"})";
    std::string unexpected_json = R"({"languages": ["Python", "C++"], "name": "Jane Doe"})";

    EXPECT_TRUE(compare_json_strings(expected_json, json_value.to_string()));
    EXPECT_FALSE(compare_json_strings(unexpected_json, json_value.to_string()));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}