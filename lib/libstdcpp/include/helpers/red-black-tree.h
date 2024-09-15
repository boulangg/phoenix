/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */
 
 #pragma once

#include <iterator>
#include <stdexcept>
#include <utility>

namespace std {
namespace details {

enum red_black_color
{
    BLACK,
    RED
};

template <class Value>
struct red_black_node
{
    red_black_color color;
    Value value;
    red_black_node* parent;
    red_black_node* left;
    red_black_node* right;
};

template <class Key, class Value, class Extractor, class Comparator>
class red_black_tree;

template <class Value>
class red_black_tree_iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = Value;
    using pointer = Value*;
    using reference = Value&;

    using node_type = red_black_node<Value>;

private:
    node_type* _node;

public:
    red_black_tree_iterator(node_type* node) : _node(node) {}
    // red_black_tree_iterator(const red_black_tree_iterator& it) : _node(it._node) {}

    // red_black_tree_iterator& operator=(const red_black_tree_iterator& it)
    //{
    //     _node = it._node;
    //     return *this;
    // }

    red_black_tree_iterator& operator++()
    {
        if (_node->right != nullptr) {
            _node = _node->right;
            while (_node->left != nullptr) {
                _node = _node->left;
            }
        } else {
            while (_node->parent != nullptr && _node->parent->left != _node) {
                _node = _node->parent;
            }
            _node = _node->parent;
        }

        return *this;
    }

    red_black_tree_iterator operator++(int)
    {
        red_black_tree_iterator it(*this);
        ++(*this);
        return it;
    }

    red_black_tree_iterator& operator--()
    {
        if (_node->left != nullptr) {
            _node = _node->left;
            while (_node->right != nullptr) {
                _node = _node->right;
            }
        } else {
            while (_node->parent != nullptr && _node->parent->right != _node) {
                _node = _node->parent;
            }
            _node = _node->parent;
        }

        return *this;
    }

    red_black_tree_iterator operator--(int)
    {
        red_black_tree_iterator it(*this);
        --(*this);
        return it;
    }

    pointer operator->() const
    {
        return &_node->value;
    }

    reference operator*() const
    {
        return _node->value;
    }

    // friend void swap(_hashtable_iterator& lhs, _hashtable_iterator& rhs);
    template <class T>
    friend bool operator==(const red_black_tree_iterator<T>&, const red_black_tree_iterator<T>&);
    template <class T>
    friend bool operator!=(const red_black_tree_iterator<T>&, const red_black_tree_iterator<T>&);

    template <class, class, class, class>
    friend class red_black_tree;
};

template <class Value>
bool operator==(const red_black_tree_iterator<Value>& x, const red_black_tree_iterator<Value>& y)
{
    return x._node == y._node;
}

template <class Value>
bool operator!=(const red_black_tree_iterator<Value>& x, const red_black_tree_iterator<Value>& y)
{
    return x._node != y._node;
}
template <class Key, class Value, class Extractor, class Comparator>
class red_black_tree
{
public:
    using iterator = red_black_tree_iterator<Value>;
    using node_type = typename iterator::node_type;
    using value_type = typename iterator::value_type;
    using key_type = Key;
    using size_type = size_t;

    using extractor_type = Extractor;
    using compare = Comparator;

private:
    size_type _node_count;
    using node_type_ptr = node_type*;
    node_type_ptr _root;

    extractor_type _extractor;
    compare _comparator;

public:
    red_black_tree() : _node_count(0), _root(nullptr), _extractor(), _comparator() {}

    red_black_tree(const red_black_tree& other) :
        _node_count(0), _root(nullptr), _extractor(other._extractor), _comparator(other._comparator)
    {
        for (auto& val : other) {
            this->insert(val);
        }
    }

    red_black_tree(red_black_tree&& other) : red_black_tree()
    {
        swap(*this, other);
    }

    ~red_black_tree()
    {
        clear();
    }

    red_black_tree& operator=(red_black_tree other)
    {
        swap(*this, other);
        return *this;
    }

    friend void swap(red_black_tree& first, red_black_tree& second)
    {
        // enable ADL
        using std::swap;

        swap(first._node_count, second._node_count);
        swap(first._begin, second._begin);
        swap(first._end, second._end);
        swap(first._extractor, second._extractor);
        swap(first._comparator, second._comparator);
    }

    // Capacity
    bool empty() const
    {
        return size() == 0;
    }

    size_type size() const
    {
        return _node_count;
    }

    // Iterators
    iterator begin() const
    {
        return iterator(_minimum(_root));
    }

    iterator end() const
    {
        return iterator(nullptr);
    }

    // Lookup
    value_type& at(const key_type& key)
    {
        iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range();
        } else {
            return *it;
        }
    }

    iterator find(const key_type& key) const
    {
        if (_node_count == 0) {
            return end();
        }

        node_type_ptr node = _root;

        while (node != nullptr) {
            auto currentNodeKey = _extract(node->value);
            if (_compare_key(currentNodeKey, key)) {
                node = node->left;
            } else if (_compare_key(key, currentNodeKey)) {
                node = node->right;
            } else {
                return iterator(node);
            }
        }

        // node_type_ptr node = _get_root();
        // while (node != nullptr) {
        //     if (_compare_key(node->value, key)) {
        //         node = node->left;
        //     } else if (_compare_key(key, _extract(node->value))) {
        //         node = node->right;
        //     } else {
        //         return iterator(node);
        //     }
        // }

        return end();
    }

    bool contains(const key_type& key) const
    {
        return find(key) != end();
    }

    iterator lower_bound(const key_type& key)
    {
        // TODO
    }

    iterator upper_bound(const key_type& key)
    {
        // TODO
    }

    // Modifiers
    void clear() noexcept
    {
        if (_node_count == 0) {
            return;
        }

        node_type_ptr node = _root;
        while (node != nullptr) {
            if (node->left != nullptr) {
                node = node->left;
            } else if (node->right != nullptr) {
                node = node->right;
            } else {
                node_type_ptr parent = node->parent;
                if (parent != nullptr && node == parent->left) {
                    parent->left = nullptr;
                } else if (parent != nullptr && node == parent->right) {
                    parent->right = nullptr;
                }
                delete node;
                node = parent;
            }
        }

        // node_type_ptr node = _get_root();
        // while (node != _get_root()) {
        //     if (node->left != nullptr) {
        //         node = node->left;
        //     } else if (node->right != nullptr) {
        //         node = node->right;
        //     } else {
        //         node_type_ptr parent = node->parent;
        //         delete node;
        //         node = parent;
        //     }
        // }

        _node_count = 0;
        _root = nullptr;
    }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        auto node = _insert(value);
        return std::pair<iterator, bool>(iterator(node.first), node.second);
    }

    iterator erase(iterator pos)
    {
        if (pos == end()) {
            return end();
        }
        auto next = std::next(pos);
        _erase(pos._node);
        return next;
    }

    size_type erase(const key_type& key)
    {
        iterator it = find(key);
        if (it == end()) {
            return 0;
        } else {
            erase(it);
            return 1;
        }
    }

private:
    bool _is_left_child(node_type_ptr y)
    {
        if (y->parent == nullptr) {
            throw std::runtime_error("is_left_child: no parent");
        }

        return (y->parent->left == y);
    }

    bool _is_right_child(node_type_ptr y)
    {
        if (y->parent == nullptr) {
            throw std::runtime_error("is_right_child: no parent");
        }

        return (y->parent->right == y);
    }

    void _left_rotate(node_type_ptr x)
    {
        node_type_ptr y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            _root = y;
        } else if (_is_left_child(x)) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    void _right_rotate(node_type_ptr x)
    {
        node_type_ptr y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            _root = y;
        } else if (_is_right_child(x)) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    void _transplant(node_type_ptr x, node_type_ptr y)
    {
        if (x == _root) {
            _root = y;
        } else if (_is_left_child(x)) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->parent = x->parent;
    }

    node_type_ptr _minimum(node_type_ptr node) const
    {
        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    node_type_ptr _maximum(node_type_ptr node) const
    {
        while (node->right != nullptr) {
            node = node->right;
        }

        return node;
    }

    std::pair<node_type_ptr, bool> _insert(value_type value)
    {
        node_type_ptr parent = nullptr;
        node_type_ptr child = _root;
        while (child != nullptr) {
            parent = child;
            if (_compare_value(value, parent->value)) {
                child = parent->left;
            } else if (_compare_value(parent->value, value)) {
                child = parent->right;
            } else {
                return std::make_pair(parent, false);
            }
        }

        auto node = new node_type{red_black_color::RED, value, nullptr, nullptr, nullptr};
        ++_node_count;

        if (parent == nullptr) {
            _root = node;
            node->color = red_black_color::BLACK;
            return std::make_pair(node, true);
        }

        node->parent = parent;

        if (_compare_value(value, parent->value)) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        _rebalance_after_insert(node);

        return std::make_pair(node, true);
    }

    void _rebalance_after_insert(node_type_ptr node)
    {
        node_type_ptr x = node;
        while (x->parent->color == red_black_color::RED) {
            node_type_ptr p = x->parent;  // parent
            node_type_ptr gp = p->parent; // grandparent
            if (_is_left_child(p)) {
                node_type_ptr u = gp->right; // uncle
                if (u != nullptr && u->color == red_black_color::RED) {
                    p->color = red_black_color::BLACK;
                    u->color = red_black_color::BLACK;
                    x = gp;
                } else {
                    if (_is_right_child(x)) {
                        x = p;
                        _left_rotate(x);
                    }
                    x->parent->color = red_black_color::BLACK;
                    x->parent->parent->color = red_black_color::RED;
                    _right_rotate(x->parent->parent);
                }
            } else {
                node_type_ptr u = gp->left; // uncle
                if (u != nullptr && u->color == red_black_color::RED) {
                    u->color = red_black_color::BLACK;
                    p->color = red_black_color::BLACK;
                    x = gp;
                } else {
                    if (_is_left_child(x)) {
                        x = p;
                        _right_rotate(x);
                    }
                    x->parent->color = red_black_color::BLACK;
                    x->parent->parent->color = red_black_color::RED;
                    _left_rotate(x->parent->parent);
                }
            }

            if (x == _root) {
                break;
            }
        }

        _root->color = red_black_color::BLACK;
    }

    void _erase(node_type_ptr node)
    {
        --_node_count;

        red_black_color originalColor = node->color;

        node_type_ptr x, y;
        if (node->left == nullptr) {
            x = node->right;
            _transplant(node, x);
        } else if (node->right == nullptr) {
            x = node->left;
            _transplant(node, x);
        } else {
            y = _minimum(node->right);
            originalColor = y->color;
            x = y->right;
            if (y->parent == node) {
                // x->parent = y; useless ??
            } else {
                _transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }

            _transplant(node, y);
            y->left = node->left;
            y->left->parent = y;

            y->color = node->color;
        }

        delete node;

        if (originalColor == red_black_color::BLACK) {
            _rebalance_after_erase(x);
        }
    }

    void _rebalance_after_erase(node_type_ptr node)
    {
        node_type_ptr x = node;
        while (x != _root && x->color == red_black_color::BLACK) {
            if (_is_left_child(x)) {
                node_type_ptr w = x->parent->right;
                if (w->color == red_black_color::RED) {
                    w->color = red_black_color::BLACK;
                    x->parent->color = red_black_color::RED;
                    _left_rotate(x->parent);
                    w = x->parent->right;
                }

                if (w->left->color == red_black_color::BLACK && w->right->color == red_black_color::BLACK) {
                    w->color = red_black_color::RED;
                    x = x->parent;
                } else {
                    if (w->right->color == red_black_color::BLACK) {
                        w->left->color = red_black_color::BLACK;
                        w->color = red_black_color::RED;
                        _right_rotate(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = red_black_color::BLACK;
                    w->right->color = red_black_color::BLACK;
                    _left_rotate(x);
                    x = _root;
                }
            } else {
                node_type_ptr w = x->parent->left;
                if (w->color == red_black_color::RED) {
                    w->color = red_black_color::BLACK;
                    x->parent->color = red_black_color::RED;
                    _right_rotate(x->parent);
                    w = x->parent->left;
                }

                if (w->left->color == red_black_color::BLACK && w->right->color == red_black_color::BLACK) {
                    w->color = red_black_color::RED;
                    x = x->parent;
                } else {
                    if (w->left->color == red_black_color::BLACK) {
                        w->right->color = red_black_color::BLACK;
                        w->color = red_black_color::RED;
                        _left_rotate(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = red_black_color::BLACK;
                    w->left->color = red_black_color::BLACK;
                    _right_rotate(x);
                    x = _root;
                }
            }
        }

        x->color = red_black_color::BLACK;
    }

    const key_type& _extract(const value_type& value) const
    {
        return _extractor(value);
    }

    bool _compare_key(const key_type& x, const key_type& y) const
    {
        return _comparator(x, y);
    }

    bool _compare_value(const value_type& x, const value_type& y) const
    {
        return _compare_key(_extract(x), _extract(y));
    }
};

}
}
