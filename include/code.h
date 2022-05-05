#pragma once
#include <cinttypes>
#include <cstddef>
#include <vector>

namespace Huffman {

    class Code {
    public:
        Code(uint8_t symbol, std::vector<bool> code);
        explicit Code(uint8_t symbol);

        std::vector<bool> code_;

        [[nodiscard]] size_t length() const;
        uint8_t symbol() const;

        bool operator<(const Code& a) const;

    private:
        uint8_t symbol_;
    };

}
