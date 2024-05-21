#include "json.h"

JsonValue::JsonValue(): value{nullptr} {};
JsonValue::JsonValue(bool _value): value{_value} {};
JsonValue::JsonValue(double _value): value{_value} {};
JsonValue::JsonValue(const std::string& _value): value{_value} {};
JsonValue::JsonValue(const Object& _value): value{_value} {};
JsonValue::JsonValue(const Array& _value): value{_value} {};

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
    verify_type(Type::Boolean);
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
    verify_type(Type::Object);

    
    return std::get<Array>(value)[index];
}

const JsonValue& JsonValue::at(const std::string& index) const {
    verify_type(Type::Object);
    return std::get<Object>(value).at(index);
}