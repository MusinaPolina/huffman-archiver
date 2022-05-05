#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "node.h"
#include "code.h"
#include "reader.h"

namespace Huffman {

    class Tree {
    public:
        explicit Tree(std::map<uint8_t, uint32_t> frequency_table);
        explicit Tree(const std::vector<Code>& codes);
        ~Tree();

        uint8_t getSymbol(DataProcessing::Reader& reader);
        std::vector<Code> getCodes();

    private:
        Node* root_ = nullptr;

        void addCode(const Code& code);
        void DFS(std::vector<Code>& codes, Node* node, std::vector<bool>& code);
    };
}