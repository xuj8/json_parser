#include "parser.h"
#include <boost/lexical_cast.hpp>

std::optional<JsonValue> parse(std::istream& input) {
    std::optional<JsonValue> result;
    
    // setup reader
    BufferReader reader(input);

    // consume whitespace
    consume_whitespace(reader);

    // begin parsing
    if (!reader) 
        return std::nullopt;
    
    // only allowed json file level values are object or array
    switch(*reader.peek()) {
        case JsonConstants::OBJECT_START:
            result = parse_object(reader);
            break;
        case JsonConstants::ARRAY_START:
            result = parse_array(reader);
            break;
        default:
            return std::nullopt;
    }

    return result;
}

std::optional<JsonValue> parse_value(BufferReader& reader) {
    // consume whitespace
    consume_whitespace(reader);

    try {
        std::optional<JsonValue> value;
        switch(reader.throw_peek()) {
            case JsonConstants::STRING_QUOTE:
                return parse_string(reader);
            case JsonConstants::ARRAY_START:
                return parse_array(reader);
            case JsonConstants::OBJECT_START:
                return parse_object(reader);
            case 't':
            case 'f':
                return parse_bool(reader);
            case 'n':
                value = parse_null(reader);
                return value;
            default:
                if (isdigit(reader.throw_peek()) || reader.throw_peek() == JsonConstants::MINUS) 
                    return parse_number(reader);
                else return std::nullopt;
        }
    } catch (std::exception &) {
        return std::nullopt;
    }
}   

std::optional<JsonValue> parse_string(BufferReader& reader) {
    // consume whitespace
    std::optional<std::string> str = read_string(reader);

    if (!str.has_value()) return std::nullopt;
    return JsonValue(*str);
}

std::optional<JsonValue> parse_number(BufferReader& reader) {
    // consume whitespace
    consume_whitespace(reader);

    // grab the string first
    std::optional<std::string> num_string = read_num_string(reader);
    if (!num_string.has_value()) 
        return std::nullopt;

    JsonValue result;

    try {
        result.set_value(boost::lexical_cast<double>(*num_string));
    } catch (boost::bad_lexical_cast &) {
        return std::nullopt;
    }

    return result;
}

// see https://www.json.org/fatfree.html
std::optional<std::string> read_num_string(BufferReader& reader) {
    std::string result;
    consume_whitespace(reader);
    try {
        // if there is a sign, grab it
        if (reader.throw_peek() == JsonConstants::MINUS)
            result += reader.throw_next_byte();

        // grab the decimal part
        if (!isdigit(reader.throw_peek()))
            return std::nullopt;
        else {
            while(isdigit(reader.throw_peek())) {
                result += reader.throw_next_byte();
            }
        }

        // if there is fractional part, grab it
        if (reader.throw_peek() == JsonConstants::DECIMAL_POINT) {
            result += reader.throw_next_byte();
            if (!isdigit(reader.throw_peek()))
                return std::nullopt;
            while(isdigit(reader.throw_peek())) {
                result += reader.throw_next_byte();
            }
        }

        // if there is exponent, grab it
        if (tolower(reader.throw_peek()) == JsonConstants::EXPONENT_UNCASED) {
            result += reader.throw_next_byte();

            // if there is sign, grab it
            if (reader.throw_peek() == JsonConstants::MINUS || reader.throw_peek() == JsonConstants::PLUS)
                result += reader.throw_next_byte();

            // grab digits
            if (!isdigit(reader.throw_peek()))
                return std::nullopt;
            
            while(isdigit(reader.throw_peek())) {
                result += reader.throw_next_byte();
            }
        }
        return result;
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<std::string> read_string(BufferReader& reader) {
    std::string result;
    consume_whitespace(reader);

    try {
        if (reader.throw_peek() != JsonConstants::STRING_QUOTE)
            return std::nullopt;
        reader.throw_next_byte();

        while(true) {
            if (reader.throw_peek() == JsonConstants::ESCAPE) {
                std::optional<std::string> escape_sequence = read_escape_sequence(reader);
                if (!escape_sequence.has_value()) return std::nullopt;
                else result += *escape_sequence;
            } else if (reader.throw_peek() == JsonConstants::STRING_QUOTE) {
                reader.throw_next_byte();
                break;
            } else {
                result += reader.throw_next_byte();
            }
        }
        return result;
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<std::string> read_escape_sequence(BufferReader& reader) {
    std::string result;
    consume_whitespace(reader);

    try {
        if (reader.throw_peek() != JsonConstants::ESCAPE) return std::nullopt;
        result += reader.throw_next_byte();
        switch (reader.throw_peek()) {
            case JsonConstants::HEX:
                // read 4 hex
                result += reader.throw_next_byte();
                for (int idx = 0; idx < 4; idx++) {
                    if (is_hex(reader.throw_peek()))
                        result += reader.throw_next_byte();
                    else   
                        return std::nullopt;
                }
                break;
            case JsonConstants::STRING_QUOTE:
            case JsonConstants::REVERSE_SLASH:
            case JsonConstants::SLASH:
            case JsonConstants::BACKSPACE:
            case JsonConstants::FORMFEED:
            case JsonConstants::LINEFEED:
            case JsonConstants::RETURN:
            case JsonConstants::TAB:
                result += reader.throw_next_byte();
                break;
            default:
                return std::nullopt;
        }
        return result;
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<JsonValue> parse_object(BufferReader& reader) {
    JsonValue result(JsonValue::Type::Object);

    try {
        // parse object begin
        if (reader.throw_peek() != JsonConstants::OBJECT_START)
            return std::nullopt;
        reader.throw_next_byte();

        // whitespace
        consume_whitespace(reader);

        // if there is string key-value pair, grab it and enter loop
        if (reader.throw_peek() == JsonConstants::STRING_QUOTE) {
            std::optional<std::pair<std::string, JsonValue>> kv = read_key_value_pair(reader);
            if (!kv.has_value()) return std::nullopt;
            result.set_index(kv->first, kv->second);
            
            while(true) {
                consume_whitespace(reader);
                if (reader.throw_peek() == JsonConstants::COMMA) {
                    reader.throw_next_byte();
                    consume_whitespace(reader);

                    std::optional<std::pair<std::string, JsonValue>> kv = read_key_value_pair(reader);
                    if (!kv.has_value()) return std::nullopt;
                    result.set_index(kv->first, kv->second);
                } else if (reader.throw_peek() == JsonConstants::OBJECT_END) {
                    break;
                } else return std::nullopt;
            }
        }

        consume_whitespace(reader);
        if (reader.throw_peek() != JsonConstants::OBJECT_END)
            return std::nullopt;
        reader.throw_next_byte();

        return result;

    
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<JsonValue> parse_array(BufferReader& reader) {
    try {
        consume_whitespace(reader);

        // consume beginning of array
        if (reader.throw_peek() != JsonConstants::ARRAY_START)
            return std::nullopt;
        reader.throw_next_byte();

        JsonValue result(JsonValue::Type::Array);
    
        consume_whitespace(reader);

        // if next thing is not array end, try to parse it as a value

        if (reader.throw_peek() != JsonConstants::ARRAY_END) {
            std::optional<JsonValue> value = parse_value(reader);
            if (!value.has_value()) return std::nullopt;
            result.push_back(*value);
            
            while(true) {
                consume_whitespace(reader);
                if (reader.throw_peek() == JsonConstants::COMMA) {
                    reader.throw_next_byte();
                    consume_whitespace(reader);
                    value = parse_value(reader);
                    if (!value.has_value()) return std::nullopt;
                    result.push_back(*value);
                } else if (reader.throw_peek() == JsonConstants::ARRAY_END) {
                    break;
                } else return std::nullopt;
            }
        }
        
        consume_whitespace(reader); 
        if (reader.throw_peek() != JsonConstants::ARRAY_END) return std::nullopt;
        reader.throw_next_byte();

        return result;
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<JsonValue> parse_bool(BufferReader& reader) {
    try {
        consume_whitespace(reader);
        if (reader.throw_peek() == 't') {
            if (reader.throw_next_byte() != 't' ||
                reader.throw_next_byte() != 'r' ||
                reader.throw_next_byte() != 'u' ||
                reader.throw_next_byte() != 'e')
                return std::nullopt;
            return true;
        } else if (reader.throw_peek() == 'f') {
            if (reader.throw_next_byte() != 'f' ||
                reader.throw_next_byte() != 'a' ||
                reader.throw_next_byte() != 'l' ||
                reader.throw_next_byte() != 's' ||
                reader.throw_next_byte() != 'e')
                return std::nullopt;
            return false;
        } else return std::nullopt;
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<JsonValue> parse_null(BufferReader& reader) {
    try {
        consume_whitespace(reader);
        if (reader.throw_peek() == 'n') {
            if (reader.throw_next_byte() != 'n' ||
                reader.throw_next_byte() != 'u' ||
                reader.throw_next_byte() != 'l' ||
                reader.throw_next_byte() != 'l')
                return std::nullopt;

            std::optional<JsonValue> value(JsonValue(nullptr));
            return value;
        } else return std::nullopt;
    } catch (std::exception &) {
        return std::nullopt;
    }
}

std::optional<std::pair<std::string, JsonValue>> read_key_value_pair(BufferReader& reader) {
    consume_whitespace(reader);
    try {
        if (reader.throw_peek() == JsonConstants::STRING_QUOTE) {
            // grab key-value pair
            std::optional<std::string> key = read_string(reader);
            if (!key.has_value()) return std::nullopt;
            
            // read delimiter
            consume_whitespace(reader);
            if (reader.throw_peek() != JsonConstants::KEY_VALUE_SEPARATOR) return std::nullopt;
            reader.throw_next_byte();

            // read value
            consume_whitespace(reader);
            std::optional<JsonValue> value = parse_value(reader);
            if (!value.has_value()) return std::nullopt;

            return std::pair(*key, *value);
        } else return std::nullopt;
    } catch (std::exception &e) {
        return std::nullopt;
    }
}   

void consume_whitespace(BufferReader& reader) {
    while(reader && is_whitespace(reader.peek())) {
        reader.next_byte();
    }
}

bool is_whitespace(char c) {
    return isspace(c);
}

bool is_whitespace(std::optional<char> c) {
    return c.has_value() && is_whitespace(*c);
}

bool is_hex(char c) {
    return std::isxdigit(static_cast<unsigned char>(c));
}