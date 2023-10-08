#pragma once

#include <cmath>
#include <iterator>
#include <list>

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
class hashtable_iterator {
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = Value;
	using pointer = Value*;
	using reference = Value&;

	using node_type = hashtable_node<Value>;

	hashtable_iterator(const hashtable_iterator& it) :
		_bucket(it._bucket), _node(it._node)
	{}
	hashtable_iterator(node_type** bucket, node_type* node) :
		_bucket(bucket), _node(node)
	{}

	~hashtable_iterator()
	{}

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

	//friend void swap(_hashtable_iterator& lhs, _hashtable_iterator& rhs);
	template<class T>
	friend bool operator==(const hashtable_iterator<T>&, const hashtable_iterator<T>&);
	template<class T>
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
	node_type** _buckets;
	using node_type_ptr = node_type*;
	node_type_ptr _end;

	extractor_type _extractor;
	hasher _hash;
	key_equal _key_equal;

public:
	hashtable(size_type bucket_count, extractor_type extractor, hasher hash, key_equal equal) :
		_bucket_count(bucket_count), _node_count(0), _max_load_factor(1), _extractor(extractor), _hash(hash), _key_equal(equal)
	{
		_buckets = new node_type_ptr[bucket_count + 1];

		_end = new node_type();
		_end->next = _end;
		_buckets[bucket_count] = _end;
	}

	~hashtable()
	{}

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
	iterator begin()
	{
		return iterator(_buckets, *_buckets);
	}

	iterator end()
	{
		return iterator(_buckets + _bucket_count, _end);
	}

	// Lookup
	Value& at(const key_type& key)
	{
		iterator it = find(key);
		if (it == end()) {
			throw std::out_of_range();
		} else {
			return *it;
		}
	}

	iterator find(const Key& key)
	{
		size_t i = reduce(key);
		node_type** bucket = _buckets + i;
		node_type* node = find_node(*bucket, key);
		if (node == nullptr) {
			return end();
		} else {
			return iterator(bucket, node);
		}
	}

	bool contains(const Key& key)
	{
		return find(key) != end();
	}

	// Modifiers
	void clear() noexcept
	{
		node_type** bucket = _buckets;
		while (*bucket != _end) {
			if (*bucket == nullptr) {
				++bucket;
			} else {
				node_type* oldNode = *bucket;
				*bucket = (*bucket)->next;
				--_node_count;
				delete oldNode;
			}
		}
	}

	std::pair<iterator, bool> insert(const value_type& value)
	{
		const key_type& key = extract(value);

		iterator it = find(key);
		if (it != end()) {
			return std::pair<iterator, bool>(std::move(it), false);
		}

		size_t i = reduce(key);
		node_type** bucket = _buckets + i;
		node_type* newNode = new node_type{ value, *bucket };
		*bucket = newNode;
		++_node_count;

		if (load_factor() > max_load_factor()) {
			rehash(0);
		}

		return std::pair(find(key), true);
	}

	iterator erase(iterator pos)
	{
		node_type** bucket = hashtable_base<Value>::get_bucket(pos);
		while (*bucket != hashtable_base<Value>::get_node(pos)) {
			bucket = &(*bucket)->next;
		}

		node_type* oldNode = *bucket;
		*bucket = (*bucket)->next;
		--_node_count;
		++pos;
		delete oldNode;
		return pos;
	}

	size_type erase(const Key& key)
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
		node_type* node = *(_buckets + n);
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
	}

	void rehash(size_type count)
	{
		// TODO
	}

	void reserve(size_type count)
	{
		rehash(std::ceil(count / max_load_factor()));
	}

private:
	const key_type& extract(const value_type& value) const
	{
		return _extractor(value);
	}

	size_type reduce(const key_type& key)
	{
		return _hash(key) % _bucket_count;
	}

	bool compare(const key_type& key, const value_type& value)
	{
		return _key_equal(key, extract(value));
	}

	node_type* find_node(node_type* node, const key_type& key)
	{
		while (node != nullptr) {
			if (compare(key, node->value)) {
				return node;
			}

			node = node->next;
		}

		return nullptr;
	}
};

}
}
