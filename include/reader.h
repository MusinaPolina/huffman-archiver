#pragma once
#include <iostream>
#include <queue>
#include "exceptions.h"

namespace DataProcessing {
    class Reader {
    public:
        explicit Reader(std::istream& stream);

        uint8_t readBits(size_t count);
        bool readBit();

        uint32_t readInt();

        bool canRead();
        uint32_t bytesExtracted();

        void close();
    private:
        bool readByte();
        std::queue<bool> buffer_;

        std::istream& stream_;
    };
}