/*
 * Copyright (c) 2016-2023 Boulanger Guillaume, Chathura Namalgamuwa
 * The file is distributed under the MIT license
 * The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE
 */

#pragma once

#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <list>
#include <stdexcept>
#include <utility>

namespace std {
namespace detail {

template <class Value>
struct hashtable_node
{
    Value value;
    hashtable_node<Value>* next;
};

template <class Value>
class hashtable_base;

template <class Value>
class hashtable_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = Value;
    using pointer = Value*;
    using reference = Value&;

    using node_type = hashtable_node<Value>;

    hashtable_iterator(const hashtable_iterator& it) : _bucket(it._bucket), _node(it._node) {}
    hashtable_iterator(node_type** bucket, node_type* node) : _bucket(bucket), _node(node) {}

    hashtable_iterator& operator=(const hashtable_iterator& it)
    {
        _bucket = it._bucket;
        _node = it._node;
        return *this;
    }

    hashtable_iterator& operator++()
    {
        _node = _node->next;
        if (_node == nullptr) {
            ++_bucket;
            while (!*_bucket) {
                ++_bucket;
            }
            _node = *_bucket;
        }

        return *this;
    }

    hashtable_iterator operator++(int)
    {
        hashtable_iterator it(*this);
        ++(*this);
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
    friend bool operator==(const hashtable_iterator<T>&, const hashtable_iterator<T>&);
    template <class T>
    friend bool operator!=(const hashtable_iterator<T>&, const hashtable_iterator<T>&);

    friend class hashtable_base<Value>;

private:
    node_type** _bucket;
    node_type* _node;
};

template <class Value>
bool operator==(const hashtable_iterator<Value>& x, const hashtable_iterator<Value>& y)
{
    return x._node == y._node;
}

template <class Value>
bool operator!=(const hashtable_iterator<Value>& x, const hashtable_iterator<Value>& y)
{
    return x._node != y._node;
}

template <class Value>
class hashtable_base
{
protected:
    using iterator = hashtable_iterator<Value>;
    using node_type = typename iterator::node_type;

    node_type** get_bucket(iterator it)
    {
        return it._bucket;
    }

    node_type* get_node(iterator it)
    {
        return it._node;
    }
};

struct hashtable_rehash_policy
{
    using size_type = size_t;
    static constexpr size_t _default_bucket_count = 16;

    size_type reduce(const size_type bucket_count, const size_type& hash) const
    {
        return hash % bucket_count;
    }

    size_type rehash_optimal_bucket_count(const float min_bucket_count) const
    {
        return rehash_optimal_bucket_count(static_cast<size_t>(min_bucket_count));
    }

    size_type rehash_optimal_bucket_count(const size_type min_bucket_count) const
    {
        return std::bit_ceil(min_bucket_count);
    }
};

template <class Key, class Value, class Extractor, class Hash, class KeyEqual>
class hashtable : private hashtable_base<Value>
{
public:
    using iterator = hashtable_iterator<Value>;
    using node_type = typename iterator::node_type;
    using value_type = typename iterator::value_type;
    using key_type = Key;
    using size_type = size_t;

    using extractor_type = Extractor;
    using hasher = Hash;
    using key_equal = KeyEqual;

private:
    size_type _bucket_count;
    size_type _node_count;
    float _max_load_factor;
    using node_type_ptr = node_type*;
    node_type_ptr* _buckets;
    node_type_ptr _end;

    extractor_type _extractor;
    hasher _hash;
    key_equal _key_equal;
    hashtable_rehash_policy _rehash_policy;

public:
    hashtable(size_type bucket_count, extractor_type extractor, hasher hash, key_equal equal) :
        _node_count(0), _max_load_factor(1), _extractor(extractor), _hash(hash), _key_equal(equal), _rehash_policy()
    {
        _bucket_count = _rehash_policy.rehash_optimal_bucket_count(bucket_count);

        _buckets = new node_type_ptr[_bucket_count + 1]();

        _end = new node_type();
        _end->next = _end;
        _buckets[_bucket_count] = _end;
    }

    ~hashtable()
    {
        clear();
        delete _end;
        delete _buckets;
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
        return iterator(_buckets, *_buckets);
    }

    iterator end() const
    {
        return iterator(_buckets + _bucket_count, _end);
    }

    // Lookup
    value_type& at(const key_type& key)
    {
        iterator it = find(key);
        if (it == end()) {
            throw std::out_of_range("hashtable out_of_range");
        } else {
            return *it;
        }
    }

    iterator find(const key_type& key) const
    {
        size_t i = _reduce(key);
        node_type_ptr* bucket = _buckets + i;
        node_type_ptr node = _find_node(*bucket, key);
        if (node == nullptr) {
            return end();
        } else {
            return iterator(bucket, node);
        }
    }

    bool contains(const key_type& key) const
    {
        return find(key) != end();
    }

    // Modifiers
    void clear() noexcept
    {
        node_type_ptr* bucket = _buckets;
        while (*bucket != _end) {
            if (*bucket == nullptr) {
                ++bucket;
            } else {
                node_type_ptr oldNode = *bucket;
                *bucket = (*bucket)->next;
                --_node_count;
                delete oldNode;
            }
        }
    }

    std::pair<iterator, bool> insert(const value_type& value)
    {
        const key_type& key = _extract(value);

        iterator it = find(key);
        if (it != end()) {
            return std::pair<iterator, bool>(std::move(it), false);
        }

        node_type_ptr newNode = new node_type{value, nullptr};
        _insert_node(key, newNode);
        ++_node_count;

        if (load_factor() > max_load_factor()) {
            rehash(0);
        }

        return std::pair(find(key), true);
    }

    iterator erase(iterator pos)
    {
        node_type_ptr* bucket = hashtable_base<Value>::get_bucket(pos);
        while (*bucket != hashtable_base<Value>::get_node(pos)) {
            bucket = &(*bucket)->next;
        }

        node_type_ptr oldNode = *bucket;
        *bucket = (*bucket)->next;
        --_node_count;
        ++pos;
        delete oldNode;
        return pos;
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

    // Bucket Interface
    size_type bucket_count() const
    {
        return _bucket_count;
    }

    size_type bucket_size(size_type n) const
    {
        if (n >= _bucket_count) {
            return 0;
        }

        size_type size = 0;
        node_type_ptr node = *(_buckets + n);
        while (node != nullptr) {
            ++size;
            node = node->next;
        }

        return size;
    }

    size_type bucket(const Key& key) const
    {
        auto it = find(key);
        auto bucket_ptr = get_bucket(it);
        size_type bucket_nb = (bucket_ptr - _buckets) / sizeof(node_type*);
        return bucket_nb;
    }

    // Hash policy
    float load_factor() const
    {
        return _node_count / (float)_bucket_count;
    }

    float max_load_factor() const
    {
        return _max_load_factor;
    }

    void max_load_factor(float ml)
    {
        _max_load_factor = ml;

        if (load_factor() > max_load_factor()) {
            rehash(0);
        }
    }

    void rehash(size_type count)
    {
        size_t min_bucket_count = std::ceil(_node_count / max_load_factor());
        min_bucket_count = std::max(min_bucket_count, count);
        _bucket_count = _rehash_policy.rehash_optimal_bucket_count(min_bucket_count);

        node_type_ptr* oldBucketPtr = _buckets;
        node_type_ptr* currentBbucket = _buckets;
        _buckets = new node_type_ptr[_bucket_count]();

        while (*currentBbucket != _end) {
            if (*currentBbucket == nullptr) {
                ++currentBbucket;
            } else {
                node_type_ptr node = *currentBbucket;
                *currentBbucket = (*currentBbucket)->next;
                _insert_node(_extract(node->value), std::move(node));
            }
        }
        _buckets[_bucket_count] = _end;

        delete oldBucketPtr;
    }

    void reserve(size_type count)
    {
        rehash(std::ceil(count / max_load_factor()));
    }

private:
    const key_type& _extract(const value_type& value) const
    {
        return _extractor(value);
    }

    size_type _reduce(const key_type& key) const
    {
        return _rehash_policy.reduce(_bucket_count, _hash(key));
    }

    bool _compare(const key_type& key, const value_type& value) const
    {
        return _key_equal(key, _extract(value));
    }

    node_type_ptr _find_node(node_type_ptr node, const key_type& key) const
    {
        while (node != nullptr) {
            if (_compare(key, node->value)) {
                return node;
            }

            node = node->next;
        }

        return nullptr;
    }

    void _insert_node(const key_type& key, node_type_ptr value)
    {
        size_t i = _reduce(key);
        node_type_ptr* bucket = _buckets + i;
        value->next = *bucket;
        *bucket = value;
    }
};

}
}
