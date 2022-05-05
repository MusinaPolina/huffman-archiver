#include "reader.h"
#include <climits>

namespace DataProcessing {
    Reader::Reader(std::istream &stream): stream_(stream) {
        stream_.seekg(0, std::istream::beg);
    }

    bool reverseChar(uint8_t byte, size_t i) {
        return byte & (1 << (CHAR_BIT - 1 - i));
    }

    bool Reader::readByte() {
        if (!buffer_.empty()) {
            return false;
        }
        uint8_t byte;
        if (!stream_.read(reinterpret_cast<char*>(&byte), 1)) {
            throw Exceptions::ReadingError();
        }
        for (size_t i = 0; i < CHAR_BIT; i++) {
            buffer_.push(reverseChar(byte, i));
        }
        return true;
    }

    bool Reader::readBit() {
        readByte();
        bool bit = buffer_.front();
        buffer_.pop();
        return bit;
    }

    uint8_t Reader::readBits(size_t count) {
        uint8_t bits = 0;

        for (size_t i = 0; i < count; i++) {
            bits <<= 1;
            bits |= readBit();
        }

        return bits;
    }

    bool Reader::canRead() {
        if (!buffer_.empty()) return true;
        return stream_.peek() != EOF;
    }

    uint32_t Reader::bytesExtracted() {
        if (stream_.tellg() == -1) {
            stream_.clear();
            stream_.seekg(0, std::istream::end);
            return stream_.tellg();
        }
        return stream_.tellg();
    }

    uint32_t Reader::readInt() {
        uint32_t result = 0;
        for (size_t i = 0; i < UINT32_WIDTH; i++) {
            result <<= 1;
            result |= readBit();
        }
        return result;
    }

    void Reader::close() {
        while (!buffer_.empty()) {
            buffer_.pop();
        }
    }
}