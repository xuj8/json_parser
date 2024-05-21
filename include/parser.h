#include "json.h"
#include "buffer_reader.h"

#include <iostream>
#include <optional>

std::optional<JsonValue> parse(std::istream& input);

void consume_whitespace(BufferReader& reader);

std::optional<JsonValue> parse_string(BufferReader& reader);

std::optional<JsonValue> parse_number(BufferReader& reader);

std::optional<JsonValue> parse_object(BufferReader& reader);

std::optional<JsonValue> parse_array(BufferReader& reader);

bool is_whitespace(char c);