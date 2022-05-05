#include "code.h"

namespace Huffman {

    Code::Code(uint8_t symbol): symbol_(symbol) { }
    Code::Code(uint8_t symbol, std::vector<bool> code): symbol_(symbol), code_(std::move(code)) {}

    size_t Code::length() const {
        return code_.size();
    }

    uint8_t Code::symbol() const {
        return symbol_;
    }

    bool Code::operator<(const Code& a) const {
        return symbol_ < a.symbol_;
    }
}
