#include "json.h"
#include "buffer_reader.h"

#include <iostream>
#include <optional>

std::optional<JsonValue> parse(std::istream& input);

std::optional<JsonValue> parse_value(BufferReader& reader);

std::optional<JsonValue> parse_bool(BufferReader& reader);

std::optional<JsonValue> parse_null(BufferReader& reader);

std::optional<JsonValue> parse_string(BufferReader& reader);

std::optional<JsonValue> parse_number(BufferReader& reader);

std::optional<JsonValue> parse_object(BufferReader& reader);

std::optional<JsonValue> parse_array(BufferReader& reader);

std::optional<std::string> read_num_string(BufferReader& reader);

std::optional<std::string> read_string(BufferReader& reader);

std::optional<std::string> read_escape_sequence(BufferReader& reader);

std::optional<std::pair<std::string, JsonValue>> read_key_value_pair(BufferReader& reader);

void consume_whitespace(BufferReader& reader);

bool is_whitespace(char c);

bool is_whitespace(std::optional<char> c);

bool is_hex(char c);