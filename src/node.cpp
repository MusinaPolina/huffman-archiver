#include "node.h"

namespace Huffman {

    Node::Node(uint8_t symbol): symbol_(symbol) {}

    Node::Node(uint8_t symbol, uint32_t weight): symbol_(symbol), weight_(weight) {}

    Node::Node(Node *l, Node *r) : left_(l), right_(r) {
        weight_ = left_->weight_ + right_->weight_;
    }

    Node::~Node() {
        delete left_;
        delete right_;
    }

    bool Node::isLeaf() {
        return left_ == nullptr && right_ == nullptr;
    }

    bool Node::operator<(const Node& a) const {
        return weight_ > a.weight_;
    }
}