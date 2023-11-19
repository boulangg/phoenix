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

    red_black_tree_iterator(const red_black_tree_iterator& it) : _node(it._node) {}
    red_black_tree_iterator(node_type* node) : _node(node) {}

    red_black_tree_iterator& operator=(const red_black_tree_iterator& it)
    {
        _node = it._node;
        return *this;
    }

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

private:
    node_type* _node;
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
    node_type_ptr _begin;
    node_type_ptr _end;

    node_type_ptr _get_root() const
    {
        return _end->left;
    }

    void _set_root(node_type_ptr x) const
    {
        _end->left = x;
        x->parent = _end;
    }

    extractor_type _extractor;
    compare _comparator;

public:
    red_black_tree() : _node_count(0), _end(new node_type()), _extractor(), _comparator()
    {
        _begin = _end;
    }

    ~red_black_tree()
    {
        clear();
        delete _end;
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
        return _begin;
    }

    iterator end() const
    {
        return _end;
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
            return _end;
        }

        node_type_ptr node = _get_root();
        while (node != nullptr) {
            if (_compare_key(node->value, key)) {
                node = node->left;
            } else if (_compare_key(key, _extract(node->value))) {
                node = node->right;
            } else {
                return iterator(node);
            }
        }

        return _end;
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

        node_type_ptr node = _get_root();
        while (node != _get_root()) {
            if (node->left != nullptr) {
                node = node->left;
            } else if (node->right != nullptr) {
                node = node->right;
            } else {
                node_type_ptr parent = node->parent;
                delete node;
                node = parent;
            }
        }

        _node_count = 0;
    }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        auto it = find(_extract(value));
        if (it != end()) {
            return std::pair<iterator, bool>(std::move(it), false);
        } else {
            auto node = new node_type{red_black_color::BLACK, value, nullptr, nullptr, nullptr};
            _insert(node);
            return std::pair<iterator, bool>(iterator(node), true);
        }
    }

    iterator erase(iterator pos)
    {
        auto node = _erase(pos._node);
        return iterator(node);
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

    void _left_rotate(node_type_ptr y)
    {
        if (y == nullptr) {
            throw std::runtime_error("left_rotate: can't rotate NULL node");
        }

        node_type_ptr x = y->parent;
        if (x == nullptr) {
            throw std::runtime_error("left_rotate: can't rotate a node with a NULL parent");
        }

        if (_is_left_child(y)) {
            throw std::runtime_error("left_rotate: can't rotate a node which is not a left child");
        }

        if (y->left != nullptr) {
            y->left->parent = x;
        }
        x->left = y->left;

        if (x->parent == _end) {
            _set_root(y);
        } else {
            if (_is_left_child(x)) {
                x->parent->left = y;
            } else {
                x->parent->right = y;
            }
            y->parent = x->parent;
        }

        x->parent = y;
        y->left = x;
    }

    void _right_rotate(node_type_ptr x)
    {
        if (x == nullptr) {
            throw std::runtime_error("right_rotate: can't rotate NULL node");
        }

        node_type_ptr y = x->parent;
        if (y == nullptr) {
            throw std::runtime_error("right_rotate: can't rotate a node with a NULL parent");
        }

        if (_is_right_child(y)) {
            throw std::runtime_error("right_rotate: can't rotate a node which is not a right child");
        }

        if (x->right != nullptr) {
            x->right->parent = x;
        }
        y->right = x->right;

        if (y->parent == _end) {
            _set_root(x);
        } else {
            if (_is_right_child(y)) {
                y->parent->right = x;
            } else {
                y->parent->left = x;
            }
            x->parent = y->parent;
        }

        y->parent = x;
        x->right = y;
    }

    void _transplant(node_type_ptr x, node_type_ptr y)
    {
        if (x == _get_root()) {
            _set_root(y);
        } else if (_is_left_child(x)) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->parent = x->parent;
    }

    node_type_ptr _minimum(node_type_ptr node)
    {
        while (node->left != nullptr) {
            node = node->left;
        }

        return node;
    }

    node_type_ptr _maximum(node_type_ptr node)
    {
        while (node->right != nullptr) {
            node = node->right;
        }

        return node;
    }

    node_type_ptr _insert(node_type_ptr node)
    {
        node->left = nullptr;
        node->right = nullptr;

        ++_node_count;

        if (_get_root() == nullptr) {
            _set_root(node);
            node->color = red_black_color::BLACK;
            return node;
        }

        node_type_ptr parent = nullptr;
        node_type_ptr child = _get_root();
        while (child != nullptr) {
            parent = child;
            child = _compare_value(node->value, parent->value) ? parent->left : parent->right;
        }

        node->parent = parent;

        if (_compare_value(node->value, parent->value)) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        node->color = red_black_color::RED;

        if (_begin->left != nullptr) {
            _begin = _begin->left;
        }

        _rebalance_after_insert(node);

        return node;
    }

    void _rebalance_after_insert(node_type_ptr node)
    {
        node_type_ptr x = node;
        while (x->parent->color == red_black_color::RED) {
            node_type_ptr p = node->parent;
            node_type_ptr gp = p->parent;
            if (_is_left_child(p)) {
                if (gp->right != nullptr && gp->right->color == red_black_color::RED) {
                    gp->left->color = red_black_color::BLACK;
                    gp->right->color = red_black_color::BLACK;
                    x = gp;
                } else {
                    if (_is_right_child(p)) {
                        x = p;
                        p = gp;
                        gp = p->parent;
                        _left_rotate(x);
                    }
                    p->color = red_black_color::BLACK;
                    gp->color = red_black_color::RED;
                    _right_rotate(gp);
                }
            } else {
                if (gp->left != nullptr && gp->left->color == red_black_color::RED) {
                    gp->left->color = red_black_color::BLACK;
                    gp->right->color = red_black_color::BLACK;
                    x = gp;
                } else {
                    if (_is_left_child(p)) {
                        x = p;
                        p = gp;
                        gp = p->parent;
                        _right_rotate(x);
                    }
                    p->color = red_black_color::BLACK;
                    gp->color = red_black_color::RED;
                    _left_rotate(gp);
                }
            }

            if (x == _get_root()) {
                break;
            }
        }

        _get_root()->color = red_black_color::BLACK;
    }

    node_type_ptr _erase(node_type_ptr node)
    {
        --_node_count;

        red_black_color originalColor = node->color;

        if (node == _begin) {
            if (node->right != nullptr) {
                _begin = node->right;
            } else {
                _begin = node->parent;
            }
        }

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
                x->parent = y; // weird
            } else {
                _transplant(y, y->right);
            }

            _transplant(node, y);
            y->color = originalColor;
        }

        if (originalColor == red_black_color::BLACK) {
            _rebalance_after_erase(x);
        }

        return x;
    }

    void _rebalance_after_erase(node_type_ptr node)
    {
        node_type_ptr x = node;
        node_type_ptr w;
        while (x != _get_root() && x->color == red_black_color::BLACK) {
            if (_is_left_child(x)) {
                w = x->parent->right;
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
                    x = _get_root();
                }
            } else {
                w = x->parent->left;
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
                    x = _get_root();
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
