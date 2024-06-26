#include "buffer_reader.h"

BufferReader::BufferReader(std::istream& _stream): next_pos{std::nullopt}, cur_read_size{0}, next_byte_status{Status::OKAY}, stream{_stream} {
    update_buffer();
};

BufferReader::operator bool() const {
    return next_byte_status == Status::OKAY;
}

BufferReader::Status BufferReader::status() const {
    return next_byte_status;
}

std::optional<char> BufferReader::next_byte() {
    if (next_byte_status == Status::OKAY) {
        char return_val = buffer[*next_pos];
        next_pos = *next_pos + 1;
        if (*next_pos >= cur_read_size) [[unlikely]] {
            update_buffer();
        }
        return return_val;
    } else return std::nullopt;
}

std::optional<char> BufferReader::peek() const {
    if (next_byte_status == Status::OKAY) {
        return buffer[*next_pos];
    } else return std::nullopt;
}

char BufferReader::throw_next_byte() {
    std::optional<char> cur_read = next_byte();
    if (cur_read.has_value()) [[likely]] return *cur_read;
    throw std::runtime_error("Invalid next_byte");
}

char BufferReader::throw_peek() const {
    std::optional<char> cur_read = peek();
    if (cur_read.has_value()) [[likely]] return *cur_read;
    throw std::runtime_error("Invalid peek");
}

void BufferReader::update_buffer() {
    // check current state of stream
    if (next_byte_status != Status::OKAY) return;

    if (stream.good()) {
        stream.read(buffer, BUFFER_SIZE);
        cur_read_size = stream.gcount();
        next_pos = 0;
    } else {
        next_pos = std::nullopt;
        cur_read_size = 0;

        if (stream.eof()) {
            next_byte_status = Status::END_OF_FILE;
        } else {
            next_byte_status = Status::FAIL;
        }
    }
}

