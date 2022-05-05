#include "tree.h"
#include <vector>
#include <queue>
#include <algorithm>

namespace Huffman {

    Tree::Tree(std::map<uint8_t, uint32_t> frequency_table) {

        if (frequency_table.empty()) return;
        for (uint8_t i = 0; frequency_table.size() == 1; i++) frequency_table[i]++;

        auto compare = [](Node* l, Node* r) {return *l < *r; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> priorities(compare);

        for (auto p : frequency_table) {
            priorities.push(new Node(p.first, p.second));
        }

        while (priorities.size() > 1) {
            auto first = [&]() {
                Node* c = priorities.top();
                priorities.pop();
                return c;
            };

            Node *a = first();
            Node *b = first();

            priorities.push(new Node(a, b));
        }

        root_ = priorities.top();
    }

    Tree::~Tree() {
        delete root_;
    }

    void Tree::DFS(std::vector<Code>& codes, Node* node, std::vector<bool>& code) {

        if (!node) return;

        if (node->isLeaf()) {
            codes.emplace_back(node->symbol_, code);
            return;
        }

        code.push_back(false);
        DFS(codes, node->left_, code);
        code.pop_back();

        code.push_back(true);
        DFS(codes, node->right_, code);
        code.pop_back();
    }

    std::vector<Code> Tree::getCodes() {

        std::vector<Code> codes;
        std::vector<bool> dfs_code;

        DFS(codes, root_, dfs_code);

        return codes;
    }

    Tree::Tree(const std::vector<Code>& codes) {

        root_ = new Node();

        for (const auto& code: codes) {
            addCode(code);
        }
    }

    void Tree::addCode(const Code& code) {

        Node* node = root_;

        for (size_t i = 0; i < code.length(); i++) {

            auto &nxt = code.code_[i] ? node->right_ : node->left_;

            if (nxt == nullptr) {
                Node* c = new Node();
                nxt = c;
            }
            node = nxt;
        }
        node->symbol_ = code.symbol();
    }

    uint8_t Tree::getSymbol(DataProcessing::Reader& reader) {
        Node* node = root_;

        while (!node->isLeaf()) {
            bool path = reader.readBit();
            node = path ? node->right_ : node->left_;
            if (!node) {
                throw Exceptions::InvalidFileFormat("incorrect compression of file");
            }
        }
        return node->symbol_;
    }
}