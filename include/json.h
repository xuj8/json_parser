#pragma once

#include <variant>
#include <boost/container/map.hpp>
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
    constexpr char PLUS = '+';
    constexpr char EXPONENT_UNCASED = 'e';
    constexpr char REVERSE_SLASH = '\\';
    constexpr char SLASH = '/';
    constexpr char BACKSPACE = 'b';
    constexpr char FORMFEED = 'f';
    constexpr char LINEFEED = 'n';
    constexpr char RETURN = 'r';
    constexpr char TAB = 't';
    constexpr char HEX = 'u';
    constexpr char COMMA = ',';
};

struct JsonValue;

// We are deliberately not using shared_ptr. 
struct JsonValue {

    using Object = boost::container::map<std::string, JsonValue>;
    using Array = std::vector<JsonValue>;

    enum class Type {
        Null,
        Number,
        Boolean,
        String,
        Object,
        Array
    };

    static constexpr char* TypeNames[] = {
        "null type",
        "number type",
        "boolean type",
        "string type",
        "object type",
        "array type"
    };

    using var_t = std::variant<std::nullptr_t, double, bool, std::string, Object, Array>;

    JsonValue();

    JsonValue(nullptr_t _value);
    JsonValue(bool _value);
    JsonValue(int _value);
    JsonValue(double _value);
    JsonValue(const std::string& _value);
    JsonValue(const char * _value);

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

    bool exists(const std::string& index) const;
    bool exists(const int index) const;

    void set_index(const std::string& index, const JsonValue& _value);
    void set_index(const std::string& index, JsonValue&& _value);
    void set_index(const std::string& index, const Array& _value);
    void set_index(const std::string& index, Array&& _value);
    void set_index(const std::string& index, const Object& _value);
    void set_index(const std::string& index, Object&& _value);
    void set_index(const int index, const JsonValue& _value);
    void set_index(const int index, JsonValue&& _value);
    void set_index(const int index, const Array& _value);
    void set_index(const int index, Array&& _value);
    void set_index(const int index, const Object& _value);
    void set_index(const int index, Object&& _value);

    void push_back(const JsonValue& _value);
    void push_back(JsonValue&& _value);
    void push_back(const Array& _value);
    void push_back(Array&& _value);
    void push_back(const Object& _value);
    void push_back(Object&& _value);

    void set_value(bool _value);
    void set_value(double _value);
    void set_value(const std::string& _value);
    void set_value(std::string&& _value);
    void set_value(const Object& _value);
    void set_value(Object&& _value);
    void set_value(const Array& _value);
    void set_value(Array&& _value);

    void set_type(Type type);

    std::string to_string() const;

private:

    JsonValue(const Object& _value);
    JsonValue(const Array& _array);
    JsonValue(Object&& _value);
    JsonValue(Array&& _value);
    
    void verify_type(Type expected) const;
    void verify_index(int index) const;

    std::string object_to_string() const;
    std::string array_to_string() const;

    var_t value;
};