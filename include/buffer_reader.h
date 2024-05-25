#pragma once
#include <iostream>
#include <fstream>
#include <optional>

struct BufferReader {
    enum class Status {
        END_OF_FILE,
        FAIL,
        OKAY
    };

    static constexpr size_t BUFFER_SIZE = 1 << 12;

    BufferReader(std::istream& _stream);

    operator bool() const;

    Status status() const;

    std::optional<char> next_byte();

    std::optional<char> peek() const;

    char throw_next_byte();

    char throw_peek() const;

private:
    void update_buffer();

    char buffer[BUFFER_SIZE];
    std::optional<size_t> next_pos;
    size_t cur_read_size = 0;
    Status next_byte_status = Status::OKAY;
    std::istream& stream;
};