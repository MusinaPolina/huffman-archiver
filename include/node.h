#pragma once
#include <cinttypes>

namespace Huffman {

    class Node {
    public:
        Node() = default;
        explicit Node(uint8_t symbol);
        Node (uint8_t symbol, uint32_t weight);
        Node (Node* l, Node* r);
        ~Node();

        bool isLeaf();
        bool operator<(const Node& a) const;

        friend class Tree;

    private:
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        uint8_t symbol_ = 0;
        uint32_t weight_ = 0;
    };
}

