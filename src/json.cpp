#include "json.h"
#include <stdexcept>
#include <sstream>



JsonValue::JsonValue(): value{nullptr} {};
JsonValue::JsonValue(nullptr_t _value): value{nullptr} {};
JsonValue::JsonValue(bool _value): value{_value} {};
JsonValue::JsonValue(int _value): value{(double) _value} {};
JsonValue::JsonValue(double _value): value{_value} {};
JsonValue::JsonValue(const std::string& _value): value{_value} {};
JsonValue::JsonValue(const char* _value): value{std::string(_value)} {};
JsonValue::JsonValue(const Object& _value): value{_value} {}; 
JsonValue::JsonValue(const Array& _value): value{_value} {}; 
JsonValue::JsonValue(Object&& _value): value{std::move(_value)} {};
JsonValue::JsonValue(Array&& _value): value{std::move(_value)} {};

JsonValue::JsonValue(Type _type) {
    switch (_type) {
        case Type::Null: value = nullptr; break;
        case Type::Boolean: value = false; break;
        case Type::Number: value = 0.0; break;
        case Type::String: value = std::string(); break;
        case Type::Object: value = Object(); break;
        case Type::Array: value = Array(); break;
    }
}

JsonValue::Type JsonValue::type() const {
    return static_cast<JsonValue::Type>(value.index());
}

bool JsonValue::as_boolean() const {
    verify_type(Type::Boolean);
    return std::get<bool>(value);
}

double JsonValue::as_double() const {
    verify_type(Type::Number);
    return std::get<double>(value);
}

const std::string& JsonValue::as_string() const {
    verify_type(Type::String);
    return std::get<std::string>(value);
}

const JsonValue::Object& JsonValue::as_object() const {
    verify_type(Type::Object);
    return std::get<Object>(value);
}

const JsonValue::Array& JsonValue::as_array() const {
    verify_type(Type::Array);
    return std::get<Array>(value);
}

JsonValue& JsonValue::at(const std::string& index) {
    verify_type(Type::Object);
    return std::get<Object>(value)[index];
}

const JsonValue& JsonValue::at(const std::string& index) const {
    verify_type(Type::Object);
    return std::get<Object>(value).at(index);
}

JsonValue& JsonValue::at(int index) {
    verify_type(Type::Array);
    verify_index(index);
    return std::get<Array>(value)[index];
}

const JsonValue& JsonValue::at(int index) const {
    verify_type(Type::Array);
    verify_index(index);
    return std::get<Array>(value).at(index);
}

bool JsonValue::exists(const std::string& index) const {
    return type() == Type::Object && std::get<Object>(value).contains(index);
}

bool JsonValue::exists(const int index) const {
    return index >= 0 && type() == Type::Array && index < (int) std::get<Array>(value).size();
}

void JsonValue::set_index(const std::string& index, const JsonValue& _value) {
    verify_type(Type::Object);
    std::get<Object>(value)[index] = _value;
}
void JsonValue::set_index(const std::string& index, JsonValue&& _value) {
    verify_type(Type::Object);
    std::get<Object>(value)[index] = std::move(_value);
}
void JsonValue::set_index(const std::string& index, const JsonValue::Array& _value) {
    set_index(index, JsonValue(_value));
}
void JsonValue::set_index(const std::string& index, JsonValue::Array&& _value) {
    set_index(index, JsonValue(_value));
}
void JsonValue::set_index(const std::string& index, const JsonValue::Object& _value){
    set_index(index, JsonValue(_value));
}
void JsonValue::set_index(const std::string& index, JsonValue::Object&& _value) {
    set_index(index, JsonValue(_value));
}

void JsonValue::set_index(const int index, const JsonValue& _value) {
    verify_type(Type::Array);
    verify_index(index);
    std::get<Array>(value)[index] = _value;
}
void JsonValue::set_index(const int index, JsonValue&& _value) {
    verify_type(Type::Array);
    verify_index(index);
    std::get<Array>(value)[index] = std::move(_value);
}
void JsonValue::set_index(const int index, const JsonValue::Array& _value) {
    set_index(index, JsonValue(_value));
}
void JsonValue::set_index(const int index, JsonValue::Array&& _value) {
    set_index(index, JsonValue(_value));
}
void JsonValue::set_index(const int index, const JsonValue::Object& _value) {
    set_index(index, JsonValue(_value));
}
void JsonValue::set_index(const int index, JsonValue::Object&& _value) {
    set_index(index, JsonValue(_value));
}

void JsonValue::push_back(const JsonValue& _value) {
    verify_type(Type::Array);
    std::get<Array>(value).push_back(_value);
}
void JsonValue::push_back(JsonValue&& _value) {
    verify_type(Type::Array);
    std::get<Array>(value).push_back(std::move(_value));
}
void JsonValue::push_back(const Array& _value) {
    push_back(JsonValue(_value));
}
void JsonValue::push_back(Array&& _value) {
    push_back(JsonValue(_value));
}
void JsonValue::push_back(const Object& _value) {
    push_back(JsonValue(_value));
}
void JsonValue::push_back(Object&& _value) {
    push_back(JsonValue(_value));
}

void JsonValue::set_value(bool _value) {
    value = _value;
}

void JsonValue::set_value(double _value) {
    value = _value;
}

void JsonValue::set_value(const std::string& _value) {
    value = _value;
}

void JsonValue::set_value(std::string&& _value) {
    value = std::move(_value);
}

void JsonValue::set_value(const Object& _value) {
    value = _value;
}

void JsonValue::set_value(Object&& _value) {
    value = std::move(_value);
}

void JsonValue::set_value(const Array& _value) {
    value = _value;
}

void JsonValue::set_value(Array&& _value) {
    value = std::move(_value);
}

void JsonValue::set_type(Type type) {
    switch (type) {
        case Type::Null: value = nullptr; return;
        case Type::Boolean: value = bool(); return;
        case Type::Number: value = double(); return;
        case Type::String: value = std::string(); return;
        case Type::Object: value = Object(); return;
        case Type::Array: value = Array(); return;
        default:
            throw std::runtime_error("Invalid JSON type");
    }
}

std::string JsonValue::to_string() const {
    switch (type()) {
        case Type::Null: return "null";
        case Type::Boolean: return as_boolean() ? "true" : "false";
        case Type::Number: return std::to_string(as_double());
        case Type::String: return "\"" + as_string() + "\"";
        case Type::Object: return object_to_string();
        case Type::Array: return array_to_string();
        default: throw std::runtime_error("Invalid JSON type");
    }
}

void JsonValue::verify_type(Type expected) const {
    if (type() != expected) {
        std::stringstream ss;
        ss << "Invalid method type, requested " << JsonValue::TypeNames[static_cast<int>(expected)] << ", but JsonValue is of type " << JsonValue::TypeNames[static_cast<int>(type())];
        throw std::runtime_error(ss.str());
    }
}

void JsonValue::verify_index(int index) const {
    if (index < 0 || index >= (int) std::get<Array>(value).size())
        throw std::runtime_error("Index out of bounds");
}

std::string JsonValue::object_to_string() const {
    std::stringstream ss;
    bool start = true;
    ss << JsonConstants::OBJECT_START;
    for (const auto& [key, json_val]: std::get<Object>(value)) {
        if (!start) ss << ", ";
        start = false;

        ss << JsonConstants::STRING_QUOTE << key << JsonConstants::STRING_QUOTE;
        ss << JsonConstants::KEY_VALUE_SEPARATOR << " ";

        ss << json_val.to_string();
    }
    ss << JsonConstants::OBJECT_END;
    return ss.str();
}

std::string JsonValue::array_to_string() const {
    std::stringstream ss;
    bool start = true;
    ss << JsonConstants::ARRAY_START;
    for (const auto& json_val: std::get<Array>(value)) {
        if (!start) ss << ", ";
        start = false;
        ss << json_val.to_string();
    }
    ss << JsonConstants::ARRAY_END;
    return ss.str();
}   


