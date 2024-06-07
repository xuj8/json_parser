#include <gtest/gtest.h>
#include <sstream>
#include "json.h"
#include "parser.h"
#include <iostream>
#include "tools/cpp/runfiles/runfiles.h"
#include <nlohmann/json.hpp>

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

std::ifstream open_json_test_file(const std::string& filepath_to_root) {
    std::string workspace_name = "custom_json_parser"; 
    auto runfiles = bazel::tools::cpp::runfiles::Runfiles::CreateForTest();
    std::string runfiles_path = runfiles->Rlocation(workspace_name + "/" + filepath_to_root);
    auto file = std::ifstream(runfiles_path);
    if (!file.is_open()) 
        throw std::runtime_error("Error, file not open");
    return file;
}


// run tests on the official json test files

TEST(JsonParserTest, ParseOfficialTestCases) {
    std::string invalid_prefix = "data/tests/official/fail";
    std::string valid_prefix = "data/tests/official/pass";
    
    std::vector<std::string> invalids, valids;
    
    for(int i = 1; i <= 33; i++) {
        std::string filepath = invalid_prefix + std::to_string(i) + ".json";
        invalids.push_back(filepath);
    }
    for(int i = 1; i <= 3; i++) {
        std::string filepath = valid_prefix + std::to_string(i) + ".json";
        valids.push_back(filepath);
    }
    
    for (const auto& filepath: valids) {
        auto file = open_json_test_file(filepath);
        auto json_opt = parse(file);
        EXPECT_TRUE(json_opt.has_value()) << "Expected " << filepath << " to parse";
        
        // now, parse the same file w/ nlohmann and also using the to_string of the json_opt.
        nlohmann::json reference;
        file = open_json_test_file(filepath);
        file >> reference;
        
        nlohmann::json from_parsed = nlohmann::json::parse((json_opt->to_string()));
        
        EXPECT_TRUE(reference == from_parsed) << "Expected " << filepath << "'s parsed representation to equal reference";
    }
    
    for(const auto& filepath: invalids) {
        auto file = open_json_test_file(filepath);
        auto json_opt = parse(file);
        EXPECT_TRUE(!json_opt.has_value()) << "Did not expect " << filepath << " to parse";
    } 
}

// run tests on Crickett's test cases
TEST(JsonParserTest, ParseCrickettTestCases) {
    std::vector<std::string> valids = {"data/tests/step1/valid.json", 
                                       "data/tests/step2/valid.json",  
                                       "data/tests/step2/valid2.json",
                                       "data/tests/step3/valid.json",  
                                       "data/tests/step4/valid.json",  
                                       "data/tests/step4/valid2.json"};
            
    std::vector<std::string> invalids = {"data/tests/step1/invalid.json",
                                         "data/tests/step2/invalid.json",
                                         "data/tests/step2/invalid2.json",
                                         "data/tests/step3/invalid.json",
                                         "data/tests/step4/invalid.json"};
                                         
    // check valids
    
    for (const auto& filepath: valids) {
        auto file = open_json_test_file(filepath);
        auto json_opt = parse(file);
        EXPECT_TRUE(json_opt.has_value());
        
        // now, parse the same file w/ nlohmann and also using the to_string of the json_opt.
        nlohmann::json reference;
        file = open_json_test_file(filepath);
        file >> reference;
        
        nlohmann::json from_parsed = nlohmann::json::parse((json_opt->to_string()));
        
        EXPECT_TRUE(reference == from_parsed);
    }
    
    for(const auto& filepath: invalids) {
        auto file = open_json_test_file(filepath);
        auto json_opt = parse(file);
        EXPECT_TRUE(!json_opt.has_value());
    }                               
}






int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
