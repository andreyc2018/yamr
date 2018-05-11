#pragma once

#include "logger.h"
#include <map>
#include <iostream>

class TrieNode
{
    public:
        using container_t = std::string;
        using type_t = container_t::value_type;
        using node_t = TrieNode;
        using children_t = std::map<type_t, node_t>;
        using iterator = children_t::iterator;
        using const_iterator = children_t::const_iterator;

        TrieNode() : value_() {}
        TrieNode(const type_t& v) : value_(v) {}
        TrieNode(TrieNode&& rv) = default;
        TrieNode(const TrieNode& v) = default;

        TrieNode& operator=(TrieNode&& rv) = default;
        TrieNode& operator=(const TrieNode& v) = default;

        iterator find(const type_t& v) { return children_.find(v); }
        const_iterator find(const type_t& v) const { return children_.find(v); }

        iterator begin() { return children_.begin(); }
        const_iterator begin() const { return children_.begin(); }
        iterator end() { return children_.end(); }
        const_iterator end() const { return children_.end(); }

        bool operator==(const type_t& r) const { return value_ == r; }
        bool operator!=(const type_t& r) const { return !operator==(r); }
        bool operator==(const TrieNode& r) const { return operator==(r.value_); }
        bool operator!=(const TrieNode& r) const { return !operator==(r.value_); }

        const type_t& value() const { return value_; }
        node_t& node(const type_t& v) { return children_[v]; }
        node_t& append(const type_t& v) { children_[v] = TrieNode(v); return children_[v]; }

        friend std::ostream& operator<<(std::ostream& out, const TrieNode& node);
    private:
        type_t value_;
        children_t children_;
};

std::ostream& operator<<(std::ostream& out, const TrieNode& node)
{
    out << node.value() << '\n';
    for (auto& n : node) {
        out << n.first << ' ';
    }
    out << '\n';
    return out;
}

class Trie
{
    public:
        using node_t = TrieNode;
        using container_t = node_t::container_t;

        Trie() {}

//        T find(const std::string& /*data*/)
//        {
//        }

        void add(const container_t& data)
        {
            node_t& current = root_;
            for (size_t i = 0; i < data.size(); ++i) {
                auto& e = data[i];
                std::cout << e << " ";
                if (current != e) {
                    auto it = current.find(e);
                    if (it == current.end()) {
                        current = current.append(e);
                    }
                    else {
                        current = current.node(e);
                    }
                }
            }
            std::cout << '\n';
        }

        const node_t& root() const { return root_; }

        friend std::ostream& operator<<(std::ostream& out, const Trie& trie);

    private:
        node_t root_;
};

//std::ostream& operator<<(std::ostream& out, const Trie& trie)
//{
//    Trie::node_t& current = trie.root_;
//}
