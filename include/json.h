#pragma once

#include <variant>
#include <unordered_map>
#include <string>
#include <vector>

namespace JsonConstants {
    constexpr char ARRAY_START = '[';
    constexpr char ARRAY_END = ']';
    constexpr char OBJECT_START = '{';
    constexpr char OBJECT_END = '}';
    constexpr char STRING_QUOTE = '"';
    constexpr char KEY_VALUE_SEPARATOR = ':';
    constexpr char ITEM_SEPARATOR = ',';
    constexpr char ESCAPE = '\\';
    constexpr char DECIMAL_POINT = '.';
    constexpr char MINUS = '-';
    constexpr char EXPONENT_UNCASED = 'e';
};

// We are deliberately not using shared_ptr. 
struct JsonValue {

    using Object = std::unordered_map<std::string, JsonValue>;
    using Array = std::vector<JsonValue>;

    enum class Type {
        Null,
        Boolean,
        Number,
        String,
        Object,
        Array
    };

    JsonValue();

    JsonValue(bool _value);
    JsonValue(double _value);
    JsonValue(const std::string& _value);
    JsonValue(const Object& _value);
    JsonValue(const Array& _array);

    JsonValue(Type _type);

    Type type() const;

    bool as_boolean() const;
    double as_double() const;
    const std::string& as_string() const;
    const Object& as_object() const;
    const Array& as_array() const;

    JsonValue& at(const std::string& index);
    const JsonValue& at(const std::string& index) const;
    JsonValue& at(int index);
    const JsonValue& at(int index) const;

    void add_to_object(const std::string& index, JsonValue&& _value);
    void add_to_array(JsonValue&& _value);

    void set_value(bool _value);
    void set_value(double _value);
    void set_value(const std::string& _value);
    void set_value(const Object& _value);
    void set_value(const Array& _value);

    std::string to_string() const;

private:
    void verify_type(Type expected) const;
    void verify_index(int index) const;

    static std::string object_to_string(const Object& object);
    static std::string array_to_string(const Array& array);

    std::variant<std::nullptr_t, bool, double, std::string, Object, Array> value;
};